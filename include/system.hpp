#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "patient.hpp"
#include "subsystem.hpp"
#include "vehicle.hpp"

using namespace std;

class System {
  public:
    vector<Patient*> patients;
    vector<Vehicle*> vehicles;
    vector<Subsystem*> subsystems;

    vector<int> assign;
    vector<vector<int>> schedule;

    /* performances */
    float avgIdleTime = 0;
    float avgWaitTime = 0;
    int totalRetTime = 0;
    float performance = 0;

    /* initiate */
    System() {};
    void addPatient(Patient* p);
    void addVehicle(Vehicle* v);
    void addSubsystem(Subsystem* s);

    float oneDayPerformance();
    void displayPlan();
};

#endif  // SYSTEM_HPP
