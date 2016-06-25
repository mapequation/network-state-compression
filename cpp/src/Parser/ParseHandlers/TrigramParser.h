#ifndef __memNet__TrigramParser__
#define __memNet__TrigramParser__

#include "dependencies.h"

template<typename Network>
class TrigramParser : public ParserModule::ParseHandler {

protected:
    Network *network;
    std::vector<double> v;


public:

    TrigramParser(Network *network) : network(network) {
    }

    bool isHandler(std::string line) {
        return line.find("*3grams") == 0 || line.find("*ngrams") == 0;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            network->parsePath(v, v.size() > 3);
        }
    }

};

#endif




