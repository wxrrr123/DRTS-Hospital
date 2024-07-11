#include "patient.hpp"

Patient::Patient(int id, string dept, string doc, pair<int, int> dest,
                 int arrival)
    : id(id),
      department(dept),
      doctor(doc),
      destination(dest),
      arrivalTime(arrival),
      completionTime(0),
      addedTime(0),
      isOnVehicle(false) {
    addedTime = arrival + 60;
}

void Patient::predictAddedTime(Patient &patient) {}

double Patient::distanceTo(Patient* patient) {
    int x1 = destination.first;
    int y1 = destination.second;
    int x2 = patient->destination.first;
    int y2 = patient->destination.second;
    return abs(x1 - x2) + abs(y1 - y2);
}

void Patient::backtrack(int V) {
    if (hamiltonianCycle.size() == V) {
        int currRoutingTime = 0;
        for (int i = 0; i < V - 1; i++) {
            currRoutingTime += graph[hamiltonianCycle[i]][hamiltonianCycle[i + 1]];
        }
        currRoutingTime += graph[hamiltonianCycle[V - 1]][hamiltonianCycle[0]];
        if (currRoutingTime < minRoutingTime) {
            minRoutingTime = currRoutingTime;
            bestHamiltonianCycle = hamiltonianCycle;
        }
        return;
    }
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            visited[i] = true;
            hamiltonianCycle.push_back(i);
            backtrack(V);
            visited[i] = false;
            hamiltonianCycle.pop_back();
        }
    }
}