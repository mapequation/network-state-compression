#include "./ConditionalEntropy.h"
#include "./Parsers.h"


int getState(int prior, int physical, std::map <int, std::map<int, int>> &states, int &highestState) {
    if (states.count(prior) == 0 || states[prior].count(physical) == 0) {
        states[prior][physical] = ++highestState;
        return highestState;
    }
    return states[prior][physical];
}


void buildM1Links(I3DMap &map, I2DMap &links) {
    for (auto &priorIt : map) {
        for (auto &physIt : priorIt.second) {
            int phys = physIt.first;
            for (auto &nextIt : physIt.second) {
                links[phys][nextIt.first] += nextIt.second;
            }
        }
    }
}


void buildM2Links(I3DMap &map, I2DMap &links) {
    int prior, phys, highestState = 0;
    std::map <int, std::map<int, int>> states;
    for (auto &priorIt : map) {
        prior = priorIt.first;
        for (auto &physIt : priorIt.second) {
            phys = physIt.first;
            for (auto &nextIt : physIt.second) {
                links[getState(prior, phys, states, highestState)][getState(phys, nextIt.first, states,
                                                                            highestState)] += nextIt.second;
            }
        }
    }
}

int getStateFromMemoryKeys(int prior, int physical, I2DMap &priorPhysicalState, std::map <int, int> &danglingStates,
                           int &highestState) {
    if (priorPhysicalState.count(prior) == 0 || priorPhysicalState[prior].count(physical) == 0) {
        if (danglingStates.count(physical) == 0) {
            std::cout << " Added dangling state for node " << prior << " " << physical << std::endl;
            highestState++;
            danglingStates[physical] = highestState;
            priorPhysicalState[prior][physical] = highestState;
            return highestState;
        } else {
            // std::cout << errors << " Using dangling state for node " << prior << " " << physical << std::endl;
            return danglingStates[physical];
        }
    }
    return (int) priorPhysicalState[prior][physical];

}


void buildStateLinks(I3DMap &map, I2DMap &links, I2DMap &priorPhysicalState, std::map <int, int> &danglingStates, bool zeroBased) {
    int highestState = 1;
    if(zeroBased){
        highestState = 0;
    }
    for (auto &priorIt: priorPhysicalState) {
        for (auto &physIt : priorIt.second) {
            if (physIt.second > highestState) {
                highestState = physIt.second;
            }
        }
    }

    int prior, phys, next, state1, state2;
    std::map <int, std::map<int, int>> states;
    for (auto &priorIt : map) {
        prior = priorIt.first;
        for (auto &physIt : priorIt.second) {
            phys = physIt.first;
            for (auto &nextIt : physIt.second) {
                next = nextIt.first;
                state1 = getStateFromMemoryKeys(prior, phys, priorPhysicalState, danglingStates, highestState);
                // self link. Map ABB to states given by AB -> AB instead of AB -> BB
                if(next == phys){ 
                    state2 = state1;
                }else{
                    state2 = getStateFromMemoryKeys(phys, next, priorPhysicalState, danglingStates, highestState);
                }
                links[state1][state2] += nextIt.second;
            }
        }
    }
}

void printI2DMap(I2DMap &links, std::ostream &out = std::cout) {
    for (auto &fromIt : links) {
        for (auto &toIt : fromIt.second) {
            out << fromIt.first << " " << toIt.first << " " << toIt.second << std::endl;
        }
    }
}

void printStates(I2DMap &states, std::map <int, int> &danglingStates, I2DMap &links, std::ostream &out = std::cout) {
    std::map <int, std::pair<int, double>> statePhys;
    for(auto danglingIt : danglingStates){
        statePhys[danglingIt.second].first = danglingIt.first;
    }
    for (auto &fromIt : states) {
        for (auto &physIt : fromIt.second) {
            statePhys[physIt.second].first = physIt.first;
        }
    }
    for (auto &fromIt : links) {
        for (auto &toIt : fromIt.second) {
            statePhys[fromIt.first].second += toIt.second;
        }
    }

    for (auto &stateIt : statePhys) {
        out << stateIt.first << " " << stateIt.second.first << " " << stateIt.second.second << std::endl;
    }
}

void printI3DMap(I3DMap &map, std::ostream &out = std::cout) {
    for (auto &fromIt : map) {
        for (auto &physIt : fromIt.second) {
            for (auto &toIt : physIt.second) {
                out << fromIt.first << " " << physIt.first << " " << toIt.first << " " << toIt.second << std::endl;
            }
        }
    }
}

void print(CondData data, std::string header = "", std::ostream &out = std::cout) {
    out << header;
    out << "# Number of states: " << data.states << std::endl;
    out << "# Conditional entropy: " << data.entropy << std::endl;
    out << "# Weight: " << data.totW << std::endl;
}

void printHeader(CondData stateData, CondData m1Data, CondData m2Data, std::ostream &out = std::cout) {
    out << "# Number of states: " << stateData.states << std::endl;
    out << "# Compression ratio: " << stateData.entropy / m2Data.entropy << std::endl;
    out << "# Conditional entropy: " << stateData.entropy << std::endl;
    out << "# First order conditional entropy: " << m1Data.entropy << std::endl;
    out << "# Second order conditional entropy: " << m2Data.entropy << std::endl;
    out << "# 1st order nodes: " << m1Data.states << std::endl;
    out << "# 2nd order nodes: " << m2Data.states << std::endl;
    out << "# Weight: " << stateData.totW << std::endl;
}


bool parseZero(int argc, char **argv){
    bool zeroBased = false;
    for(int i = 1; i < argc; i++){
        std::string s = argv[i];
        if(s == "-z"){
            zeroBased = true;
        }
    }
    return zeroBased;
}