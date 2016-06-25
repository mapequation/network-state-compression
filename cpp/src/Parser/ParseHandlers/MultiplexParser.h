#ifndef __memNet__MpxParser__
#define __memNet__MpxParser__

#include "dependencies.h"


template<typename Network>
class MultiplexParser : public ParserModule::ParseHandler {

    int multiRows = 0, interRows = 0, intraRows = 0;

    enum Type {
        MULTIPLEX, INTER, INTRA
    };

    Network *network;
    Type type = MULTIPLEX;

public:

    MultiplexParser(Network *network) : network(network) {
    }

    void finalize() {
        std::cout << "Parsed " << multiRows << " " << intraRows << " " << interRows << " " << std::endl;
    }

    bool isHandler(std::string line) {
        if (line.find("*Multiplex") == 0) {
            type = MULTIPLEX;
            std::cout << "Multiplex parser" << std::endl;
            return true;
        }
        if (line.find("*Inter") == 0) {
            type = INTER;
            std::cout << "Inter parser" << std::endl;
            return true;
        }
        if (line.find("*Intra") == 0) {
            type = INTRA;
            std::cout << "Intra parser" << std::endl;
            return true;
        }
        return false;
    }

    void parseLine(std::string &line) {
        _parse(line);
    }

    void addMultiplex(int layer1, int node1, int layer2, int node2, double w = 1.0) {
        std::cout << "Added " << layer1 << " " << node1 << " " << layer2 << " " << node2 << std::endl;
    }

    void parseMultiPlex(std::string &str) {
        std::stringstream iss(str);
        int layer1, node1, layer2, node2;
        double w;
        if (iss >> layer1 >> node1 >> layer2 >> node2) {
            (iss >> w) || (w = 1.0);
            addMultiplex(layer1, node1, layer2, node2, w);
        } else {
            throw FileFormatError(io::Str() << "Can't parse link data from line '"<< -1 << "'");
        }
    }

    void parseInter(std::string &str) {
        std::stringstream iss(str);
        int layer1, node, layer2;
        double w;
        if (iss >> layer1 >> node >> layer2) {
            (iss >> w) || (w = 1.0);
            addMultiplex(layer1, node, layer2, node, w);
        } else {
            throw FileFormatError(io::Str() << "Can't parse link data from line '" << -1 << "'");
        }
    }

    void parseIntra(std::string &str) {
        std::stringstream iss(str);
        int layer, node1, node2;
        double w;
        if (iss >> layer >> node1 >> node2) {
            (iss >> w) || (w = 1.0);
            addMultiplex(layer, node1, layer, node2, w);
        } else {
            throw FileFormatError(io::Str() << "Can't parse link data from line '" << -1 << "'");
        }
    }


    void _parse(std::string &str) {
        switch (type) {
            case MULTIPLEX:
                multiRows++;
                return parseMultiPlex(str);
            case INTER:
                interRows++;
                return parseInter(str);
            case INTRA:
                intraRows++;
                return parseIntra(str);
        }
    }

};

#endif




