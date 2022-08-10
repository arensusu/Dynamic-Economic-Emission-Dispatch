#ifndef NSGAOS__
#define NSGAOS__

#include "algorithm_DE.h"

class NSGAOS : public DE
{
public:
    explicit NSGAOS() : DE("NSGAOS") { F_ = 0.5; CR_ = 0.5; }
    ~NSGAOS() {}

    virtual bool Setup(std::ifstream& file);
    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

    int AOS() const;
    void CreditAssign(const Population& pop);

private:
    double eta_ = 20.0;
    
    std::size_t numOperators_ = 4;
    std::vector<int> opCounts_;
};


#endif
