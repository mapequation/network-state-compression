#ifndef __memNet__Parser__
#define __memNet__Parser__
#include "ParseHandler.h"
#include "Boost.h"

namespace ParserModule {

    class Parser {

    public:

        std::istream &parse(std::istream &str);

    protected:

        void handleCommand(std::string line);

        std::istream &getline(std::istream &str, std::string &line);

        ParseHandler *parser = new PreParser();

        ParseHandler *handler = nullptr;

    };

    void Parser::handleCommand(std::string line) {
        this->handler = parser->getParser(line);
    }

    std::istream &Parser::getline(std::istream & str, std::string & line) {
        if (std::getline(str, line)) {
            if (line.find(COMMAND_CHAR) == 0) {
                handleCommand(line);
                return getline(str, line);
            }
            if (line.find(COMMENT_CHAR) == 0) {
                return getline(str, line);
            }
        }
        return str;
    }

    std::istream &Parser::parse(std::istream & str) {
        if (handler == nullptr) {
            handler = parser->getBaseHandler();
        }
        std::string line;
        while (getline(str, line)) {
            handler->parse(str, line);
            handleCommand(line);
        }
        return str;
    }

}
#endif
