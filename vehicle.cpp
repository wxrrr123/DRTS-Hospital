#include "vehicle.hpp"

Vehicle::Vehicle(int id, int cap) : id(id), capacity(cap), departureTime(0) {}

bool Vehicle::addPatient(Patient* patient) {
    if (patients.size() < capacity) {
        patients.push_back(patient);
        return true;
    }
    return false;
}

void Vehicle::setDepartureTime(time_t time) { departureTime = time; }
