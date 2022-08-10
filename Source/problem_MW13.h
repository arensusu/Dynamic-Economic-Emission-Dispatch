#ifndef PROB_MW13__
#define PROB_MW13__

#include "problem_test.h"

class Individual;

class MW13 : public TProblem
{
public:
    explicit MW13() : TProblem("MW13") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW13() {}

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