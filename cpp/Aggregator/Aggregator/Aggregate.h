#ifndef __memNet__Aggregate__
#define __memNet__Aggregate__

#include <map>
#include <iostream>
#include <stdio.h>
#include <string>
#include "../plogp.h"

class Aggregate {
public:

    double H_in, H_out, p_in, p_out;
    std::map<int, double> outWeigths, inWeights, fromWeights;

    Aggregate(int from, std::map<int, double> &out) : Aggregate(std::map<int, double>(),
            out) {
        fromWeights[from] = p_out;
    }

    Aggregate(std::map<int, double> fromWeights, std::map<int, double> &out) :
            outWeigths(out),
            fromWeights(fromWeights) {
        p_out = sum(outWeigths);
        H_out = entropyMap(outWeigths);
    }


    void print(bool extended = false) {

        if(extended){
            std::cout << "[";
            for (auto &l : outWeigths) {
                std::cout << "(" << l.first << ":" << l.second << "), ";
            }
            std::cout << "from----\n";
            for (auto &l : fromWeights) {
                std::cout << "(" << l.first << ":" << l.second << "), ";
            }
            std::cout << "]";
        }
    }
};

Aggregate operator+(const Aggregate &a1, const Aggregate &a2) {
    std::map<int, double> outWeigths, fromWeights;
    merge(a1.outWeigths, a2.outWeigths, outWeigths);
    merge(a1.fromWeights, a2.fromWeights, fromWeights);

    return Aggregate(fromWeights, outWeigths);
}


#endif /* defined(__memNet__Aggregate__) */
