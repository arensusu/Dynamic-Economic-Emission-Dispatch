#ifndef JDE__
#define JDE__

#include "algorithm_base.h"

class Individual;

class JDE : public BaseEA
{
public:
    explicit JDE() : BaseEA("jDE"), maxffe_(1000), Psize_(20), Fmin_(0.0), Fmax_(0.0), tau1_(0.0), tau2_(0.0) {}
    ~JDE() {}

    virtual bool Setup(std::ifstream& file);

    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

    void SelfInitialization(Population& pop) const;
    void SelfInitialization(Individual& ind) const;

    void SelfAdaptive(Population& pop) const;
    void SelfAdaptive(const Individual& parent, Individual& offspring) const;

private:
    int maxffe_;
    size_t Psize_;

    double Fmin_;
    double Fmax_;

    double tau1_;
    double tau2_;

};

#endif