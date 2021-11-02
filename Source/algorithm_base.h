#ifndef ALGO_BASE__
#define ALGO_BASE__

#include <string>

class Population;
class BProblem;

class BaseEA
{
public:
    explicit BaseEA(std::string name) : name_(name) {}
    ~BaseEA() {}

    virtual bool Setup(const std::string&) = 0;

    virtual void Solve(Population&, const BProblem&) = 0;

protected:
    std::string name_;

};

#endif