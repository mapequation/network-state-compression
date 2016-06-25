#ifndef __memNet__ClusterParser__
#define __memNet__ClusterParser__

#include "dependencies.h"

template<typename Network>
class ClusterParser : public ParserModule::ParseHandler {

protected:
    Network *network;
public:

    ClusterParser(bool memory = false) : network(network) {

    }



    bool isHandler(std::string line) {
        return true;
    }

    void parseLine(std::string &line) {
        std::stringstream iss(line);
        if(memory){
            is >> key.priorId >> key.physicalId;

        }

    }


protected:

};


#endif


