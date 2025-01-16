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
    float missRate = 0;
    float performance = 0;

    /* constraints */
    int maxDelay = 30;
    // int indivWaitingTime = 120;
    float maxMissRate = 0.25;

    /* initiate */
    System() {};
    vector<Patient*> readPatientData(string file);
    void addPatient(Patient* p);
    void addVehicle(Vehicle* v);
    void addSubsystem(Subsystem* s);

    float oneDayPerformance();
    bool validateConstraint();
    void displayPlan();
};

#endif  // SYSTEM_HPP
