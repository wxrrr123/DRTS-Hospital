#include "vehicle.hpp"

void Vehicle::predictRoutingTime() {
    int lastDept = predDeptTime.size() - 1;
    int lastDeptTime = max(idealDeptTime[lastDept], predDeptTime[lastDept]);
    printf("Actual departure time: %02d:%02d\n", lastDeptTime / 60, lastDeptTime % 60);

    int nextDeptTime = lastDeptTime;
    pair<int, int> currNode = {0, 0};
    vector<pair<int, Patient *>> distances;

    /* Distances from the hospital */
    for (auto &patient : patients) {
        auto [x, y] = patient->destination;
        distances.push_back({round(sqrt(x * x + y * y) / 1), patient});
    }

    sort(distances.begin(), distances.end());

    for (auto [distance, patient] : distances) {
        auto [currX, currY] = currNode;
        auto [nextX, nextY] = patient->destination;
        nextDeptTime += round(sqrt((nextX - currX) * (nextX - currX) + (nextY - currY) * (nextY - currY)) / 1);
        currNode = {nextX, nextY};
    }

    auto [lastX, lastY] = currNode;
    nextDeptTime += round(sqrt(lastX * lastX + lastY * lastY) / 1);

    printf("Predict return time: %02d:%02d\n", nextDeptTime / 60, nextDeptTime % 60);
    predDeptTime.push_back(nextDeptTime);
}