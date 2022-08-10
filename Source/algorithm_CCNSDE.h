#ifndef CCNSDE__
#define CCNSDE__

#include "algorithm_DE.h"

class Individual;

class CCNSDE : public DE
{
public:
    explicit CCNSDE() : DE("CC") {}
    ~CCNSDE() {}

    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

private:
    void Combine(Individual& target, const Individual& source) const;
    Individual PartialEvaluate(Individual& target, const Individual& best, const std::size_t index) const;
    void UpdateBestIndv(Individual& target, const std::vector<Population>& subPop) const;
};

#endif