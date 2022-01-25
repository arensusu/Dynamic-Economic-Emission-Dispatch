#ifndef NSDE__
#define NSDE__

#include "algorithm_DE.h"

class NSDE : public DE
{
public:
    explicit NSDE() : DE("NSDE") {}
    ~NSDE() {}

    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

};


#endif
