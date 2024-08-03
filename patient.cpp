#include "patient.hpp"

Patient::Patient(int id, string dept, string doc, pair<int, int> dest, int arrival)
    : id(id),
      department(dept),
      doctor(doc),
      destination(dest),
      arrivalTime(arrival),
      completionTime(0),
      addedTime(0),
      getOnVehicleTime(0),
      region(0) {}

void Patient::predictAddedTime() {
    /* time for on call */
    int onCallTime = 35;  // to modify

    /* time for preparing drug */
    double shape = 7.400846, scale = 1.0 / 0.441871;  // 9-10 o'clock
    random_device rd;
    mt19937 generator(rd());
    gamma_distribution<double> gamma_dist(shape, scale);
    int prepDrugTime = (int)gamma_dist(generator);

    addedTime = arrivalTime + onCallTime + prepDrugTime;
}

void Patient::setRegion() {
    auto [x, y] = this->destination;
    region = (x > 0) ? (y > 0 ? 1 : 4) : (y > 0 ? 2 : 3);
}