#include "vehicle.hpp"

void Vehicle::predictReturnTime() {
    int deptTime = realDeptTime.back();
    // printf("Actual departure time: %02d:%02d\n", deptTime / 60, deptTime % 60);

    int nextRetTime = deptTime;
    pair<int, int> currNode = {0, 0};
    vector<pair<int, Patient *>> distances;

    /* Distances from the hospital */
    for (auto &p : patients) {
        auto [x, y] = p->dest;
        distances.push_back({round(sqrt(x * x + y * y)), p});
    }

    sort(distances.begin(), distances.end());

    for (auto [dist, p] : distances) {
        auto [currX, currY] = currNode;
        auto [nextX, nextY] = p->dest;
        nextRetTime += round(sqrt((nextX - currX) * (nextX - currX) + (nextY - currY) * (nextY - currY)));
        currNode = {nextX, nextY};
    }

    auto [lastX, lastY] = currNode;
    nextRetTime += round(sqrt(lastX * lastX + lastY * lastY));

    // printf("Predict return time: %02d:%02d\n", nextRetTime / 60, nextRetTime % 60);
    returnTime.push_back(nextRetTime);
}