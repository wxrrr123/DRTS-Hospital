#include "GA.hpp"
#include "subsystem.hpp"
#include "system.hpp"

using namespace std;

int main() {
    /*
    TODO: input dataset
    */
   auto start = std::chrono::high_resolution_clock::now();

    GA ga;
    cout << ">>> GENERATION 0 <<<\n";
    ga.init();
    for (int i = 0; i < ga.generation; i++) {
        cout << "\n>>> GENERATION " << i + 1 << " <<<\n";
        ga.select();
        ga.crossover();
        ga.mutation();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::ratio<3600>> duration = end - start;
    cout << "Time Consumed: " << duration.count() << " hours" << std::endl;


    return 0;
}
