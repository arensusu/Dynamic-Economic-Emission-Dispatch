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
    //const std::string& name() const { return name_; }

    virtual const double limit(const int machine, const int i) const = 0;
    virtual const double coeff(const int machine, const int coeffIndex) const = 0;
    virtual const double ramp(const int machine, const int i) const = 0;
    virtual const double load(const int period) const = 0;

    virtual const double B2(const int i, const int j) const = 0;
    virtual const double B1(const int i) const = 0;
    virtual const double B0() const = 0;

    virtual int numPeriods() const = 0;
    virtual int numMachines() const = 0;
    virtual int numVariables() const = 0;
    virtual int numObjectives() const = 0;

    virtual bool Read(const std::string&) = 0;
    virtual bool Evaluate(Individual&) const = 0;

protected:
    std::string name_;

};

#endif