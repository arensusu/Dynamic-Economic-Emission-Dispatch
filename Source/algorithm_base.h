#ifndef ALGO_BASE__
#define ALGO_BASE__

#include <string>

class Population;

class BaseEA
{
public:
    explicit BaseEA(std::string name) : name_(name) {}
    ~BaseEA() {}

    virtual void operator() (Population&) = 0;

protected:
    std::string name_;
};

#endif