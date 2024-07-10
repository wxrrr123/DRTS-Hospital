#include "system.hpp"

void System::addPatient(int id, string dept, string doc, pair<int, int> dest,
                        int arrival) {
    Patient* patient = new Patient(id, dept, doc, dest, arrival);
    patients.push(patient);
}

void System::addVehicle(int id) {
    Vehicle* vehicle = new Vehicle(id);
    vehicles.push_back(vehicle);
}

void System::generateSche() {
    set<int> comb;
    for (auto& vehicle : vehicles) {
        while (comb.size() < vehicle->numberOfTrip) {
            srand(time(0) + vehicle->id);
            comb.insert((rand() % 13 + 10) * 60);  // 10 ~ 22 o'clock
        }
        vehicle->idealDeptTime = vector<int>(comb.begin(), comb.end());
        vehicle->predDeptTime.push_back(vehicle->idealDeptTime.front());
        comb.clear();
    }

    planReturnTrips();
}

void System::planReturnTrips() {
    while (clock <= 1320) {
        while (!patients.empty()) {
            Patient* patient = new Patient();
            patient = patients.top();
            if (patient->addedTime <= clock) {
                waitingLine.push_back(patient);
                patients.pop();
            } else {
                break;
            }
        }

        for (auto& veh : vehicles) {
            vector<int> ideal = veh->idealDeptTime;
            vector<int> pred = veh->predDeptTime;
            int nth_dept = veh->predDeptTime.size() - 1;  // nth departure time
            int deptTime = max(ideal[nth_dept], pred[nth_dept]);

            if (clock == deptTime) {
                /* Add patients into vehicles */
                while (!waitingLine.empty()) {
                    if (veh->patients.size() < veh->capacity) break;

                    Patient* firstPatient = new Patient();
                    firstPatient = waitingLine.front();
                    if (firstPatient->addedTime > deptTime) break;

                    veh->patients.push_back(firstPatient);
                    waitingLine.pop_front();
                }

                veh->predictRoutingTime();
            }
        }

        clock++;
    }
}

void System::displayPlan() {
    for (auto& veh : vehicles) {
        cout << veh->id << ": ";
        int size = veh->predDeptTime.size();
        for (int i = 0; i < size - 1; i++) {
            int deptTime = max(veh->idealDeptTime[i], veh->predDeptTime[i]);
            cout << veh->idealDeptTime[i] << " ";
        }
        cout << "\n";
    }
}
