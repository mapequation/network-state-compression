#ifndef memNet_plogp_h
#define memNet_plogp_h

#include <stdio.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <map>

struct entropyData{
    double PlogP;
    double H;
    double sum;
};

double entropy(std::vector<double> *p);

double entropyMap(std::map<int, double> & outWeigths);

double entropyMap2(std::map<int, double> &m1, std::map<int, double> &m2);

double entropyMap22(std::map<int, double> &in1, std::map<int, double> &in2, std::map<int, double> &ou1, std::map<int, double> &out2);

entropyData entropy_local(std::vector<double> *p);

entropyData entropyMap2_local(std::map<int, double> &m1, std::map<int, double> &m2);

entropyData entropyMap_local(std::map<int, double> & outWeigths);

entropyData createEntropyData_local(double sum, double plogp);


#endif
