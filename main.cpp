#include "subsystem.hpp"
#include "system.hpp"

// temp input dataset
#define dayNum 100
#define regionNum 5
#define startTime 600
#define endTime 1080
#define vehNum 10
#define capacity 15
#define tripNum 3

using namespace std;

int main() {
    /*
    TODO: input dataset
    */

    // temp system design
    vector<int> assign = {2, 2, 2, 2, 2};
    vector<vector<int>> schedule;
    schedule.push_back({600, 720, 780, 900, 960, 1020});
    schedule.push_back({600, 780, 840, 900, 960, 1020});
    schedule.push_back({600, 660, 780, 900, 1020, 1080});
    schedule.push_back({660, 720, 780, 900, 1020, 1080});
    schedule.push_back({660, 780, 840, 900, 960, 1080});

    for (int d = 0; d < dayNum; d++) {
        System* S = new System();

        /* initiate system */
        S->assign = assign;
        S->schedule = schedule;
        S->patients = S->readPatientData("DLtimestamp.csv");

        for (int i = 0; i < vehNum; i++) {
            Vehicle* v = new Vehicle(i + 1, capacity, tripNum);
            S->addVehicle(v);
        }

        /* initiate subsystem */
        int vehId = 1;
        for (int i = 0; i < regionNum; i++) {
            Subsystem* s = new Subsystem(i + 1, startTime, endTime);
            S->addSubsystem(s);
            s->setSchedule(schedule[i]);

            for (auto& p : S->patients) {
                if (p->region == s->id) s->addPatient(p);
            }

            for (int j = 0; j < assign[s->id - 1]; j++) s->addVehicle(vehId++);
        }

        for (auto s : S->subsystems) s->oneDaySimulation();

        S->oneDayPerformance();

        for (auto& p : S->patients) delete p;
        S->patients.clear();
    }

    totalPerformance();

    return 0;
}

void totalPerformance() {}
