#ifndef LOG__
#define LOG__

#include <fstream>
#include <string>

#include "indicator.h"

class Population;

class Log
{
public:
    explicit Log(const std::string&);
    ~Log() { trend_.close(); }

    void All(const Population&);
    void Trend(const Population&, const std::size_t);
    void FinalFront(const Population&);

    void RecordIGD() { igdVals_.push_back(igd_.val()); }

    void operator()(const int);

private:
    IGD igd_;
    std::string pname_;
    std::ofstream trend_;
    std::vector<double> igdVals_;

};

#endif