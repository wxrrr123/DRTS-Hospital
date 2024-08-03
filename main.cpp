#include "system.hpp"

using namespace std;

int main() {
    for (int i = 0; i < 1; i++) {
        System* system = new System();

        vector<pair<int, int>> coords;
        for (int j = 0; j < 30; j++) {
            coords.push_back({rand() % 41 - 20, rand() % 41 - 20});
        }

        for (int j = 1; j <= 100; j++) {
            pair<int, int> coord = coords[rand() % 30];
            int arr = rand() % 841 + 480;  // [480, 1320]

            system->addPatient(j, "MED", "Dr. A", coord, arr);
        }

        for (int j = 1; j <= 4; j++) system->addVehicle(j);

        system->generateSchedule();
        system->displayPlan();
        system->calculatePerformance();

        delete system;
    }

    return 0;
}
