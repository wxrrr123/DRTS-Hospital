#include "subsystem.hpp"

#include <random>

void Subsystem::addPatient(Patient* p) { patients.push(p); }

void Subsystem::addVehicle(int id) {
    Vehicle* v = new Vehicle(id);
    vehicles.push_back(v);
}

void Subsystem::setSchedule(vector<int> schedule) { this->schedule = schedule; }

void Subsystem::oneDaySimulation() {
    clock = 600;
    int t = 1;

    queue<Vehicle*> availVeh, onServVeh;
    for (auto& v : vehicles) availVeh.push(v);

    while (clock <= 1080) {
        while (!patients.empty()) {
            Patient* p = patients.top();
            if (p->addedTime <= clock) {
                waitingLine.push_back(p);
                patients.pop();
            } else
                break;
        }

        while (t < schedule.size()) {
            if (clock >= schedule[t - 1] && !availVeh.empty()) {
                Vehicle* v = availVeh.front();
                v->idealDeptTime.push_back(schedule[t - 1]);
                v->realDeptTime.push_back(clock);

                printf("\nClock: %02d:%02d\n", clock / 60, clock % 60);

                /* Add patients into vehicles */
                while (!waitingLine.empty() && v->patients.size() < v->capacity) {
                    Patient* firstPatient = waitingLine.front();
                    firstPatient->getOnVehicleTime = clock;

                    v->patients.push_back(firstPatient);
                    returnedPatients.push_back(firstPatient);

                    waitingLine.pop_front();

                    printf("Patient %03d gets on. ", firstPatient->id);
                    printf("Dest: (%3d, %3d), AddedTime: %02d:%02d, WaitingTime: %02d:%02d\n", firstPatient->dest.first,
                           firstPatient->dest.second, firstPatient->addedTime / 60, firstPatient->addedTime % 60,
                           (clock - firstPatient->addedTime) / 60, (clock - firstPatient->addedTime) % 60);
                }

                /* Update vehicles */
                availVeh.pop();
                onServVeh.push(v);
                v->predictReturnTime();
                v->patients.clear();
                t++;

                while (!onServVeh.empty()) {
                    auto& firstVeh = onServVeh.front();
                    if (clock >= firstVeh->returnTime.back() && firstVeh->returnTime.size() < firstVeh->tripNum) {
                        availVeh.push(firstVeh);
                        onServVeh.pop();
                    } else
                        break;
                }

            } else
                break;
        }

        clock++;
    }
}

void Subsystem::oneDayPerformance() {}

// void Subsystem::displayPlan() {
//     cout << "\n>>>>> SYSTEM RESULT <<<<<" << endl;
//     cout << "> Vehicle Assignment" << endl;
//     for (auto& veh : vehicles) {
//         cout << "  ID " << veh->id << ", Region " << veh->region << " => ";
//         for (auto& deptTime : veh->idealDeptTime) {
//             printf("%02d:%02d ", deptTime / 60, deptTime % 60);
//         }
//         cout << endl;
//     }
//     cout << endl;

//     cout << "> Departure Time Schedule" << endl;
//     vector<multiset<int>> depart(numOfRegion);
//     for (auto& veh : vehicles) {
//         for (auto& deptTime : veh->idealDeptTime) {
//             depart[veh->region - 1].insert(deptTime);
//         }
//     }
//     for (int i = 0; i < numOfRegion; i++) {
//         cout << "  Region " << i + 1 << " => ";
//         for (auto& deptTime : depart[i]) {
//             printf("%02d:%02d ", deptTime / 60, deptTime % 60);
//         }
//         cout << endl;
//     }
//     cout << endl;
// }

// void Subsystem::oneDayPerformance() {
//     for (auto& veh : vehicles) {
//         for (int i = 0; i < veh->tripNum; i++) {
//             idleTime += max(0, veh->idealDeptTime[i] - veh->predDeptTime[i]);
//         }
//     }

//     int totalWaitingTime = 0;
//     for (auto& p : returnedPatients) {
//         totalWaitingTime += p->getOnVehicleTime - p->addedTime;
//     }
//     waitingTime = (returnedPatients.size()) ? totalWaitingTime / returnedPatients.size() : 0;

//     missedPatients = sampleSize - returnedPatients.size();

//     /* calculate the total return time of missed patients (60 km/hr) */
//     int totalReturnTime = 0;
//     for (auto& line : waitingLine) {
//         for (auto& p : line) {
//             auto [x, y] = p->dest;
//             totalReturnTime += round(sqrt(x * x + y * y) / 1);
//         }
//     }

//     performance = 2000 - (idleTime + 1.5 * waitingTime + 0.1 * totalReturnTime);

//     printf("> Performances\n");
//     printf("  Idle Time: %02d:%02d\n", idleTime / 60, idleTime % 60);
//     printf("  Average Waiting Time: %02d:%02d\n", waitingTime / 60, waitingTime % 60);
//     printf("  Missed Patients: %d people\n", missedPatients);
//     printf("  Total Return Time of Missed Patients: %02d:%02d\n", totalReturnTime / 60, totalReturnTime % 60);
//     printf("  Total Performance: %d\n", performance);
// }