#include "patient.hpp"

Patient::Patient(int id, string dept, string doc, string dest, time_t arrival)
    : id(id),
      department(dept),
      doctor(doc),
      destination(dest),
      arrivalTime(arrival),
      completionTime(0),
      waitingTime(0) {}
