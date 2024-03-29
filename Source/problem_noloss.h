#ifndef NOLOSS__
#define NOLOSS__

#include <vector>
#include <fstream>
#include <limits>

#include "problem_base.h"

class NProblem : public BProblem
{
public:
    explicit NProblem(const std::string name = "NoLoss") : BProblem(name) {}
    virtual ~NProblem() {}

    // Basic settings of generators.
    virtual const double limit(const std::size_t machine, const std::size_t i) const { return limits_[machine][i]; }
    virtual const double coeff(const std::size_t machine, const std::size_t coeffIndex) const { return coeffs_[machine][coeffIndex]; }
    virtual const double ramp(const std::size_t machine, const std::size_t i) const { return ramps_[machine][i]; }
    virtual const double load(const std::size_t period) const { return loads_[period]; }

    // No loss, so all of them are minimum of double.
    virtual const double B2(const std::size_t i, const std::size_t j) const { return std::numeric_limits<double>::min(); }
    virtual const double B1(const std::size_t i) const { return std::numeric_limits<double>::min(); }
    virtual const double B0() const { return std::numeric_limits<double>::min(); }

    // Settings of problem.
    virtual size_t numPeriods() const { return numPeriods_; }
    virtual size_t numMachines() const { return numMachines_; }
    virtual size_t numVariables() const { return numMachines_ * numPeriods_; }

    // Read the setting file.
    virtual bool Read(const std::string& fname);

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;
    virtual bool Evaluate(std::vector<double>& objs, const std::vector<double>& powers) const;
    virtual void Check(Individual& ind, const double threshold) const;

protected:
    std::vector<std::vector<double>> limits_;
    std::vector<std::vector<double>> coeffs_;
    std::vector<std::vector<double>> ramps_;
    std::vector<double> loads_;

    std::size_t numMachines_ = 0;
    std::size_t numPeriods_ = 0;

    // Set the parameters from file.
    bool SetCoeff(std::ifstream& file);
    bool SetLoad(std::ifstream& file);
};
#endif