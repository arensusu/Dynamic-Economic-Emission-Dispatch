#ifndef PROB_BASE__
#define PROB_BASE__

#include <string>

class Individual;

class BProblem
{
public:
    explicit BProblem(const std::string name) : name_(name) { }
    ~BProblem() {}

    void SetName(const std::string& name) { name_ = name; }

    virtual int numVariables() const = 0;
    virtual int numObjectives() const = 0;

    virtual bool Read(const std::string&) = 0;
    virtual bool Evaluate(Individual&) const = 0;

protected:
    std::string name_;

};

#endif