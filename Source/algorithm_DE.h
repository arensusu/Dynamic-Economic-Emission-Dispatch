#ifndef ALGO_DE__
#define ALGO_DE__

#include "algorithm_base.h"

class DE : public BaseEA
{
public:
    explicit DE() : BaseEA("DE"), maxffe_(1000), Psize_(20), F_(0.5), CR_(0.5) {}
    ~DE() {}

    virtual bool Setup(std::ifstream& file);

    virtual void Solve (Population& sol, const BProblem& prob, Log& log);

    double& F() { return F_; }
    double& CR() { return CR_; }

    void Adaptive(const int ffe);

private:
    int maxffe_;
    double Fmin_ = 0.2, Fmax_ = 0.8;
    double CRmin_ = 0.2, CRmax_ = 0.8;
    size_t Psize_;
    double F_;
    double CR_;

};

#endif