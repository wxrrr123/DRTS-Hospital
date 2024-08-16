#include "system.hpp"

using namespace std;

int main() {
    System* system = new System();

    system->readPatientData("DLtimestamp.csv");

    for (int j = 1; j <= 5; j++) system->addVehicle(j, j % 4 + 1);

    system->generateSchedule();
    system->displayPlan();
    system->calculatePerformance();

    delete system;

    return 0;
}
