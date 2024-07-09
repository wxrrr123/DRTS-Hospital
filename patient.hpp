#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <ctime>
#include <string>

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
    bool isOnVehicle;

    Patient(){};
    Patient(int id, string dept, string doc, pair<int, int> dest, int arrival);
    void predictAddedTime(Patient &patient);
};

#endif  // PATIENT_HPP
