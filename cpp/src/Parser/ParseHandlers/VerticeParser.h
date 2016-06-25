#ifndef __memNet__VerticeParser__
#define __memNet__VerticeParser__

#include "dependencies.h"

template<typename Network>
class VerticeParser : public ParserModule::ParseHandler {

protected:
    Network *network;
public:

    VerticeParser(Network *network) : network(network) {
    }

    bool isHandler(std::string line) {
        return line.find("*Vertices") == 0;
    }

    void parseLine(std::string &line) {
        std::stringstream iss(line);
        network->parseVertice(iss);
    }


protected:

};


#endif


