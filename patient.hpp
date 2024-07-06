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
    time_t arrivalTime;
    time_t completionTime;
    time_t addedTime;

    Patient(int id, string dept, string doc, pair<int, int> dest,
            time_t arrival);
};

#endif  // PATIENT_HPP
