#ifndef __SafeFile_H_
#define __SafeFile_H_

#include <fstream>

using std::ifstream;
using std::ofstream;

class SafeInFile : public std::ifstream
{
public:
    SafeInFile(const std::string filename, ios_base::openmode mode = ios_base::in)
            : ifstream(filename, mode)
    {
        if (fail())
            exit(1);
    }

    ~SafeInFile()
    {
        if (is_open())
            close();
    }
};

class SafeOutFile : public std::ofstream
{
public:
    SafeOutFile(const std::string filename, ios_base::openmode mode = ios_base::out)
            : ofstream(filename, mode)
    {
        if (fail())
            exit(1);
    }

    ~SafeOutFile()
    {
        if (is_open())
            close();
    }
};

#endif