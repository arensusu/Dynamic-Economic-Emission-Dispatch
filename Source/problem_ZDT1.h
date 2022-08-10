#ifndef PROB_ZDT1__
#define PROB_ZDT1__

#include "problem_test.h"

class Individual;

class ZDT1 : public TProblem
{
public:
    explicit ZDT1() : TProblem("ZDT1") { numObjectives_ = 2; numVariables_ = 240; }
    virtual ~ZDT1() {}

    // Functions.
    double F1(const Individual& ind) const;
    double F2(const Individual& ind) const;

    // Evaluate the individual.
    virtual bool Evaluate(Individual& ind) const;

};

#endif