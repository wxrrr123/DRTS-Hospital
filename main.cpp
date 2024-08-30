#include "system.hpp"

using namespace std;

vector<Patient*> readPatientData(string filename);

int main() {
    vector<Patient*> patientSamples = readPatientData("DLtimestamp.csv");

    vector<int> regions = {1, 1, 2, 3, 4};
    do {
        System* system = new System();

        for (auto& patient : patientSamples) system->addPatient(patient);

        system->clock = 600;
        system->sampleSize = patientSamples.size();

        system->waitingLine.resize(system->numOfRegion + 1);

        for (int i = 1; i <= 5; i++) system->addVehicle(i);
        for (int i = 0; i < system->vehicles.size(); i++) {
            system->vehicles[i]->region = regions[i];
        }

        system->generateSchedule();
        system->planReturnTrips();
        system->displayPlan();
        system->calculatePerformance();

        delete system;
    } while (next_permutation(regions.begin(), regions.end()));

    for (auto patient : patientSamples) delete patient;

    return 0;
}

vector<Patient*> readPatientData(string filename) {
    vector<Patient*> ret;

    ifstream file(filename);
    string line;
    getline(file, line);  // Skip the header line

    random_device rd;
    mt19937 gen(rd());

    vector<string> reservoir(100);
    int lineCount = 0;
    while (lineCount < 100 && getline(file, line)) reservoir[lineCount++] = line;  // Read the first 100 lines

    // Reservoir sampling for the rest of the lines
    while (getline(file, line)) {
        ++lineCount;
        if (uniform_int_distribution<>(0, lineCount - 1)(gen) < 100) {
            reservoir[uniform_int_distribution<>(0, 99)(gen)] = line;
        }
    }

    // Randomly generate 30 coordinates
    vector<pair<int, int>> coords;
    for (int j = 0; j < 30; j++) {
        coords.push_back({uniform_int_distribution<>(-20, 20)(gen), uniform_int_distribution<>(-20, 20)(gen)});
    }

    // Process the selected lines
    int id = 1;
    for (const auto& selectedLine : reservoir) {
        if (selectedLine.empty()) continue;  // Skip empty lines if any

        istringstream iss(selectedLine);
        string dept, temp, added_str;

        // Read department (dept)
        if (!getline(iss, dept, ',')) continue;

        // Skip intermediate fields
        for (int j = 0; j < 5; ++j) {
            if (!getline(iss, temp, ',')) break;
        }

        // Read medication time (added)
        if (!getline(iss, added_str, ',')) continue;

        // Check if department or medication time is empty
        if (dept.empty() || added_str.empty()) continue;

        // Convert medication time to minutes (assuming format is HH:MM)
        istringstream added_iss(added_str);
        string date, time;
        getline(added_iss, date, ' ');
        getline(added_iss, time);
        int hours = stoi(time.substr(0, 2));
        int minutes = stoi(time.substr(3, 2));
        int added = hours * 60 + minutes;

        if (added > 1080) continue;

        pair<int, int> coord = coords[uniform_int_distribution<>(0, 29)(gen)];

        Patient* patient = new Patient(id++, dept, coord, added);
        ret.push_back(patient);
    }

    return ret;
}
