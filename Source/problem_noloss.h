#ifndef NOLOSS__
#define NOLOSS__

#include <vector>
#include <fstream>
#include <limits>

#include "problem_base.h"

class NProblem : public BProblem
{
public:
    explicit NProblem(const int m, const int p, const int o, const std::string name = "NoLoss") : BProblem(name), numMachines_(m), numPeriods_(p), numObjectives_(o) {}
    ~NProblem() {}

    virtual const double limit(const int machine, const int i) const { return limits_[machine][i]; }
    virtual const double coeff(const int machine, const int coeffIndex) const { return coeffs_[machine][coeffIndex]; }
    virtual const double ramp(const int machine, const int i) const { return ramps_[machine][i]; }
    virtual const double load(const int period) const { return loads_[period]; }

    virtual const double B2(const int i, const int j) const { return std::numeric_limits<double>::min(); }
    virtual const double B1(const int i) const { return std::numeric_limits<double>::min(); }
    virtual const double B0() const { return std::numeric_limits<double>::min(); }

    virtual int numPeriods() const { return numPeriods_; }
    virtual int numMachines() const { return numMachines_; }
    virtual int numVariables() const { return numMachines_ * numPeriods_; }
    virtual int numObjectives() const { return numObjectives_; }

    virtual bool Read(const std::string&);
    virtual bool Evaluate(Individual&) const;
    
protected:
    std::vector<std::vector<double>> limits_;
    std::vector<std::vector<double>> coeffs_;
    std::vector<std::vector<double>> ramps_;
    std::vector<double> loads_;

    int numMachines_;
    int numPeriods_;
    int numObjectives_;

    bool SetCoeff(std::ifstream&);
    bool SetLoad(std::ifstream&);
};
#endif