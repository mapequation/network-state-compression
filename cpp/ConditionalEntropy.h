#include <map>
#include <set>
#include <cmath>
#include <iostream>

struct CondData {
    double H_links, H_states, totW, entropy;
    int states;
};

CondData condEntropy(std::map<int, std::map<int, double >> &links) {
    std::map<int, double> states;
    std::set<int> uniqueNodes;

    int from, to;
    double H_links = 0, w, totW = 0;


    /**
     * Calculate link entropy
     */
    for (auto &fromIt: links) {
        from = fromIt.first;
        uniqueNodes.insert(from);
        for (auto &toIt : fromIt.second) {
            w = toIt.second;
            if (w > 0) {
                H_links += w * log2(w);
                states[from] += w;
                uniqueNodes.insert(to);
                totW += w;
            }
        }
    }

    /**
     * Calculate state entropy
     */
    double H_states = 0;
    for (auto &stateIt : states) {
        w = stateIt.second;
        if (w > 0) {
            H_states += w * log2(w);
        }
    }

    double entropy = (H_states - H_links) / totW;

    std::cout << H_links << " " << H_states << " " << totW << " " << entropy << std::endl;

    CondData data;
    data.H_links = H_links;
    data.H_states = H_states;
    data.totW = totW;
    data.entropy = entropy;
    data.states = uniqueNodes.size();

    return data;

}