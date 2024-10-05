#include "system.hpp"

vector<Patient*> System::readPatientData(string file) {
    vector<Patient*> ret;

    ifstream f(file);
    string line;
    getline(f, line);  // Skip the header line

    random_device rd;
    mt19937 gen(rd());

    vector<string> reservoir(100);
    int lineCnt = 0;
    while (lineCnt < 100 && getline(f, line)) reservoir[lineCnt++] = line;  // Read the first 100 lines

    // Reservoir sampling for the rest of the lines
    while (getline(f, line)) {
        ++lineCnt;
        if (uniform_int_distribution<>(0, lineCnt - 1)(gen) < 100) {
            reservoir[uniform_int_distribution<>(0, 99)(gen)] = line;
        }
    }

    // Randomly generate 30 coordinates
    vector<pair<int, int>> coords;
    for (int j = 0; j < 30; j++) {
        coords.push_back({uniform_int_distribution<>(-20, 20)(gen), uniform_int_distribution<>(-20, 20)(gen)});
    }

    // Process the selected lines
    int id = 1;
    for (const auto& selectedLine : reservoir) {
        if (selectedLine.empty()) continue;  // Skip empty lines if any

        istringstream iss(selectedLine);
        string dept, temp, added_str;

        // Read department (dept)
        if (!getline(iss, dept, ',')) continue;

        // Skip intermediate fields
        for (int j = 0; j < 5; ++j) {
            if (!getline(iss, temp, ',')) break;
        }

        // Read medication time (added)
        if (!getline(iss, added_str, ',')) continue;

        // Check if department or medication time is empty
        if (dept.empty() || added_str.empty()) continue;

        // Convert medication time to minutes (assuming format is HH:MM)
        istringstream added_iss(added_str);
        string date, time;
        getline(added_iss, date, ' ');
        getline(added_iss, time);
        int hours = stoi(time.substr(0, 2));
        int minutes = stoi(time.substr(3, 2));
        int added = hours * 60 + minutes;

        if (added > 1080) continue;

        pair<int, int> coord = coords[uniform_int_distribution<>(0, 29)(gen)];

        Patient* p = new Patient(id++, coord, added);
        p->setRegion();
        ret.push_back(p);
    }

    return ret;
}

void System::addPatient(Patient* p) { patients.push_back(p); };

void System::addVehicle(Vehicle* v) { vehicles.push_back(v); };

void System::addSubsystem(Subsystem* s) { subsystems.push_back(s); };

void System::displayPlan() {
    cout << "\n>>>>> SYSTEM RESULT <<<<<" << endl;
    cout << "> Vehicle Assignment" << endl;
    for (auto& s : subsystems) {
        for (auto& v : s->vehicles) {
            cout << "  ID " << v->id << ", Region " << s->id << " => ";
            for (auto& deptTime : v->idealDeptTime) {
                printf("%02d:%02d ", deptTime / 60, deptTime % 60);
            }
            cout << endl;
        }
        cout << endl;
    }
}

float System::oneDayPerformance() {
    int totalIdleTime = 0;
    for (auto& v : vehicles) {
        for (int i = 1; i < v->tripNum; i++) {
            totalIdleTime += max(0, v->realDeptTime[i] - v->returnTime[i - 1]);
        }
    }
    avgIdleTime = totalIdleTime / (vehicles.size() * vehicles.front()->tripNum);

    int totalWaitingTime = 0, totalRetPatient = 0;
    for (auto& p : patients) {
        if (p->returned) {
            totalWaitingTime += p->getOnVehicleTime - p->addedTime;
            totalRetPatient++;
        }
    }
    avgWaitingTime = totalRetPatient ? totalWaitingTime / totalRetPatient : 0;

    /* calculate the total return time of missed patients (60 km/hr) */
    int missedPatients = patients.size() - totalRetPatient;
    for (auto& p : patients) {
        if (!p->returned) {
            auto [x, y] = p->dest;
            totalRetTime += round(sqrt(x * x + y * y));
        }
    }

    performance = (1000 - (avgIdleTime + 1.5 * avgWaitingTime + totalRetTime)) / 10.0;

    printf("> Performances\n");
    printf("  Idle Time: %02d:%02d\n", avgIdleTime / 60, avgIdleTime % 60);
    printf("  Average Waiting Time: %02d:%02d\n", avgWaitingTime / 60, avgWaitingTime % 60);
    printf("  Missed Patients: %d people\n", missedPatients);
    printf("  Total Return Time of Missed Patients: %02d:%02d\n", totalRetTime / 60, totalRetTime % 60);
    printf("  Total Performance: %.1f\n", performance);

    return performance;
}