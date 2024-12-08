#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <bits/stdc++.h>

#include <random>

using namespace std;

class Patient {
  public:
    int id;
    pair<int, int> dest;
    int addedTime;
    int getOnVehicleTime;
    int region;
    bool returned;

    map<int, vector<float>> areaProb = {{1, {65, 25, 10}},
                                        {2, {47.71, 30.27, 11.92, 3.67, 0.92, 4.59, 0.92}},
                                        {3, {37.41, 49.64, 10.07, 2.15, 0, 0.72, 0}},
                                        {4, {47.27, 45.45, 7.27}},
                                        {5, {40, 33.85, 13.85, 4.62, 1.54, 6.15}}};

    map<int, vector<int>> lineDist = {{1, {5, 12, 22}},
                                      {2, {5, 11, 18, 29, 35, 44, 55}},
                                      {3, {5, 7, 13, 28, 37, 44, 51}},
                                      {4, {5, 9, 20}},
                                      {5, {5, 9, 15, 22, 28, 40}}};

    map<int, pair<int, int>> dist2coord = {
        {5, {3, 4}},    {7, {4, 6}},    {9, {5, 8}},    {11, {6, 9}},   {12, {9, 8}},   {13, {5, 12}},
        {15, {9, 12}},  {18, {10, 15}}, {20, {12, 16}}, {22, {12, 18}}, {25, {15, 20}}, {27, {15, 22}},
        {28, {18, 21}}, {29, {21, 20}}, {40, {24, 32}}, {44, {33, 29}}, {51, {45, 24}}, {55, {33, 44}}};

    Patient() {};
    Patient(int id, pair<int, int> dest, int added);
    void setRegion();
};

#endif  // PATIENT_HPP
