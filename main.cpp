#include "system.hpp"

using namespace std;

int main() {
    for (int i = 0; i < 10; i++) {
        System* system = new System();

        system->addPatient(1, "PC", "Dr. A", {20, 30}, 856);
        system->addPatient(2, "MED", "Dr. B", {10, 40}, 1024);

        system->addVehicle(1);
        system->addVehicle(2);

        system->generateSche();
        system->displayPlan();

        delete system;
    }

    return 0;
}
