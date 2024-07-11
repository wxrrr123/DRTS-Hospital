#include "vehicle.hpp"
#include <iostream>
#include <climits>

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
    cout << "lastDeptTime: " << lastDeptTime << endl;

    /* To modify */
    int nextDeptTime = lastDeptTime;
    if (patients.size() > 0) {
        Patient* hospital = new Patient();
        hospital->destination = make_pair(0, 0);
        vector<Patient*> patientsTmp;
        for (int i = 0; i < patients.size(); i++) {
            patientsTmp.push_back(patients[i]);
        }
        patientsTmp.push_back(hospital);
        vector<vector<int>> graph(patientsTmp.size(), vector<int>(patientsTmp.size(), INT32_MAX));
        for (int i = 0; i < patientsTmp.size(); i++) {
            Patient* patient = patientsTmp[i];
            for (int j = 0; j < patientsTmp.size(); j++) {
                if (i == j) continue;
                Patient* nextPatient = patientsTmp[j];
                graph[i][j] = patient->distanceTo(nextPatient);
            }
        }
        // find hamiltonian cycle
        int hospitalIdx = patientsTmp.size() - 1;
        hospital = patientsTmp[hospitalIdx];
        hospital->minRoutingTime = INT32_MAX;
        hospital->visited = vector<bool>(patientsTmp.size(), false);
        hospital->graph = graph;
        hospital->visited[hospitalIdx] = true;
        hospital->hamiltonianCycle.push_back(hospitalIdx);
        hospital->backtrack(patientsTmp.size());
        nextDeptTime += hospital->minRoutingTime * 2;
    }

    //int nextDeptTime = lastDeptTime + patients.size() * 10;
    cout << "Vehicle " << id << " is predicted to be back at " << nextDeptTime << endl;
    nextDeptTime = max(nextDeptTime, idealDeptTime[lastDept + 1]);
    cout << "Vehicle " << id << " is predicted to depart at " << nextDeptTime << endl;
    predDeptTime.push_back(nextDeptTime);
}
