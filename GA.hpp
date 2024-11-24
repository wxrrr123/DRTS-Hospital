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
    const int dayNum = 10;
    const int regionNum = 4;
    const int startTime = 600;
    const int endTime = 1080;
    const int capacity = 15;
    const int tripNum = 10;
    const int vehNum = 4;
    vector<int> assign = {2, 1, 2, 2};
    vector<vector<int>> schedule;

    /* GA parameters */
    int generation = 20;
    int chromNum = 8;
    int geneNum = tripNum * vehNum;
    int bitNum = 2;    // {bit:min} = {00:20, 01:30, 10:40, 11:60}
    float pc = 0.5;    // crossover probibilty
    float pm = 0.005;  // mutation probability

    vector<Chromo> pop;
    vector<Chromo> candidates;

    GA() {};
    void init();
    void select();
    void crossover();
    void mutation();
    vector<vector<int>> chrom2sche(vector<int>& assign, Chromo& chrom);
    float totalPerformance(float totalKPI);
    float sysDesignEval(vector<int>& assign, vector<vector<int>>& schedule);
};

#endif  // GA_HPP