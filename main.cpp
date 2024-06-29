#include "system.hpp"

using namespace std;

int main() {
    System system;

    system.addPatient(1, "Cardiology", "Dr. A", "Destination A", time(0));
    system.addPatient(2, "Neurology", "Dr. B", "Destination B", time(0) + 3600);

    system.addVehicle(1, 2);
    system.addVehicle(2, 3);

    system.planReturnTrips();

    system.displayPlan();

    return 0;
}
