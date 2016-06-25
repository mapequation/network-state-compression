#ifndef BOOST__H
#define BOOST__H

#include <vector>
#ifdef USE_BOOST
#include <boost/spirit/include/qi.hpp>
namespace client {
    template <typename Iterator>
    bool parse_numbers(Iterator first, Iterator last, std::vector<double> &v) {
        bool r = boost::spirit::qi::phrase_parse(first, last, (*boost::spirit::qi::double_), boost::spirit::ascii::space, v);
        if (first != last) // fail if we did not get a full match
            return false;
        return r;
    }

    bool parse_numbers(std::string &s, std::vector<double> &v){
        return parse_numbers(s.begin(), s.end(), v);
    }
}
#else
namespace client {
    bool parse_numbers(std::string &str, std::vector<double> &v) {
        std::stringstream iss(str);
        double d;
        while(iss >> d){
            v.push_back(d);
        }
        return true;
    }
}
#endif

#endif