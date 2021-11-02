#ifndef ALGO_DE__
#define ALGO_DE__

#include <fstream>

#include "algorithm_base.h"

class DE : public BaseEA
{
public:
    explicit DE() : BaseEA("DE"), maxffe_(1000), Psize_(20), F_(0.5), CR_(0.5) {}
    ~DE() {}

    virtual bool Setup(std::ifstream&);

    virtual void Solve (Population&, const BProblem&);

private:
    int maxffe_;
    size_t Psize_;
    double F_;
    double CR_;

};

#endif