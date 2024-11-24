#include "GA.hpp"

#include <random>

void GA::init() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, (endTime - startTime) / 60 - 1);

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

        // Print the genes of the chromosome and its fitness
        cout << "Chromosome " << i + 1 << " => ";
        for (auto& gene : chrom.genes) {
            for (auto bit : gene) cout << bit;
            cout << " ";
        }
        schedule = chrom2sche(assign, chrom);
        chrom.fit = sysDesignEval(assign, schedule);
        totalFit += chrom.fit;
        cout << "Fitness = " << chrom.fit << endl;
    }
        printf("Average Fitness = %.3f\n", totalFit / chromNum);

    return;
}

void GA::select() {
    // Calculate the total fitness for each chromosome
    float totalFitness = accumulate(pop.begin(), pop.end(), 0, [](float sum, Chromo& chrom) {
        return sum + chrom.fit;  // Sum of all fitness values
    });

    // Calculate selection probability for each chromosome
    vector<float> selectProb(chromNum);
    for (int i = 0; i < chromNum; i++) {
        selectProb[i] = pop[i].fit / totalFitness;  // Probability based on fitness
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

        // Compute the fitnesses
        schedule = chrom2sche(assign, offspring1);
        offspring1.fit = sysDesignEval(assign, schedule);
        schedule = chrom2sche(assign, offspring2);
        offspring2.fit = sysDesignEval(assign, schedule);

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

    // Print the genes of the chromosome and its fitness
    int i = 1;
    float totalFit = 0;
    for (auto chrom : pop) {
        cout << "Chromosome " << i++ << " => ";
        for (auto& gene : chrom.genes) {
            for (auto bit : gene) cout << bit;
            cout << " ";
        }

        schedule = chrom2sche(assign, chrom);
        chrom.fit = sysDesignEval(assign, schedule);
        cout << "Fitness = " << chrom.fit << endl;
        totalFit += chrom.fit;
    }
    printf("Average Fitness = %.3f\n", totalFit / chromNum);
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
                    time = 20;
                    break;
                case 1:
                    time = 30;
                    break;
                case 2:
                    time = 40;
                    break;
                case 3:
                    time = 60;
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
        S->patients = S->readPatientData("DLtimestamp.csv");

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
};
