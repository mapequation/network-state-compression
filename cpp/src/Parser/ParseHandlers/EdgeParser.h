#ifndef __memNet__EdgeParser__
#define __memNet__EdgeParser__

#include "dependencies.h"
#include <vector>

template<typename Network>
class EdgeParser : public ParserModule::ParseHandler {

protected:
    Network *network;
    std::vector<double> v;

public:

    EdgeParser(Network *network) : network(network) {
    }

    bool isHandler(std::string line) {
        return line.find("*Edges") == 0
                || line.find("*Links") == 0
                || line.find("*Arcs") == 0;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            network->parsePath(v, v.size() > 2);
        }
    }


};

#endif




