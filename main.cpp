#include "ga.hpp"
#include "subsystem.hpp"
#include "system.hpp"

using namespace std;

int main() {
    /*
    TODO: input dataset
    */

    GA ga;
    cout << ">>> GENERATION 1 <<<" << endl;
    ga.init();
    for (int i = 0; i < ga.generation; i++) {
        cout << ">>> GENERATION " << i + 2 << " <<<" << endl;
        ga.crossover();
        ga.mutation();
    }

    return 0;
}
