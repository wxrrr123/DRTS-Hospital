#include "GA.hpp"

void GA::init() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, (1 << (bitNum - 1)) - 1);

    float totalFit = 0;
    for (int i = 0; i < chromNum; i++) {
        // Store unique random integers standing for dept time
        vector<int> dept;
        for (int i = 0; i < geneNum; i++) {
            dept.push_back(dist(gen));
        }

        // Convert each integer in the set to binary form and store it as a gene
        Chromo chrom;
        for (auto& d : dept) {
            vector<bool> gene(bitNum);
            for (int k = 0; k < bitNum; k++) {
                gene[bitNum - 1 - k] = (d >> k) & 1;  // Convert decimal to binary representation
            }
            chrom.genes.push_back(gene);  // Add the binary gene to the chromosome
        }
        pop.push_back(chrom);  // Add the chromosome to the population
    }
}

void GA::selection() {
    // Calculate ranked weight of each chromosome
    sort(pop.begin(), pop.end(), [](const Chromo& a, const Chromo& b) { return a.fit < b.fit; });

    // Linear ranking selection
    vector<float> rankedWeight(chromNum);
    for (int i = 1; i <= chromNum; i++) {
        rankedWeight[i] = 2 - sp + 2 * (sp - 1) * (i - 1) / (chromNum - 1);
    }

    float totalFitness = 0;
    for (int i = 0; i < chromNum; i++) {
        totalFitness += rankedWeight[i];  // Sum of all fitnesses
    }

    // Calculate selection probability for each chromosome
    vector<float> selectProb(chromNum);
    for (int i = 0; i < chromNum; i++) {
        selectProb[i] = rankedWeight[i] / totalFitness;  // Probability based on ranked weight
    }

    // Calculate cumulative probability for selection
    vector<float> cumProb(chromNum);
    cumProb[0] = selectProb[0];
    for (int i = 1; i < chromNum; i++) {
        cumProb[i] = cumProb[i - 1] + selectProb[i];  // Cumulative sum
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0, 1.0);  // Generate a random number between [0, 1]

    // Roulette selection to choose chromosomes to crossover
    set<int> selectedIdx;
    while (selectedIdx.size() < pc * chromNum) {
        float r = dist(gen);

        // First chromosome
        if (r < cumProb[0]) {
            selectedIdx.insert(0);
            continue;
        }

        // Select the chromosome corresponding to the cumulative probability
        for (int i = 1; i < cumProb.size(); i++) {
            if (r < cumProb[i] && r >= cumProb[i - 1]) {
                selectedIdx.insert(i);
            }
        }
    }

    candidates.clear();
    for (int idx : selectedIdx) candidates.push_back(pop[idx]);
}

void GA::crossover() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0, 1.0);

    // Mate selected chromosomes randomly
    for (int i = 0; i < candidates.size(); i += 2) {
        if (i + 1 >= candidates.size()) break;  // Ensure we have a pair

        Chromo &parent1 = candidates[i], &parent2 = candidates[i + 1];

        // Randomly select crossover point
        int pos = round(dist(gen) * (parent1.genes.size() - 1));

        // Create offspring
        Chromo offspring1, offspring2;

        // First part from first parent, second part from second parent
        offspring1.genes.insert(offspring1.genes.end(), parent1.genes.begin(), parent1.genes.begin() + pos);
        offspring1.genes.insert(offspring1.genes.end(), parent2.genes.begin() + pos, parent2.genes.end());

        // Reverse for the second offspring
        offspring2.genes.insert(offspring2.genes.end(), parent2.genes.begin(), parent2.genes.begin() + pos);
        offspring2.genes.insert(offspring2.genes.end(), parent1.genes.begin() + pos, parent1.genes.end());

        // Replace the parents
        parent1 = offspring1, parent2 = offspring2;
    }
}

void GA::mutation() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0, 1.0);

    for (int i = 0; i < chromNum; i++) {
        for (int j = 0; j < geneNum; j++) {
            for (int k = 0; k < bitNum; k++) {
                // Generate a random float number r in the range [0, 1]
                float r = dist(gen);

                // If r is less than the mutation probability pm, flip the bit
                if (r < pm) pop[i].genes[j][k] = !pop[i].genes[j][k];  // Mutate the bit by flipping it
            }
        }
    }
}

float GA::totalPerformance(float totalKPI) {
    // printf("\n>>>>>>> SYSTEM DESIGN KPI = %.1f <<<<<<<\n", totalKPI / dayNum);
    return totalKPI / dayNum;
}

vector<vector<int>> GA::chrom2sche(vector<int>& assign, Chromo& chrom) {
    vector<vector<int>> sche(regionNum);

    int geneIdx = 0;  // Index to track the position in genes
    for (int i = 0; i < regionNum; i++) {
        for (int j = 0; j < assign[i] * tripNum; j++, geneIdx++) {  // Loop through each vehicle for the current region
            // Decode the binary genes for time interval
            int code = (chrom.genes[geneIdx][0] << 1) | chrom.genes[geneIdx][1], time = 0;
            switch (code) {
                case 0:
                    time = bit2time[0];
                    break;
                case 1:
                    time = bit2time[1];
                    break;
                case 2:
                    time = bit2time[2];
                    break;
                case 3:
                    time = bit2time[3];
                    break;
                default:
                    break;
            }

            // Calculate the actual time based on the previous departure time
            int actualTime = sche[i].size() ? sche[i].back() + time : 600 + time;
            actualTime = actualTime > endTime ? endTime : actualTime;
            sche[i].push_back(actualTime);
        }
    }

    return sche;
}

float GA::sysDesignEval(vector<int>& assign, vector<vector<int>>& schedule) {
    float totalKPI = 0;

    for (int d = 0; d < dayNum; d++) {
        System* S = new System();

        /* initiate system */
        S->assign = assign;
        S->schedule = schedule;
        S->patients = S->readPatientData("./data/DLtimestamp.csv");

        int vehNum = accumulate(assign.begin(), assign.end(), 0);
        for (int i = 0; i < vehNum; i++) {
            Vehicle* v = new Vehicle(i + 1, capacity, tripNum);
            S->addVehicle(v);
        }

        /* initiate subsystem */
        int vehId = 0;
        for (int i = 0; i < regionNum; i++) {
            Subsystem* s = new Subsystem(i + 1, startTime, endTime);
            S->addSubsystem(s);
            s->setSchedule(schedule[i]);

            for (auto& p : S->patients) {
                if (p->region == s->id) s->addPatient(p);
            }

            for (int j = 0; j < assign[s->id - 1]; j++) s->addVehicle(S->vehicles[vehId++]);
        }

        for (auto s : S->subsystems) {
            // printf("\n>>> SUBSYSTEM %d <<<", s->id);
            s->oneDaySimulation();
        }

        // S->displayPlan();
        totalKPI += S->oneDayPerformance();

        /* destruction */
        for (auto& p : S->patients) delete p;
        S->patients.clear();

        for (auto& v : S->vehicles) delete v;
        S->vehicles.clear();

        for (auto& s : S->subsystems) delete s;
        S->subsystems.clear();

        delete S;
    }

    return totalPerformance(totalKPI);
}

void GA::displayResult() {  // Print the genes of the chromosome and its fitness
    cout << "Processing..." << endl;
    int i = 1;
    float totalFit = 0;
    mutex mtx;
    vector<thread> threads;

    for (auto& chrom : pop) {
        threads.push_back(thread([&]() {
            // Calculate fitness in each thread
            vector<vector<int>> schedule = chrom2sche(assign, chrom);
            chrom.fit = sysDesignEval(assign, schedule);

            // Print the chromosome and its fitness
            {
                lock_guard<mutex> lock(mtx);  // Protect access to cout
                cout << "Chromosome " << i++ << " => ";
                for (auto& gene : chrom.genes) {
                    for (auto bit : gene) cout << bit;
                    cout << " ";
                }
                cout << "Fitness = " << chrom.fit << endl;
            }

            // Update total fitness
            {
                lock_guard<mutex> lock(mtx);  // Protect access to totalFit
                totalFit += chrom.fit;
            }

            // Update the best chromosome
            {
                lock_guard<mutex> lock(mtx);  // Protect access to bestChrom
                if (chrom.fit < bestChrom.fit) {
                    bestChrom = chrom;
                    bestSchedule = schedule;
                }
            }
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    printf("Average Fitness = %.3f\n", totalFit / chromNum);
    printf("Best Ever Fitness = %.3f\n", bestChrom.fit);

    cout << "Best Ever Vehicle Assignment:" << endl;
    for (int i = 0; i < bestSchedule.size(); i++) {
        printf("\tRegion %d: ", i + 1);
        for (int j = 0; j < bestSchedule[i].size(); j++) {
            printf("%02d:%02d ", bestSchedule[i][j] / 60, bestSchedule[i][j] % 60);
        }
        cout << endl;
    }
}