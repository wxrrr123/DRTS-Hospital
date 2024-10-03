#include "patient.hpp"

Patient::Patient(int id, pair<int, int> dest, int added)
    : id(id), dest(dest), addedTime(added), getOnVehicleTime(0), region(0) {}

void Patient::setRegion() {
    auto [x, y] = this->dest;
    region = (x > 0) ? (y > 0 ? 1 : 4) : (y > 0 ? 2 : 3);
}