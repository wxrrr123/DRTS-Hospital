#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "patient.hpp"
#include "vehicle.hpp"

using namespace std;

struct patientCmp {
    bool operator()(Patient* a, Patient* b) {
        return a->addedTime > b->addedTime;
    }
};

class System {
  public:
    priority_queue<Patient*, vector<Patient*>, patientCmp> patients;
    vector<Vehicle*> vehicles;
    map<int, string> departments;
    deque<Patient*> waitingLine;
    int clock = 600;

    System(){};
    void addPatient(int id, string dept, string doc, pair<int, int> dest,
                    int arrival);
    void addVehicle(int id);
    void planReturnTrips();
    void displayPlan();
    void generateSche();
};

#endif  // SYSTEM_HPP
