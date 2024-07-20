#include "system.hpp"

void System::addPatient(int id, string dept, string doc, pair<int, int> dest, int arrival) {
    Patient* patient = new Patient(id, dept, doc, dest, arrival);
    patient->predictAddedTime();
    patients.push(patient);
}

void System::addVehicle(int id) {
    Vehicle* vehicle = new Vehicle(id);
    vehicles.push_back(vehicle);
}

void System::generateSchedule() {
    set<int> comb;
    for (auto& veh : vehicles) {
        while (comb.size() < veh->numberOfTrip) {
            comb.insert((rand() % 13 + 10) * 60);  // 10 ~ 22 o'clock
        }
        veh->idealDeptTime = vector<int>(comb.begin(), comb.end());
        veh->predDeptTime.push_back(veh->idealDeptTime.front());
        comb.clear();
    }

    planReturnTrips();
}

void System::planReturnTrips() {
    while (clock <= 1320) {
        while (!patients.empty()) {
            Patient* patient = patients.top();
            if (patient->addedTime <= clock) {
                waitingLine.push_back(patient);
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
                /* Add patients into vehicles */
                while (!waitingLine.empty() && veh->patients.size() < veh->capacity) {
                    Patient* firstPatient = waitingLine.front();
                    firstPatient->getOnVehicleTime = clock;

                    veh->patients.push_back(firstPatient);
                    returnedPatients.push_back(firstPatient);

                    waitingLine.pop_front();
                }

                veh->predictRoutingTime();
                veh->patients.clear();
            }
        }

        clock++;
    }
}

void System::displayPlan() {
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

    missedPatients = waitingLine.size();

    /* calculate the total return time of missed patients (60 km/hr) */
    int totalReturnTime = 0;
    for (auto& miss : waitingLine) {
        auto [x, y] = miss->destination;
        totalReturnTime += abs(x) + abs(y);
    }

    totalPerformance = idleTime + 1.5 * avgWaitingTime + 0.1 * totalReturnTime;

    cout << "> Performances" << endl;
    cout << "  Idle Time: " << idleTime << endl;
    cout << "  Average Waiting Time: " << avgWaitingTime << endl;
    cout << "  Missed Patients: " << missedPatients << endl;
    cout << "  Total Return Time: " << totalReturnTime << endl;
    cout << "  Total Performance: " << totalPerformance << endl;
}
