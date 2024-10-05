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

    /* initiate */
    Subsystem(int id, int sT, int eT) : id(id), startTime(sT), endTime(eT) {}
    void addPatient(Patient* p);
    void addVehicle(Vehicle* v);
    void setSchedule(vector<int> schedule);

    void oneDaySimulation();
};

#endif  // SUBSYSTEM_HPP
