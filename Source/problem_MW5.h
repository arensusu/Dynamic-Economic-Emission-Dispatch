#ifndef PROB_MW5__
#define PROB_MW5__

#include "problem_test.h"

class Individual;

class MW5 : public TProblem
{
public:
    explicit MW5() : TProblem("MW5") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW5() {}

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