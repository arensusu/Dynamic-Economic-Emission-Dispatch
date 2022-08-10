#ifndef PROB_MW1__
#define PROB_MW1__

#include "problem_test.h"

class Individual;

class MW1 : public TProblem
{
public:
    explicit MW1() : TProblem("MW1") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW1() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif