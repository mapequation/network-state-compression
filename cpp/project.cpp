#include "./Common.h"



/**
 * This program project a network (trigrams) to a state representation defined in the state file. 
 * The program convert a bigram in the network file to state number defined in the state file. 
 * If the bigram is missing in the state file, then the bigram is transformed to a new unique state. 
 *
 *
 *      Network file
 *     
 *         *Vertices 500
 *         1 "DRUG METAB REV"
 *         2 "INT REV PHYS CHEM"
 *         3 "SYST BIOL"
 *         
 *         *ngrams
 *         1 2 3 2
 *         1 2 1 3
 *         2 3 1 2
 *         3 1 2 1
 *
 * 
 *     State file: (i.e from another data set)
 *      
 *          *States
 *          #state physicalId
 *          0 1
 *          1 2
 *          2 2
 *          3 3
 *          
 *          *Arcs
 *          #fromState toState weight
 *          0 0 48
 *          0 21 3
 *          
 *          *MemoryNodes
 *          #priorId physicalId state
 *          1 2 0
 *          2 3 1
 *          2 1 2
 *          2 3 3
 *          #3 1 - this state is missing in this data set
 *
 *
 *      In this example the ngrams in the network file will be converted to the folliwong
 *      (1,2) -> 0 
 *      (2,3) -> 1
 *      (2,1) -> 2
 *      (2,3) -> 3
 *      (3,1) -> 4  # a new state
 *
 *      Hence the paths will be
 *      0 1 2
 *      0 2 3
 *      1 4 2
 *      4 0 1
 *
 */


CondData getM1Data(I3DMap &network){
    I2DMap m1Links;
    buildM1Links(network, m1Links);
    return condEntropy(m1Links);
}

CondData getM2Data(I3DMap &network){
    I2DMap m2Links;
    buildM2Links(network, m2Links);
    return condEntropy(m2Links);
}

int main(int argc, char *argv[]) {
    if (argc < 4) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << " networkPath(*ngrams || *Arcs) statePath(*MemoryNodes && *DanglingStates) outPath [-z]" << std::endl;
        return 1;
    }
    
    bool zeroBased = parseZero(argc, argv);
    printf("zero based: %s\n", zeroBased ? "true" : "false");

    I2DMap memoryNodes;
    parse2DMap(memoryNodes, argv[2], "*MemoryNodes");

    I3DMap network;
    parse3DMap(network, argv[1], "*ngrams");
    parse3DMap(network, argv[1], "*Arcs");

    if(network.size() == 0){
        std::cout << "No links found in file " << argv[1] << "... Make sure you use either *ngrams or *Arcs to define the links\n";
        return 1;
    }


    IIMap danglingPhysToState;
    parseIIMap(danglingPhysToState, argv[2], "*DanglingStates");
    

    CondData m1Data = getM1Data(network);
    print(m1Data, "### M1 Data ###\n");

    CondData m2Data = getM2Data(network);
    print(m2Data, "### M2 Data ###\n");


    I2DMap stateLinks;
    buildStateLinks(network, stateLinks, memoryNodes, danglingPhysToState, zeroBased);
    CondData stateData = condEntropy(stateLinks);
    print(stateData, "### State Data ###\n");

    SafeOutFile outFile(argv[3]);
    printHeader(stateData, m1Data, m2Data, outFile);

    outFile << "*States\n";
    printStates(memoryNodes, danglingPhysToState, stateLinks, outFile);

    outFile << "*Arcs\n";
    printI2DMap(stateLinks, outFile);


}
