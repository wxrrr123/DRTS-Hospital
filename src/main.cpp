#include "GA.hpp"
#include "subsystem.hpp"
#include "system.hpp"

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();

    GA ga;

    ga.readAllPatients("./data/DLtimestamp.csv");
    cout << "***** " << ga.allPatients.size() << " patients loaded. *****\n";

    int noImproveCnt = 0;
    float lastBestFit = (float)INT_MAX;

    // for (int i = 0; i <= ga.generation; i++) {
    for (int i = 0; i <= INT_MAX; i++) {
        cout << "\n>>> GENERATION " << i << " <<<\n";

        if (i == 0) {
            ga.init();
        } else {
            ga.selection();
            ga.crossover();
            ga.mutation();
        }
        ga.simulation();

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, ratio<60>> duration = end - start;
        cout << "Time Consumed = " << duration.count() << " min" << endl;

        if (ga.bestChrom.fit < lastBestFit) {
            noImproveCnt = 0;
            lastBestFit = ga.bestChrom.fit;
        } else {
            noImproveCnt++;
        }

        if (noImproveCnt >= ga.noImproveMax) {
            cout << "\n***** No improvement for " << ga.noImproveMax << " generations. Stopping... *****\n";
            break;
        }

        if (duration.count() >= 80) {
            cout << "\n***** Time limit exceeded. Stopping... *****\n";
            break;
        }
    }
    
    ga.showBestAssignment();
    ga.testBestAssignment();

    for (auto& p : ga.allPatients) delete p;

    int retCode = system("python ./scripts/visualize.py");
    if (retCode) {
        cerr << "Error: Visualization script failed with return code " << retCode << endl;
    }

    // vector<string> chromStr = {"1011011101101110001010101011001001101011", "1000111011111100111101010111001110110110"};

    // for (auto& str : chromStr) {
    //     ga.bestChrom = ga.str2chrom(str);
    //     ga.bestSchedule = ga.chrom2sche(ga.assign, ga.bestChrom);
    //     // ga.bestChrom.fit = ga.sysDesignEval(ga.assign, ga.bestSchedule);
    //     ga.testBestAssignment();
    // }

    return 0;
}
