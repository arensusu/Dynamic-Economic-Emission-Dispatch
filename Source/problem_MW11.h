#ifndef PROB_MW11__
#define PROB_MW11__

#include "problem_test.h"

class Individual;

class MW11 : public TProblem
{
public:
    explicit MW11() : TProblem("MW11") { numObjectives_ = 2; numVariables_ = 15; }
    virtual ~MW11() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;
    double G(const Individual& ind) const;
    double C1(const Individual& ind) const;
    double C2(const Individual& ind) const;
    double C3(const Individual& ind) const;
    double C4(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif