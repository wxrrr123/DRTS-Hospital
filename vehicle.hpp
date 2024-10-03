#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "patient.hpp"

using namespace std;

class Vehicle {
  public:
    int id;
    int region;
    int capacity;
    int tripNum;
    vector<Patient*> patients;
    vector<int> idealDeptTime;
    vector<int> realDeptTime;
    vector<int> returnTime;

    Vehicle(int id) : id(id) {};
    Vehicle(int id, int capacity, int tripNum) : id(id), capacity(capacity), tripNum(tripNum) {};
    void predictReturnTime();
};

#endif  // VEHICLE_HPP
