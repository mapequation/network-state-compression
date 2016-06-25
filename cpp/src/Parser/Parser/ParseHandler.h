#ifndef __memNet__ParseHandler__
#define __memNet__ParseHandler__

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Boost.h"


namespace ParserModule {

    const char COMMAND_CHAR = '*';

    const char COMMENT_CHAR = '#';

    /**
    * Interface for type specific format. It is using the chain of responsibility pattern to extend the format types at runtime.
    */
    class ParseHandler {

    protected:

        /** Next parser in the chain */
        ParseHandler *next = nullptr;

        /**
        * Determine if this parser is responsible for parsing the next lines of data.
        */
        virtual bool isHandler(std::string line) = 0;

    public:


        /**
        * This method is called by Network parser to get the responsible parser.
        */
        ParseHandler *getParser(std::string line);

        virtual void finalize() {
        }

        /**
        * Parse the row and do something with the data.
        */
        virtual void parseLine(std::string &line) = 0;

        std::istream &parse(std::istream &str, std::string &line);

        /**
        * Add a parser to the chain.
        */
        ParseHandler *add(ParseHandler *p);

        ParseHandler *getBaseHandler() {
            return next;
        }

    };

    std::istream &ParseHandler::parse(std::istream & str, std::string & line) {
                do {
                    if (line.find(COMMAND_CHAR) == 0) {
                        finalize();
                        return str;
                    }
                    if(line.find(COMMENT_CHAR) == 0){
                        continue;
                    }
                    parseLine(line);
                } while (std::getline(str, line));
                finalize();
                return str;
            }

    class PreParser : public ParseHandler {
    public:
        void parseLine(std::string &iss) {
        }

        bool isHandler(std::string line) {
            return false;
        }
    };

    class PostParser : public ParseHandler {
    public:
        void parseLine(std::string &iss) {
        }

        bool isHandler(std::string line) {
            return true;
        }
    };

    ParseHandler *ParseHandler::getParser(std::string line) {
        if (isHandler(line)) {
            return this;
        }
        if (next == nullptr) {
            next = new PostParser();
            return next->getParser(line);
        }
        return next->getParser(line);
    }


    ParseHandler *ParseHandler::add(ParseHandler *p) {
        next = p;
        return next;
    }


}
#endif