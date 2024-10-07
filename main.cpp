#include "subsystem.hpp"
#include "system.hpp"

// temp input dataset
#define dayNum 3
#define regionNum 4
#define startTime 600
#define endTime 1080
#define capacity 15
#define tripNum 3

using namespace std;

void totalPerformance(float totalKPI);

void sysDesignEval(vector<int>& assign, vector<vector<int>>& schedule);

int main() {
    /*
    TODO: input dataset
    */

    // temp system design
    vector<int> assign = {2, 1, 2, 2};
    vector<vector<int>> schedule = {{600, 720, 780, 900, 960, 1020},
                                    {780, 840, 1020},
                                    {600, 660, 780, 900, 1020, 1080},
                                    {660, 720, 720, 960, 960, 1020}};

    sysDesignEval(assign, schedule);

    return 0;
}

void totalPerformance(float totalKPI) { printf("\n>>>>>>> SYSTEM DESIGN KPI = %.1f <<<<<<<\n", totalKPI / dayNum); }

void sysDesignEval(vector<int>& assign, vector<vector<int>>& schedule) {
    float totalKPI = 0;

    for (int d = 0; d < dayNum; d++) {
        System* S = new System();

        /* initiate system */
        S->assign = assign;
        S->schedule = schedule;
        S->patients = S->readPatientData("DLtimestamp.csv");

        int vehNum = accumulate(assign.begin(), assign.end(), 0);
        for (int i = 0; i < vehNum; i++) {
            Vehicle* v = new Vehicle(i + 1, 15, 3);
            S->addVehicle(v);
        }

        /* initiate subsystem */
        int vehId = 0;
        for (int i = 0; i < regionNum; i++) {
            Subsystem* s = new Subsystem(i + 1, startTime, endTime);
            S->addSubsystem(s);
            s->setSchedule(schedule[i]);

            for (auto& p : S->patients) {
                if (p->region == s->id) s->addPatient(p);
            }

            for (int j = 0; j < assign[s->id - 1]; j++) s->addVehicle(S->vehicles[vehId++]);
        }

        for (auto s : S->subsystems) {
            printf("\n>>> SUBSYSTEM %d <<<", s->id);
            s->oneDaySimulation();
        }

        S->displayPlan();
        totalKPI += S->oneDayPerformance();

        /* destruction */
        for (auto& p : S->patients) delete p;
        S->patients.clear();

        for (auto& v : S->vehicles) delete v;
        S->vehicles.clear();

        for (auto& s : S->subsystems) delete s;
        S->subsystems.clear();

        delete S;
    }

    totalPerformance(totalKPI);
};
