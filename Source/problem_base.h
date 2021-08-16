#ifndef PROB_BASE__
#define PROB_BASE__

#include <string>

class BaseProblem
{
public:
    explicit BaseProblem(std::string& name) : name_(name) {}
    ~BaseProblem() {}

    virtual void read(std::string&) = 0;

protected:
    std::string name_;

};

#endif