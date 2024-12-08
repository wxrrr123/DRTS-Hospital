#include "patient.hpp"

Patient::Patient(int id, pair<int, int> dest, int added)
    : id(id), dest(dest), addedTime(added), getOnVehicleTime(0), region(0), returned(false) {}

void Patient::setRegion() {
    random_device rd;
    mt19937 gen(rd());

    // set region
    discrete_distribution<> dist_region({14.08, 19.19, 24.47, 19.37, 22.89});
    region = dist_region(gen) + 1;

    // set destination
    vector<int> distances = lineDist[region];
    vector<float> prob = areaProb[region];

    discrete_distribution<> dist_area(prob.begin(), prob.end());
    dest = dist2coord[distances[dist_area(gen)]];
}