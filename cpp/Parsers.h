#include "./src/Parser/ParseHandlers/dependencies.h"
#include "./src/Parser/Parser.h"
#include "./SafeFile.h"
#include <vector>
#include <string>
#include <map>


typedef std::map<int, std::map<int, double>> I2DMap;
typedef std::map<int, I2DMap> I3DMap;
typedef std::map<int, int> IIMap;

struct I3DParseHandler : public ParserModule::ParseHandler {
    I3DMap &map;
    std::vector<double> v;
    std::string command;

    I3DParseHandler(I3DMap &map, std::string str) : map(map), command(str) { }

    bool isHandler(std::string line) {
        return line.find(command) == 0;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            if (v.size() == 4) {
                map[v[0]][v[1]][v[2]] = v[3];
            }
        }
    }
};

struct I2DParseHandler : public ParserModule::ParseHandler {

    I2DMap &map;
    std::vector<double> v;
    std::string command;

    I2DParseHandler(I2DMap &map, std::string str) : map(map), command(str) { }

    bool isHandler(std::string line) {
        return line.find(command) == 0;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            if (v.size() == 3) {
                map[v[0]][v[1]] = v[2];
            }
        }
    }
};

struct IIParseHandler : public ParserModule::ParseHandler {

    IIMap &map;
    std::vector<double> v;
    std::string command;

    IIParseHandler(IIMap &map, std::string str) : map(map), command(str) { }

    bool isHandler(std::string line) {
        return line.find(command) == 0;
    }

    void parseLine(std::string &line) {
        v.clear();
        if (client::parse_numbers(line, v)) {
            if (v.size() == 2) {
                map[v[0]] = v[1];
            }
        }
    }
};

struct I2DParser : public ParserModule::Parser {

    I2DParseHandler *handler;

    I2DParser(I2DMap &map, std::string command) {
        handler = new I2DParseHandler(map, command);
        parser->add(handler);
    }

};

struct I3DParser : public ParserModule::Parser {

    I3DParseHandler *handler;

    I3DParser(I3DMap &map, std::string command) {
        handler = new I3DParseHandler(map, command);
        parser->add(handler);
    }

};

struct IIParser : public ParserModule::Parser {

    IIParseHandler *handler;

    IIParser(IIMap &map, std::string command) {
        handler = new IIParseHandler(map, command);
        parser->add(handler);
    }
};


void parse2DMap(I2DMap &map, std::string path, std::string command = "") {
    I2DParser parser(map, command);
    SafeInFile file(path);
    parser.parse(file);
}

void parse3DMap(I3DMap &map, std::string path, std::string command = "") {
    I3DParser parser(map, command);
    SafeInFile file(path);
    parser.parse(file);
}

void parseIIMap(IIMap &map, std::string path, std::string command = "") {
    IIParser parser(map, command);
    SafeInFile file(path);
    parser.parse(file);
}




