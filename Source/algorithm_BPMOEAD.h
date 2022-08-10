#ifndef BPMOEAD__
#define BPMOEAD__

#include <vector>
#include <list>

#include "algorithm_MOEAD-DE.h"

class Individual;

class BPMOEAD : public MOEADDE
{
public:
    // Basic of MOEA/D.
    explicit BPMOEAD() : MOEADDE()
    {
        opCounts_.resize(numOperators_, 0);
    }

    ~BPMOEAD() {}

    virtual bool Setup(std::ifstream& file);
    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

    bool UpdateNeighbor(Population& pop, const Individual& ind, const std::size_t index, Log& log);

    std::size_t FindtheBest(const std::size_t index, const Population& pop);

private:
    double eta_ = 20.0;

    std::size_t numOperators_ = 2;
    std::vector<int> opCounts_;

};

#endif