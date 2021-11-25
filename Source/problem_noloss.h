#ifndef NOLOSS__
#define NOLOSS__

#include <vector>
#include <fstream>
#include <limits>

#include "problem_base.h"

class NProblem : public BProblem
{
public:
    explicit NProblem(const std::string name = "NoLoss") : BProblem(name), numMachines_(0), numPeriods_(0), numObjectives_(0) {}
    virtual ~NProblem() {}

    virtual const double limit(const int machine, const int i) const { return limits_[machine][i]; }
    virtual const double coeff(const int machine, const int coeffIndex) const { return coeffs_[machine][coeffIndex]; }
    virtual const double ramp(const int machine, const int i) const { return ramps_[machine][i]; }
    virtual const double load(const int period) const { return loads_[period]; }

    virtual const double B2(const int i, const int j) const { return std::numeric_limits<double>::min(); }
    virtual const double B1(const int i) const { return std::numeric_limits<double>::min(); }
    virtual const double B0() const { return std::numeric_limits<double>::min(); }

    virtual size_t numPeriods() const { return numPeriods_; }
    virtual size_t numMachines() const { return numMachines_; }
    virtual size_t numVariables() const { return numMachines_ * numPeriods_; }
    virtual size_t numObjectives() const { return numObjectives_; }

    virtual bool Read(const std::string&);

    virtual bool Evaluate(Individual&) const;
    virtual bool Evaluate(std::vector<double>&, const std::vector<double>&) const;
    
protected:
    std::vector<std::vector<double>> limits_;
    std::vector<std::vector<double>> coeffs_;
    std::vector<std::vector<double>> ramps_;
    std::vector<double> loads_;

    std::size_t numMachines_;
    std::size_t numPeriods_;
    std::size_t numObjectives_;

    bool SetCoeff(std::ifstream&);
    bool SetLoad(std::ifstream&);
};
#endif