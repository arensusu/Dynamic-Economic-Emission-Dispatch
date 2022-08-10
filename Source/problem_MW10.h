#ifndef PROB_MW10__
#define PROB_MW10__

#include "problem_test.h"

class Individual;

class MW10 : public TProblem
{
public:
    explicit MW10() : TProblem("MW10") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW10() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C1(const Individual& ind) const;
    double C2(const Individual& ind) const;
    double C3(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif