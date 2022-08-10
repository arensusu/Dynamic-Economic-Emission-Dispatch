#ifndef PROB_MW2__
#define PROB_MW2__

#include "problem_test.h"

class Individual;

class MW2 : public TProblem
{
public:
    explicit MW2() : TProblem("MW2") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW2() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif