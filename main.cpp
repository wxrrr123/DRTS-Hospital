#include "system.hpp"

using namespace std;

int main() {
    for (int i = 0; i < 3; i++) {
        System* system = new System();

        for (int j = 1; j <= 100; j++) {
            pair<int, int> coord = {rand() % 41 - 20, rand() % 41 - 20};
            int arr = rand() % 841 + 480;  // [480, 1320]

            system->addPatient(j, "MED", "Dr. A", coord, arr);
        }

        for (int j = 1; j <= 3; j++) system->addVehicle(j);

        system->generateSchedule();
        system->displayPlan();
        system->calculatePerformance();
        cout << endl << endl;

        delete system;
    }

    return 0;
}
