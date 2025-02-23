#ifndef GA_HPP
#define GA_HPP

#include <bits/stdc++.h>

#include "system.hpp"

using namespace std;

struct Chromo {
    vector<vector<bool>> genes;
    float fit;
};

class GA {
  public:
    /* temp input dataset */
    const int dayNum = 30;
    const int regionNum = 5;
    const int startTime = 600;  // 10:00
    const int endTime = 1080;   // 18:00
    const int capacity = 15;
    const int tripNum = 4;
    const int vehNum = 5;
    vector<int> assign = {1, 1, 1, 1, 1};
    vector<vector<int>> schedule;

    /* GA parameters */
    int generation = 1;
    int chromNum = 100;
    int geneNum = tripNum * vehNum;
    int bitNum = 2;  // {bit:min} = {"00": +40min, "01": +75min, "10": +110min, "11": +150min}
    vector<int> bit2time = {40, 75, 110, 150};
    float sp = 1.5;    // selection pressure
    float pc = 0.5;    // crossover probibilty
    float pm = 0.005;  // mutation probability

    vector<Chromo> pop;
    vector<Chromo> candidates;

    Chromo bestChrom{{}, (float)INT_MAX};
    vector<vector<int>> bestSchedule;

    GA() {};
    void init();
    void selection();
    void crossover();
    void mutation();

    vector<vector<int>> chrom2sche(vector<int>& assign, Chromo& chrom);
    float totalPerformance(float totalKPI);
    float sysDesignEval(vector<int>& assign, vector<vector<int>>& schedule);
    void displayResult();
};

#endif  // GA_HPP