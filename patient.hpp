#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <bits/stdc++.h>

#include <random>

using namespace std;

class Patient {
  public:
    int id;
    string department;
    pair<int, int> destination;
    int addedTime;
    int getOnVehicleTime;
    int region;

    Patient() {};
    Patient(int id, string dept, pair<int, int> dest, int added);
    void setRegion();
};

#endif  // PATIENT_HPP
