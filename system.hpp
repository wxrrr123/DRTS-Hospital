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
    int avgIdleTime = 0;
    int avgWaitingTime = 0;
    int totalRetTime = 0;
    float performance = 0;

    /* initiate */
    System() {};
    vector<Patient*> readPatientData(string file);
    void addPatient(Patient* p);
    void addVehicle(Vehicle* v);
    void addSubsystem(Subsystem* s);

    float oneDayPerformance();
    void displayPlan();
};

#endif  // SYSTEM_HPP
