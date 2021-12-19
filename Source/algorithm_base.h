#ifndef ALGO_BASE__
#define ALGO_BASE__

#include <string>
#include <fstream>

class Population;
class BProblem;
class Log;

class BaseEA
{
public:
    explicit BaseEA(std::string name) : name_(name) {}
    ~BaseEA() {}

    virtual bool Setup(std::ifstream& file) = 0;

    virtual void Solve(Population& sol, const BProblem& prob, Log& log) = 0;

protected:
    std::string name_;

};

#endif