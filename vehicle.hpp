#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "patient.hpp"

using namespace std;

class Vehicle {
  public:
    int id;
    const int capacity = 15;
    const int numberOfTrip = 3;
    vector<Patient*> patients;
    vector<int> idealDeptTime;
    vector<int> predDeptTime;
    vector<pair<int, int>> route;

    Vehicle(int id) : id(id) {};
    void predictRoutingTime();
};

#endif  // VEHICLE_HPP
