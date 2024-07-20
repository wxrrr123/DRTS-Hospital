#include "patient.hpp"

Patient::Patient(int id, string dept, string doc, pair<int, int> dest, int arrival)
    : id(id),
      department(dept),
      doctor(doc),
      destination(dest),
      arrivalTime(arrival),
      completionTime(0),
      addedTime(0),
      getOnVehicleTime(0) {}

void Patient::predictAddedTime() { addedTime = arrivalTime + 60; }
