#include "patient.hpp"

Patient::Patient(int id, string dept, string doc, pair<int, int> dest,
                 time_t arrival)
    : id(id),
      department(dept),
      doctor(doc),
      destination(dest),
      arrivalTime(arrival),
      completionTime(0),
      addedTime(0) {}
