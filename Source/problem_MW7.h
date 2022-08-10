#ifndef PROB_MW7__
#define PROB_MW7__

#include "problem_test.h"

class Individual;

class MW7 : public TProblem
{
public:
    explicit MW7() : TProblem("MW7") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW7() {}

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