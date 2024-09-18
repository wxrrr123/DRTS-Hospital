#include "system.hpp"

#include <random>

void System::addPatient(int id, string dept, pair<int, int> dest, int added) {
    Patient* patient = new Patient(id, dept, dest, added);
    patient->setRegion();
    patients.push(patient);
}

void System::addPatient(Patient* patient) {
    patient->setRegion();
    patients.push(patient);
}

void System::addVehicle(int id) {
    Vehicle* vehicle = new Vehicle(id);
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
}

void System::planReturnTrips() {
    clock = 600;

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
    cout << "> Vehicle Assignment" << endl;
    for (auto& veh : vehicles) {
        cout << "  ID " << veh->id << ", Region " << veh->region << " => ";
        for (auto& deptTime : veh->idealDeptTime) {
            printf("%02d:%02d ", deptTime / 60, deptTime % 60);
        }
        cout << endl;
    }
    cout << endl;

    cout << "> Departure Time Schedule" << endl;
    vector<multiset<int>> depart(numOfRegion);
    for (auto& veh : vehicles) {
        for (auto& deptTime : veh->idealDeptTime) {
            depart[veh->region - 1].insert(deptTime);
        }
    }
    for (int i = 0; i < numOfRegion; i++) {
        cout << "  Region " << i + 1 << " => ";
        for (auto& deptTime : depart[i]) {
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

    missedPatients = sampleSize - returnedPatients.size();

    /* calculate the total return time of missed patients (60 km/hr) */
    int totalReturnTime = 0;
    for (auto& line : waitingLine) {
        for (auto& patient : line) {
            auto [x, y] = patient->destination;
            totalReturnTime += round(sqrt(x * x + y * y) / 1);
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