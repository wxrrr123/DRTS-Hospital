#include "GA.hpp"
#include "subsystem.hpp"
#include "system.hpp"

using namespace std;

int main() {
    /*
    TODO: input dataset
    */
    auto start = chrono::high_resolution_clock::now();

    GA ga;
    cout << ">>> GENERATION 0 <<<\n";
    ga.init();
    ga.simulation();

    for (int i = 0; i < ga.generation; i++) {
        cout << "\n>>> GENERATION " << i + 1 << " <<<\n";
        ga.selection();
        ga.crossover();
        ga.mutation();
        ga.simulation();
    }

    ga.showBestAssignment();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, ratio<3600>> duration = end - start;
    cout << "Time Consumed: " << duration.count() << " hours" << endl;

    return 0;
}
