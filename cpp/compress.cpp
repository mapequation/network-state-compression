#include "./Common.h"
#include "./plugins/Tree/Tree.h"
#include "./Aggregator/Aggregator/Aggregates.h"
#include <ctime>
#include <queue>

class Comparator {
public:
    bool operator()(const Tree *a, const Tree *b) {
        return (a->m_diff < b->m_diff);
    }
};

typedef std::priority_queue<Tree *, std::vector<Tree *>, Comparator>
        TreeQ;

struct ClusterData {
    double H_firstOrder, H_secondOrder, totalWeight;
    int numberStates_firstOrder, numberStates_secondOrder;
};

void buildPhysPriorMap(I3DMap &priorPhysNext, I3DMap &physPriorNext) {
    int prior, phys;
    for (auto &priorIt : priorPhysNext) {
        prior = priorIt.first;
        for (auto &physIt : priorIt.second) {
            phys = physIt.first;
            for (auto &nextIt : physIt.second) {
                physPriorNext[phys][prior][nextIt.first] = nextIt.second;
            }
        }
    }
}

void buildAggregates(I3DMap &physPriorNext, Metric
&metric, std::map<int, Aggregates> &aggs) {

    for (auto &n1 : physPriorNext) {
        int current = n1.first;
        auto it = aggs.insert(std::make_pair(current, Aggregates(current, metric)));
        for (auto &n2 : n1.second) {
            int prior = n2.first;
            it.first->second.addMemoryNode(prior, n2.second);
        }
    }
}

void buildTrees(I3DMap &physPriorNext, Metric &metric, std::map<int, Tree *> &trees) {

    for (auto &n1 : physPriorNext) {

        double H = 0;
        int current = n1.first;
        Aggregates aggregates(current, metric);

        std::cout << current << " " << std::flush;
        
        for (auto &n2 : n1.second) {
            int prior = n2.first;
            aggregates.addMemoryNode(prior, n2.second);
            // std::cout << prior << " " << current << " " << n2.second.size() << std::endl;
        }

        for (auto &aggIt : aggregates.aggregates) {
            Aggregate &agg = aggIt.second;
            H += aggregates.metric(agg);
        }

        aggregates.tree->init(H);

        while (aggregates.aggregates.size() > 1) {
            aggregates.mergeSmallest();
        }
        trees.insert(std::make_pair(current, aggregates.tree));
    }
    std::cout << "\nFinished building compression tree\n";
}


double buildClusters(std::map<int, Tree *> &trees, std::map<int, std::map<int, int>> &physicalPriorState,
                     std::map<int, int> &statePhysicalId, int &stateCounter, std::map<int, int> &danglingPhysicalState) {

    double H = 0;
    std::vector<std::vector<int>> partitions;
    std::map<int, int> physicalState;

    for (auto &it : trees) {
        int physicalId = it.first;
        Tree *tree = it.second;
        tree->getPartitions(partitions);
        H += tree->getValue();

        int largestState = stateCounter;
        int numberOfStates = 0;

        for (auto &partition : partitions) {
            if((int) partition.size() > numberOfStates){
                numberOfStates = partition.size();
                largestState = stateCounter;
            }
            for (int priorId : partition) {
                physicalPriorState[physicalId][priorId] = stateCounter;
            }
            statePhysicalId[stateCounter] = physicalId;
            stateCounter++;
        }
        danglingPhysicalState[physicalId] = largestState;
        //std::cout << "Largest state for physical node " << physicalId << " " << largestState << " " << numberOfStates <<  " " << partitions.size() << std::endl;
    }
    return H;

}

int getState(int prior, int phys, std::map<int, std::map<int, int>> &physicalPriorState, std::map<int, int> &danglingPhysicalState, std::map<int, int> &statePhysicalId, int &stateCounter){
    int state;
    if(physicalPriorState.count(phys) == 0 || physicalPriorState[phys].count(prior) == 0){
        if(danglingPhysicalState.count(phys) == 0){
            state = stateCounter++;
            danglingPhysicalState[phys] = state;
            statePhysicalId[state] = phys;
            //std::cout << "Added danging state... (" << prior << "," << phys << ")" << " " << state << "\n";
            return state;
        }
        return danglingPhysicalState[phys];
    }
    return physicalPriorState[phys][prior];
}

void printCluster(std::map<int, Tree *> &trees, I3DMap &physPriorNext, ClusterData data,
                  std::string filename, bool zeroBased) {

    int stateCounter = 1;
    if(zeroBased){
        stateCounter = 0;
    }

    std::map<int, std::map<int, int>> physicalPriorState;
    std::map<int, int> danglingPhysicalState;
    std::map<int, int> statePhysicalId;

    double H = buildClusters(trees, physicalPriorState, statePhysicalId, stateCounter, danglingPhysicalState);

    std::map<int, std::map<int, double>> links;

    std::map<int, double> stateWeights;
    

    int physical, prior, next, sourceState, targetState;
    for (auto &physIt : physPriorNext) {
        physical = physIt.first;
        for (auto &priorIt: physIt.second) {
            prior = priorIt.first;
            for (auto &nextIt: priorIt.second) {
                next = nextIt.first;
                sourceState = getState(prior, physical, physicalPriorState,danglingPhysicalState, statePhysicalId, stateCounter);
                // self link. Map ABB to states given by AB -> AB instead of AB -> BB
                if(next == physical){
                    targetState = sourceState;
                }else{
                    targetState = getState(physical, next, physicalPriorState,danglingPhysicalState, statePhysicalId, stateCounter);
                }
                links[sourceState][targetState] += nextIt.second;
            }
        }
    }

    for(auto &fromIt : links){
        for(auto &toIt : fromIt.second){
            stateWeights[fromIt.first] += toIt.second;
        }
    }

    SafeOutFile file(filename + std::to_string(stateCounter) + ".net");
    file << "# Number of states: " << stateCounter << std::endl;
    file << "# Compression ratio: " << H / data.H_secondOrder << std::endl;
    file << "# Conditional entropy: " << H / data.totalWeight << std::endl;
    file << "# First order conditional entropy: " << data.H_firstOrder / data.totalWeight << std::endl;
    file << "# Second order conditional entropy: " << data.H_secondOrder / data.totalWeight << std::endl;
    file << "# 1st order nodes: " << trees.size() << std::endl;
    file << "# 2nd order nodes: " << data.numberStates_secondOrder << std::endl;
    file << "# Weight: " << data.totalWeight << std::endl;
    

    file << "*States" << std::endl;
    file << "#state physicalId\n";

     for (auto &state : statePhysicalId) {
        file << state.first << " " << state.second << " " << stateWeights[state.first] << std::endl;
    }

    file << "*Arcs\n";
    file << "#fromState toState weight\n";
    for (auto &fromIt : links) {
        for (auto &toIt : fromIt.second) {
            file << fromIt.first << " " << toIt.first << " " << toIt.second << std::endl;
        }
    }

    file << "*MemoryNodes\n";
    file << "#priorId physicalId state\n";

    for (auto &physicalIt : physicalPriorState) {
        for (auto &priorIt : physicalIt.second) {
            file << priorIt.first << " " << physicalIt.first << " " << priorIt.second << "\n";
        }
    }

    file << "*DanglingStates\n";
    file << "#physicalId state\n";

    for (auto &it : danglingPhysicalState) {
        file << it.first << " " << it.second << "\n";
    }
}

void push(TreeQ &q, Tree *tree) {
    if (tree->diff() > -1 && !tree->empty()) {
        q.push(tree);
    }
}

double step(TreeQ &q) {
    Tree *tree = q.top();
    q.pop();
    double diff = tree->diff();
    tree->step();
    push(q, tree);
    return diff;
}


ClusterData handleTree(std::map<int, Tree *> &trees, I3DMap &physPriorNext, double weight, double increase, int limit, std::string filename = "network_N500_vomm_", bool zeroBased = false) {
    TreeQ q;

    double m2H = 0, m1H = 0, currentH;
    int m2StateNodes = 0;
    ClusterData data;

    for (auto &it : trees) {
        Tree *tree = it.second;
        m2H += tree->getInitValue();
        m1H += tree->getValue();
        m2StateNodes += tree->getNumberOfLeaves();
        push(q, tree);
    }

    currentH = m1H;
    std::cout << m2H << " " << m1H << std::endl;

    data.H_firstOrder = m1H;
    data.H_secondOrder = m2H;
    data.totalWeight = weight;
    data.numberStates_secondOrder = m2StateNodes;

    int i = 0;
    int states = trees.size();
    int m1States = states;
    double factor = 1;

    while (states < limit && currentH > 1.00001 * m2H) {
        if (i % 1000 == 0) {
            printf("%-5d %-5d %7f %5f\n", states, i, currentH / weight, currentH / m2H);
        }
        if (states >= m1States * factor) {
            factor *= increase;
            printf("%-5d %-5d %7f %5f -> printed to file\n", states, i, currentH / weight, currentH / m2H);
            printCluster(trees, physPriorNext, data, filename, zeroBased);
        }
        states++;
        i++;
        currentH -= step(q);
    }

    std::cout << "Handle the rest..." << std::endl;
    while (!q.empty()) {
        currentH -= step(q);
        states++;
        i++;
    }
    printf("%-5d %-5d %7f %5f\n", states, i, currentH / weight, currentH / m2H);
    printCluster(trees, physPriorNext, data, filename, zeroBased);

    return data;

}


int main(int argc, char **argv) {
   
    if (argc < 3) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << " networkPath(*ngrams || *Arcs) outPath [increase=1.25] [limit=1000000] [-z]" << std::endl;
        return 1;
    }

    bool zeroBased = parseZero(argc, argv);

    double increase = 1.25;
    int limit = 1000000;

    if(argc > 3){
        increase = atof(argv[3]);
    }
     if(argc > 4){
        limit = atoi(argv[4]);
    }

    

    std::cout << "Run compress....\n";
    printf("networkPath: %s\n", argv[1]);
    printf("outPath: %s\n", argv[2]);
    printf("limit: %d\n", limit);
    printf("increase: %f\n", increase);
    printf("zero based: %s\n", zeroBased ? "true" : "false");


    std::cout << "Parse network....\n";
    I3DMap network;
    parse3DMap(network, argv[1], "*ngrams");
    parse3DMap(network, argv[1], "*Arcs");

    if(network.size() == 0){
        std::cout << "No links found in file " << argv[1] << "... Make sure you use either *ngrams or *Arcs to define the links\n";
        return 1;
    }

    std::cout << "Build map....\n";
    I3DMap physPriorNext;
    buildPhysPriorMap(network, physPriorNext);

    std::map<int, Tree *> trees;
    OutEntropyMetric metric;

    std::cout << "Build tree....\n";
    buildTrees(physPriorNext, metric, trees);

    I2DMap m2Links;
    buildM2Links(network, m2Links);
    CondData m2Data = condEntropy(m2Links);

    std::cout << "Handle tree....\n";
    handleTree(trees, physPriorNext, m2Data.totW, increase, limit, argv[2], zeroBased);

}
