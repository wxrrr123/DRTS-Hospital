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
    int missedPatients = 0;
    int performance = 0;

    /* initiate */
    System() {};
    vector<Patient*> readPatientData(string filename);
    void addPatient(Patient* p);
    void addVehicle(Vehicle* v);
    void addSubsystem(Subsystem* s);

    void oneDayPerformance();
    void displayPlan();
};

#endif  // SYSTEM_HPP
