#include "system.hpp"

void System::addPatient(int id, string dept, string doc, pair<int, int> dest, int arrival) {
    Patient* patient = new Patient(id, dept, doc, dest, arrival);
    patient->predictAddedTime();
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
    schedule.push_back({660, 960, 1140});
    schedule.push_back({840, 1080, 1260});
    schedule.push_back({900, 1140, 1200});
    schedule.push_back({720, 1140, 1020});

    for (int i = 0; i < vehicles.size(); i++) {
        vehicles[i]->idealDeptTime = schedule[i];
        vehicles[i]->predDeptTime.push_back(vehicles[i]->idealDeptTime.front());
    }

    planReturnTrips();
}

void System::planReturnTrips() {
    while (clock <= 1320) {
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
                printf("Vehicle ID: %d\n", veh->id);

                /* Add patients into vehicles with different regions */
                while (!waitingLine[veh->id].empty() && veh->patients.size() < veh->capacity) {
                    Patient* firstPatient = waitingLine[veh->id].front();
                    firstPatient->getOnVehicleTime = clock;

                    veh->patients.push_back(firstPatient);
                    returnedPatients.push_back(firstPatient);

                    waitingLine[veh->id].pop_front();

                    printf("Patient %02d get on.\n", firstPatient->id);
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
            cout << deptTime << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void System::calculatePerformance() {
    for (auto& veh : vehicles) {
        for (int i = 0; i < veh->predDeptTime.size(); i++) {
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
    printf("  Idle Time: %dhr%dmin\n", idleTime / 60, idleTime % 60);
    printf("  Average Waiting Time: %dhr%dmin\n", avgWaitingTime / 60, avgWaitingTime % 60);
    printf("  Missed Patients: %d people\n", missedPatients);
    printf("  Total Return Time of Missed Patients: %0dhr%dmin\n", totalReturnTime / 60, totalReturnTime % 60);
    printf("  Total Performance: %d\n", totalPerformance);
}