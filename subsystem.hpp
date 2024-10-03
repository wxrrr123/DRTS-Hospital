#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP

#include "patient.hpp"
#include "vehicle.hpp"

using namespace std;

struct patientCmp {
    bool operator()(Patient* a, Patient* b) { return a->addedTime > b->addedTime; }
};

class Subsystem {
  public:
    int id;
    int clock;
    int startTime;
    int endTime;
    vector<int> schedule;
    priority_queue<Patient*, vector<Patient*>, patientCmp> patients;
    vector<Vehicle*> vehicles;
    deque<Patient*> waitingLine;
    vector<Patient*> returnedPatients;

    /* performances */
    int idleTime = 0;
    int waitingTime = 0;
    int missedPatients = 0;
    int performance = 0;

    /* initiate */
    Subsystem(int id, int sT, int eT) : id(id), startTime(sT), endTime(eT) {}
    void addPatient(Patient* patient);
    void addVehicle(int id);
    void setSchedule(vector<int> schedule);

    void oneDaySimulation();
    void oneDayPerformance();
};

#endif  // SUBSYSTEM_HPP
