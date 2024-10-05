#include "subsystem.hpp"

#include <random>

void Subsystem::addPatient(Patient* p) { patients.push(p); }

void Subsystem::addVehicle(Vehicle* v) { vehicles.push_back(v); }

void Subsystem::setSchedule(vector<int> schedule) { this->schedule = schedule; }

void Subsystem::oneDaySimulation() {
    clock = 600;
    int t = 1;

    queue<Vehicle*> availVeh;
    list<Vehicle*> onServVeh;

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

        while (t <= schedule.size() && clock >= schedule[t - 1]) {
            if (!availVeh.empty()) {
                Vehicle* v = availVeh.front();
                v->idealDeptTime.push_back(schedule[t - 1]);
                v->realDeptTime.push_back(clock);

                printf("\nVehicle %d at clock: %02d:%02d\n", v->id, clock / 60, clock % 60);

                /* Add patients into vehicles */
                while (!waitingLine.empty() && v->patients.size() <= v->capacity) {
                    Patient* firstPatient = waitingLine.front();
                    firstPatient->getOnVehicleTime = clock;

                    v->patients.push_back(firstPatient);
                    firstPatient->returned = true;

                    waitingLine.pop_front();

                    printf("Patient %03d gets on. ", firstPatient->id);
                    printf("Dest: (%3d, %3d), AddedTime: %02d:%02d, WaitingTime: %02d:%02d\n", firstPatient->dest.first,
                           firstPatient->dest.second, firstPatient->addedTime / 60, firstPatient->addedTime % 60,
                           (clock - firstPatient->addedTime) / 60, (clock - firstPatient->addedTime) % 60);
                }

                /* Update vehicles */
                availVeh.pop();
                onServVeh.push_back(v);
                v->predictReturnTime();
                v->patients.clear();
                t++;
            } else
                break;
        }

        clock++;

        auto it = onServVeh.begin();
        while (it != onServVeh.end()) {
            if (clock >= (*it)->returnTime.back()) {
                if ((*it)->returnTime.size() < (*it)->tripNum) availVeh.push(*it);
                it = onServVeh.erase(it);
            } else
                it++;
        }
    }
}