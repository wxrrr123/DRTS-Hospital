#ifndef GA_HPP
#define GA_HPP

#include <bits/stdc++.h>

#include <atomic>
#include <boost/random/sobol.hpp>
#include <mutex>
#include <random>
#include <semaphore>
#include <thread>

#include "system.hpp"

using namespace std;

struct Chromo {
    vector<vector<bool>> genes;
    float fit;
    float stdev;
    vector<int> dayNum;
    vector<float> metrics;
};

class GA {
  public:
    /* Input dataset */
    int initDayNum = 80;
    int regionNum = 5;
    int startTime = 600;  // 10:00
    int endTime = 1080;   // 18:00
    int capacity = 15;
    int tripNum = 4;
    int vehNum = 5;
    int sampleNum = 100;
    vector<Patient*> allPatients;
    vector<int> assign = {1, 1, 1, 1, 1};
    vector<vector<int>> schedule;
    
    /* GA parameters */
    int generation = 2000;
    int chromNum = 30;
    int geneNum = tripNum * vehNum;
    int bitNum = 2;  // {bit:min} = {"00": +40min, "01": +75min, "10": +110min, "11": +150min}
    vector<int> bit2time = {40, 75, 110, 150};
    float sp = 1.5;    // selection pressure
    float pc = 0.5;    // crossover probibilty
    float pm = 0.005;  // mutation probability

    /* Experimental parameters */
    int threadNum = chromNum;
    int isQuasi = 0;  // 0: uniform, 1: quasi
    vector<int> quasiOffset;

    vector<Chromo> pop;
    vector<Chromo> candidates;

    Chromo bestChrom{{}, (float)INT_MAX};
    vector<vector<int>> bestSchedule;

    GA() {};
    void init();
    void selection();
    void crossover();
    void mutation();

    Chromo str2chrom(string& str);
    vector<vector<int>> chrom2sche(vector<int>& assign, Chromo& chrom);
    float totalPerformance(Chromo& chrom);
    float sysDesignEval(vector<int>& assign, vector<vector<int>>& schedule, Chromo& chrom);
    void simulation();
    void showBestAssignment();
    void testBestAssignment();

    void ocba();
};

#endif  // GA_HPP