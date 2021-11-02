#ifndef NOLOSS__
#define NOLOSS__

#include <vector>
#include <fstream>

#include "problem_base.h"

class NProblem : public BProblem
{
public:
    explicit NProblem(const int m, const int p, const int o) : BProblem("NoLoss"), numMachines_(m), numPeriods_(p), numObjectives_(o) {}
    NProblem(const int m, const int p, const int o, const std::string name) : BProblem(name), numMachines_(m), numPeriods_(p), numObjectives_(o) {}
    ~NProblem() {}

    const double coeff(const int machine, const int coeffIndex) const { return coeff_[machine][coeffIndex]; }

    const double load(const int period) const { return load_[period]; }

    virtual int numVariables() const { return numMachines_ * numPeriods_; }
    virtual int numObjectives() const { return numObjectives_; }

    virtual bool Read(const std::string&);
    virtual bool Evaluate(Individual&) const;
    
protected:
    std::vector<std::vector<double>> coeff_;
    std::vector<double> load_;

    int numMachines_;
    int numPeriods_;
    int numObjectives_;

    bool SetCoeff(std::ifstream&, const int);
    bool SetLoad(std::ifstream&, const int);
};
#endif