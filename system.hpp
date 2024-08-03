#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "patient.hpp"
#include "vehicle.hpp"

#define regionNum 4

using namespace std;

struct patientCmp {
    bool operator()(Patient* a, Patient* b) { return a->addedTime > b->addedTime; }
};

class System {
  public:
    priority_queue<Patient*, vector<Patient*>, patientCmp> patients;
    vector<Vehicle*> vehicles;
    map<int, string> departments;
    deque<Patient*> waitingLine[regionNum + 1];
    vector<Patient*> returnedPatients;
    int clock = 600;

    /* performances */
    int idleTime = 0;
    int avgWaitingTime = 0;
    int missedPatients = 0;
    int totalPerformance = 0;

    System() {};
    void addPatient(int id, string dept, string doc, pair<int, int> dest, int arrival);
    void addVehicle(int id);
    void planReturnTrips();
    void displayPlan();
    void generateSchedule();
    void calculatePerformance();
};

#endif  // SYSTEM_HPP
