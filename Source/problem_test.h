#ifndef PROB_TEST__
#define PROB_TEST__

#include <iostream>

#include "problem_base.h"

class Individual;

class TProblem : public BProblem
{
public:
    explicit TProblem(const std::string name) : BProblem(name) { }
    virtual ~TProblem() {}

    // Basic settings of generators.
    virtual const double limit(const std::size_t machine, const std::size_t i) const { Alert(); return 0; }
    virtual const double coeff(const std::size_t machine, const std::size_t coeffIndex) const { Alert(); return 0; }
    virtual const double ramp(const std::size_t machine, const std::size_t i) const { Alert(); return 0; }
    virtual const double load(const std::size_t period) const { Alert(); return 0; }

    // Coefficients of Transmission loss.
    virtual const double B2(const std::size_t i, const std::size_t j) const { Alert(); return 0; }
    virtual const double B1(const std::size_t i) const { Alert(); return 0; }
    virtual const double B0() const { Alert(); return 0; }

    // Settings of problem.
    virtual size_t numPeriods() const { Alert(); return 0; }
    virtual size_t numMachines() const { Alert(); return 0; }
    virtual size_t numVariables() const { return numVariables_; }

    // Read the setting file.
    virtual bool Read(const std::string& fname) { Alert(); return 0; }

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const = 0;
    virtual void Check(Individual& ind, const double threshold) const;

    // No implement.
    void Alert() const { std::cout << "No implement." << std::endl; }

protected:
    std::size_t numVariables_ = 0;
};

#endif