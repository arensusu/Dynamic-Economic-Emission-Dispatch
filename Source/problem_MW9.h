#ifndef PROB_MW9__
#define PROB_MW9__

#include "problem_test.h"

class Individual;

class MW9 : public TProblem
{
public:
    explicit MW9() : TProblem("MW9") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW9() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif