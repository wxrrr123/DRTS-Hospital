#include "GA.hpp"
#include "subsystem.hpp"
#include "system.hpp"

using namespace std;

int main() {
    /*
    TODO: input dataset
    */

    GA ga;
    cout << ">>> GENERATION 0 <<<" << endl;
    ga.init();
    for (int i = 0; i < ga.generation; i++) {
        cout << ">>> GENERATION " << i + 1 << " <<<" << endl;
        ga.select();
        ga.crossover();
        ga.mutation();
    }

    return 0;
}
