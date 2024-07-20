#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <bits/stdc++.h>

using namespace std;

class Patient {
  public:
    int id;
    string department;
    string doctor;
    pair<int, int> destination;
    int arrivalTime;
    int completionTime;
    int addedTime;
    int getOnVehicleTime;

    Patient(){};
    Patient(int id, string dept, string doc, pair<int, int> dest, int arrival);
    void predictAddedTime();
};

#endif  // PATIENT_HPP
