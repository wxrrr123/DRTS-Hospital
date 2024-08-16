#include "system.hpp"

#include <random>

void System::readPatientData(string filename) {
    ifstream file(filename);
    string line;
    vector<string> reservoir(100);
    int lineCount = 0;

    // Skip the header line
    getline(file, line);

    random_device rd;
    mt19937 gen(rd());

    // Read the first 100 lines
    for (; lineCount < 100 && getline(file, line); ++lineCount) {
        reservoir[lineCount] = line;
    }

    // Reservoir sampling for the rest of the lines
    while (getline(file, line)) {
        ++lineCount;
        if (uniform_int_distribution<>(0, lineCount - 1)(gen) < 100) {
            reservoir[uniform_int_distribution<>(0, 99)(gen)] = line;
        }
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

        vector<pair<int, int>> coords;
        for (int j = 0; j < 30; j++) {
            coords.push_back({uniform_int_distribution<>(-20, 20)(gen), uniform_int_distribution<>(-20, 20)(gen)});
        }

        pair<int, int> coord = coords[uniform_int_distribution<>(0, 29)(gen)];

        addPatient(id++, dept, coord, added);
    }
}

void System::addPatient(int id, string dept, pair<int, int> dest, int added) {
    Patient* patient = new Patient(id, dept, dest, added);
    patient->setRegion();
    patients.push(patient);
}

void System::addVehicle(int id, int region) {
    Vehicle* vehicle = new Vehicle(id, region);
    vehicles.push_back(vehicle);
}

void System::generateSchedule() {
    /* randomly generate*/
    // set<int> comb;
    // for (auto& veh : vehicles) {
    //     while (comb.size() < veh->numberOfTrip) {
    //         comb.insert((rand() % 13 + 10) * 60);  // 10 ~ 22 o'clock
    //     }
    //     veh->idealDeptTime = vector<int>(comb.begin(), comb.end());
    //     veh->predDeptTime.push_back(veh->idealDeptTime.front());
    //     comb.clear();
    // }

    vector<vector<int>> schedule;
    schedule.push_back({660, 720, 840});
    schedule.push_back({600, 840, 1080});
    schedule.push_back({600, 720, 900});
    schedule.push_back({720, 840, 1020});
    schedule.push_back({600, 780, 1020});

    for (int i = 0; i < vehicles.size(); i++) {
        vehicles[i]->idealDeptTime = schedule[i];
        vehicles[i]->predDeptTime.push_back(vehicles[i]->idealDeptTime.front());
    }

    planReturnTrips();
}

void System::planReturnTrips() {
    while (clock <= 1080) {
        while (!patients.empty()) {
            Patient* patient = patients.top();
            if (patient->addedTime <= clock) {
                waitingLine[patient->region].push_back(patient);
                patients.pop();
            } else
                break;
        }

        for (auto& veh : vehicles) {
            vector<int> ideal = veh->idealDeptTime;
            vector<int> pred = veh->predDeptTime;
            int nth_dept = veh->predDeptTime.size() - 1;  // nth departure time
            if (nth_dept >= veh->numberOfTrip) continue;
            int deptTime = max(ideal[nth_dept], pred[nth_dept]);

            if (clock == deptTime) {
                printf("\nClock: %02d:%02d\n", clock / 60, clock % 60);
                printf("Vehicle ID: %d-%d, IdleTime: %02d:%02d\n", veh->id, nth_dept + 1,
                       (ideal[nth_dept] - pred[nth_dept]) / 60, (ideal[nth_dept] - pred[nth_dept]) % 60);

                /* Add patients into vehicles with different regions */
                while (!waitingLine[veh->region].empty() && veh->patients.size() < veh->capacity) {
                    Patient* firstPatient = waitingLine[veh->region].front();
                    firstPatient->getOnVehicleTime = clock;

                    veh->patients.push_back(firstPatient);
                    returnedPatients.push_back(firstPatient);

                    waitingLine[veh->region].pop_front();

                    printf("Patient %03d gets on. ", firstPatient->id);
                    printf("Dept: %4s, Dest: (%3d, %3d), AddedTime: %02d:%02d, WaitingTime: %02d:%02d\n",
                           firstPatient->department.c_str(), firstPatient->destination.first,
                           firstPatient->destination.second, firstPatient->addedTime / 60, firstPatient->addedTime % 60,
                           (clock - firstPatient->addedTime) / 60, (clock - firstPatient->addedTime) % 60);
                }

                veh->predictRoutingTime();
                veh->patients.clear();
            }
        }

        clock++;
    }
}

void System::displayPlan() {
    cout << "\n>>>>> SYSTEM RESULT <<<<<" << endl;
    cout << "> Schedule" << endl;
    for (auto& veh : vehicles) {
        cout << "  " << veh->id << ": ";
        int size = veh->predDeptTime.size();
        for (int i = 0; i < size - 1; i++) {
            int deptTime = max(veh->idealDeptTime[i], veh->predDeptTime[i]);
            printf("%02d:%02d ", deptTime / 60, deptTime % 60);
        }
        cout << endl;
    }
    cout << endl;
}

void System::calculatePerformance() {
    for (auto& veh : vehicles) {
        for (int i = 0; i < veh->numberOfTrip; i++) {
            idleTime += max(0, veh->idealDeptTime[i] - veh->predDeptTime[i]);
        }
    }

    int totalWaitingTime = 0;
    for (auto& patient : returnedPatients) {
        totalWaitingTime += patient->getOnVehicleTime - patient->addedTime;
    }
    avgWaitingTime = (returnedPatients.size()) ? totalWaitingTime / returnedPatients.size() : 0;

    for (auto& line : waitingLine) {
        missedPatients = line.size();
    }

    /* calculate the total return time of missed patients (60 km/hr) */
    int totalReturnTime = 0;
    for (auto& line : waitingLine) {
        for (auto& patient : line) {
            auto [x, y] = patient->destination;
            totalReturnTime += abs(x) + abs(y);
        }
    }

    totalPerformance = 2000 - (idleTime + 1.5 * avgWaitingTime + 0.1 * totalReturnTime);

    printf("> Performances\n");
    printf("  Idle Time: %02d:%02d\n", idleTime / 60, idleTime % 60);
    printf("  Average Waiting Time: %02d:%02d\n", avgWaitingTime / 60, avgWaitingTime % 60);
    printf("  Missed Patients: %d people\n", missedPatients);
    printf("  Total Return Time of Missed Patients: %02d:%02d\n", totalReturnTime / 60, totalReturnTime % 60);
    printf("  Total Performance: %d\n", totalPerformance);
}