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
    ~Log() { trend_.close(); detail_.close(); child_.close(); op_.close(); }

    // All individuals.
    void All(const Population& pop);

    // Pareto front.
    void Front(const Population& pop);

    // Genes of population.
    void Detail(const Population& pop);

    void Child(const Population& pop);
    std::ofstream& child() { return child_; }

    void OpCount(const std::vector<int>& counts);

    void OpCount(const std::vector<double>& counts);

    // Pareto front of all runs.
    void Average(const Population& pop);

    void RecordIGD() { igdVals_.push_back(igd_.val()); }

    // Setup the file of run.
    void operator()(const int i);

    void operator++(int) { infeasibleTime_++; }

private:
    std::size_t final_;

    std::string pname_;

    std::ofstream trend_;

    std::ofstream detail_;

    std::ofstream op_;

    std::ofstream child_;

    IGD igd_;
    std::vector<double> igdVals_;

    std::size_t infeasibleTime_ = 0;
};

#endif