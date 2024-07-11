#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "patient.hpp"
#include <ctime>
#include <vector>

using namespace std;

class Vehicle {
  public:
    int id;
    const int capacity = 15;
    vector<Patient*> patients;
    vector<int> idealDeptTime;
    vector<int> predDeptTime;
    int numberOfTrip = 3;

    Vehicle(int id) : id(id){};
    bool addPatient(Patient* patient);
    void predictRoutingTime();
};

#endif  // VEHICLE_HPP
