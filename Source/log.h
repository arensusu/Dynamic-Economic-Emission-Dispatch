#ifndef LOG__
#define LOG__

#include <fstream>
#include <string>

class Population;

class Log
{
public:
    explicit Log(const std::string&, const int);
    ~Log() { trend_.close(); final_.close(); }

    void Trend(const Population&, const std::size_t);
    void Final(const Population&);

private:
    std::ofstream trend_;
    std::ofstream final_;

};

#endif