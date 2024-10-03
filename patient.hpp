#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <bits/stdc++.h>

#include <random>

using namespace std;

class Patient {
  public:
    int id;
    pair<int, int> dest;
    int addedTime;
    int getOnVehicleTime;
    int region;

    Patient() {};
    Patient(int id, pair<int, int> dest, int added);
    void setRegion();
};

#endif  // PATIENT_HPP
