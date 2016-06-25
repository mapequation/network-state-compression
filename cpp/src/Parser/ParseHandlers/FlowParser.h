#ifndef __memNet__FlowParser__
#define __memNet__FlowParser__

#include "dependencies.h"
#include <vector>

template<typename Network>
class FlowParser : public ParserModule::ParseHandler {

protected:
    Network *network;
    std::vector<double> v;

    bool parseNodes = false;

    bool hasLinks = false, hasNodes = false;

public:

    FlowParser(Network *network) : network(network) {
    }

    bool isHandler(std::string line) {
        if (line.find("*FlowPaths") == 0) {
            hasLinks = true;
            network->hasFlow(hasLinks && hasNodes);
            parseNodes = false;
            return true;
        }
        if (line.find("*FlowNodes") == 0) {
            hasNodes = true;
            network->hasFlow(true);
            network->hasFlow(hasLinks && hasNodes);
            parseNodes = true;
            return true;
        }
        return false;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            if (parseNodes) {
                network->parseStateNode(v);
            }
            else {
                network->parsePath(v, true, true);
            }
        }
    }

};

#endif




