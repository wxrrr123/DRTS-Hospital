#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <bits/stdc++.h>
#include <ctime>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

class Patient {
  public:
    int id;
    string department;
    string doctor;
    pair<int, int> destination;
    int arrivalTime;
    int completionTime;
    int addedTime;
    bool isOnVehicle;

    // hamiltonian cycle
    uint32_t minRoutingTime = UINT32_MAX;
    vector<int> bestHamiltonianCycle;
    vector<int> hamiltonianCycle;
    vector<vector<int>> graph;
    vector<bool> visited;

    Patient(){};
    Patient(int id, string dept, string doc, pair<int, int> dest, int arrival);
    void predictAddedTime(Patient &patient);
    double distanceTo(Patient* patient);
    void backtrack(int n);
};

#endif  // PATIENT_HPP
