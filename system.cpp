#include "system.hpp"

#include <ctime>
#include <iostream>

void System::addPatient(int id, string dept, string doc, string dest,
                        time_t arrival) {
    patients.emplace_back(id, dept, doc, dest, arrival);
}

void System::addVehicle(int id, int cap) { vehicles.emplace_back(id, cap); }

void System::planReturnTrips() {
    time_t currentTime = time(0);
    for (auto& vehicle : vehicles) {
        for (auto& patient : patients) {
            if (patient.completionTime <= currentTime) {
                if (vehicle.addPatient(&patient)) {
                    patient.addedTime = currentTime - patient.arrivalTime;
                }
            }
        }
        vehicle.setDepartureTime(currentTime);
    }
}

void System::displayPlan() {
    for (auto& vehicle : vehicles) {
        cout << "Vehicle " << vehicle.id << " departing at "
             << vehicle.departureTime / 86400 << " with patients: ";
        for (auto& patient : vehicle.patients) {
            cout << patient->id << " ";
        }
        cout << endl;
    }
}
