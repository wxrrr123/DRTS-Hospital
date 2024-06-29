#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <ctime>
#include <vector>

#include "patient.hpp"

using namespace std;

class Vehicle {
  public:
    int id;
    int capacity;
    vector<Patient*> patients;
    time_t departureTime;

    Vehicle(int id, int cap);
    bool addPatient(Patient* patient);
    void setDepartureTime(time_t time);
};

#endif  // VEHICLE_HPP
