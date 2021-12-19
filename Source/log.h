#ifndef LOG__
#define LOG__

#include <fstream>
#include <string>

#include "indicator.h"

class Population;

class Log
{
public:
    explicit Log(const std::string& name, const size_t run);
    ~Log() { trend_.close(); }

    // All individuals.
    void All(const Population& pop);

    // Pareto front.
    void Front(const Population& pop);

    // Pareto front of all runs.
    void Average(const Population& pop);

    void RecordIGD() { igdVals_.push_back(igd_.val()); }

    // Setup the file of run.
    void operator()(const int i);

private:
    std::size_t final_;

    std::string pname_;

    std::ofstream trend_;

    IGD igd_;
    std::vector<double> igdVals_;

};

#endif