#ifndef PROB_MW3__
#define PROB_MW3__

#include "problem_test.h"

class Individual;

class MW3 : public TProblem
{
public:
    explicit MW3() : TProblem("MW3") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW3() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C1(const Individual& ind) const;
    double C2(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif