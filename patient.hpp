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
    string destination;
    time_t arrivalTime;
    time_t completionTime;
    time_t waitingTime;

    Patient(int id, string dept, string doc, string dest, time_t arrival);
};

#endif  // PATIENT_HPP
