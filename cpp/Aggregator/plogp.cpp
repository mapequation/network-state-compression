#include "plogp.h"

inline
double log2(double p)
{
    return std::log(p) * M_LOG2E;
}

inline
double plogp(double p)
{
    return p > 0.0 ? p * log2(p) : 0.0;
}

double entropy(std::vector<double> *p){
    return entropy_local(p).H;
}

double entropyMap(std::map<int, double> & outWeigths){
    return entropyMap_local(outWeigths).H;
}

double entropyMap2(std::map<int, double> &m1, std::map<int, double> &m2){
    return entropyMap2_local(m1, m2).H;
}

double entropyMap22(std::map<int, double> &in1, std::map<int, double> &in2, std::map<int, double> &out1, std::map<int, double> &out2){
    entropyData in = entropyMap2_local(in1, in2);
    entropyData out = entropyMap2_local(out1, out2);
    return createEntropyData_local(in.sum + out.sum, in.H + out.H).H;
}

entropyData createEntropyData_local(double sum, double H){
    entropyData data;
    data.sum = sum;
    data.H = H;
    if(sum > 0){
        data.H = H / sum + log2(sum);
    }else{
        data.sum = 0.0;
        data.H = 0.0;
    }
    return data;
}

entropyData entropy_local(std::vector<double> *p){
    double sum = 0;
    double H = 0;
    for(unsigned int i = 0; i < p->size(); i++){
        H -= plogp(p->at(i));
        sum += p->at(i);
    }
    return createEntropyData_local(sum, H);
}


entropyData entropyMap_local(std::map<int, double> & outWeigths){
    double sum = 0;
    double H = 0;
    for(auto & d : outWeigths){
        H -= plogp(d.second);
        sum += d.second;
    }
    return createEntropyData_local(sum, H);
}


// Calculate entropy for 2 independent distributions.
entropyData entropyMap_local(std::map<int, double> &m1, std::map<int, double> &m2){
    double sum = 0;
    double H = 0;
    for(auto & d : m1){
        H -= plogp(d.second);
        sum += d.second;
    }
    for(auto & d : m2){
        H -= plogp(d.second);
        sum += d.second;
    }
    return createEntropyData_local(sum, H);
}




// Calculate entropy for two dependent distributions
entropyData entropyMap2_local(std::map<int, double> &m1, std::map<int, double> &m2){
    std::map<int, double>::iterator left = m1.begin();
    std::map<int, double>::iterator right = m2.begin();
    
    double diff;
    double p;
    double sum = 0;
    double H = 0;
    
    while(left != m1.end() && right != m2.end()){
        diff = left->first - right->first;
        if(diff < 0){
            p = left->second;
            left++;
        }
        else if(diff > 0){
            p = right->second;
            right++;
        }
        else{
            p = left->second + right->second;
            left++; right++;
        }
        H -= plogp(p);
        sum += p;
    }
    while(left != m1.end()){
        p = left->second;
        H -= plogp(p);
        sum += p;
        left++;
    }
    
    while(right != m2.end()){
        p = right->second;
        H -= plogp(p);
        sum += p;
        right++;
    }
    return createEntropyData_local(sum, H);
}