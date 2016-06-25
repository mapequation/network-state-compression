#ifndef __memNet__PathParser__
#define __memNet__PathParser__

#include "dependencies.h"
#include <vector>

template<typename Network>
class PathParser : public ParserModule::ParseHandler {

protected:
    Network *network;
    std::vector<double> v;

public:

    PathParser(Network *network) : network(network) {
    }

    bool isHandler(std::string line) {
        return line.find("*Paths") == 0;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            network->parsePath(v);
        }
    }

};

#endif




