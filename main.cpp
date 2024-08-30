#include "system.hpp"

using namespace std;

int main() {
    System* system = new System();

    system->readPatientData("DLtimestamp.csv");

    system->waitingLine.resize(system->numOfRegion + 1);

    for (int j = 1; j <= 5; j++) system->addVehicle(j, (j - 1) % 4 + 1);

    system->generateSchedule();
    system->planReturnTrips();
    system->displayPlan();
    system->calculatePerformance();

    delete system;

    return 0;
}
