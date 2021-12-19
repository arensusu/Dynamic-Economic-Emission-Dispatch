#ifndef PROB_BASE__
#define PROB_BASE__

#include <string>
#include <vector>

class Individual;

class BProblem
{
public:
    explicit BProblem(const std::string name) : name_(name) { }
    virtual ~BProblem() {}

    void SetName(const std::string& name) { name_ = name; }

    // Basic settings of generators.
    virtual const double limit(const std::size_t machine, const std::size_t i) const = 0;
    virtual const double coeff(const std::size_t machine, const std::size_t coeffIndex) const = 0;
    virtual const double ramp(const std::size_t machine, const std::size_t i) const = 0;
    virtual const double load(const std::size_t period) const = 0;
    
    // Coefficients of Transmission loss.
    virtual const double B2(const std::size_t i, const std::size_t j) const = 0;
    virtual const double B1(const std::size_t i) const = 0;
    virtual const double B0() const = 0;

    // Settings of problem.
    virtual size_t numPeriods() const = 0;
    virtual size_t numMachines() const = 0;
    virtual size_t numVariables() const = 0;
    virtual size_t numObjectives() const = 0;

    // Read the setting file.
    virtual bool Read(const std::string& fname) = 0;
    
    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const = 0;
    virtual bool Evaluate(std::vector<double>& objs, const std::vector<double>& powers) const = 0;

protected:
    std::string name_;

};

#endif