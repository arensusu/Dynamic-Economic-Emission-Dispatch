#ifndef PROB_MW6__
#define PROB_MW6__

#include "problem_test.h"

class Individual;

class MW6 : public TProblem
{
public:
    explicit MW6() : TProblem("MW6") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW6() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif