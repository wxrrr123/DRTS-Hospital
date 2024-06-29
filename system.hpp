#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <map>
#include <string>
#include <vector>

#include "patient.hpp"
#include "vehicle.hpp"

using namespace std;

class System {
  private:
    vector<Patient> patients;
    vector<Vehicle> vehicles;
    map<int, string> departments;

  public:
    void addPatient(int id, string dept, string doc, string dest,
                    time_t arrival);
    void addVehicle(int id, int cap);
    void planReturnTrips();
    void displayPlan();
};

#endif  // SYSTEM_HPP
