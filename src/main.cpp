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

    // Read all patient data once
    System S;
    ga.allPatients = S.readPatientData("./data/DLtimestamp.csv");
    cout << ga.allPatients.size() << " patients loaded.\n\n";

    cout << ">>> GENERATION 0 <<<\n";
    ga.init();
    ga.simulation();

    for (int i = 0; i < ga.generation; i++) {
        cout << "\n>>> GENERATION " << i + 1 << " <<<\n";
        ga.selection();
        ga.crossover();
        ga.mutation();
        ga.simulation();
        
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, ratio<60>> duration = end - start;
        cout << "Time Consumed = " << duration.count() << " min\n";
    }
    
    ga.showBestAssignment();
    ga.testBestAssignment();

    for (auto& p : ga.allPatients) delete p;

    int retCode = system("python ./scripts/visualize.py");
    if (retCode) {
        cerr << "Error: Visualization script failed with return code " << retCode << endl;
    }

    // vector<string> chromStr = {"1011101001111110111111111010101010101111", "1001101110101110001111111110000001111111",
    //                            "1110011011111101011010101110100111111111", "1010001111111010011111100111101111100111",
    //                            "1001101110101110010110111011111111100110", "1101010110111010111101101110000110101010",
    //                            "0110100110111011111011111100110010101011", "1110111010111101011110111111010101111111",
    //                            "1110011011101101111101101011100110011011", "0111100101111011010111111011111011011111"};

    // for (auto& str : chromStr) {
    //     ga.bestChrom = ga.str2chrom(str);
    //     ga.bestSchedule = ga.chrom2sche(ga.assign, ga.bestChrom);
    //     // ga.bestChrom.fit = ga.sysDesignEval(ga.assign, ga.bestSchedule);
    //     ga.testBestAssignment();
    // }

    return 0;
}
