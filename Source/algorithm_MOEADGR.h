#ifndef MOEADGR__
#define MOEADGR__

#include <vector>

#include "algorithm_MOEAD-DE.h"

class Individual;

class MOEADGR : public MOEADDE
{
public:
    explicit MOEADGR() : MOEADDE() {}
    ~MOEADGR() {}

    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

    std::size_t FindSubproblem(const Individual& ind) const;

};

#endif