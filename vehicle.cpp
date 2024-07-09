#include "vehicle.hpp"

bool Vehicle::addPatient(Patient* patient) {
    if (patients.size() < capacity) {
        patients.push_back(patient);
        return true;
    }
    return false;
}

void Vehicle::predictRoutingTime() {
    int lastDept = predDeptTime.size() - 1;
    int lastDeptTime = max(idealDeptTime[lastDept], predDeptTime[lastDept]);

    /* To modify */
    int nextDeptTime = lastDeptTime + patients.size() * 10;
    predDeptTime.push_back(nextDeptTime);
}