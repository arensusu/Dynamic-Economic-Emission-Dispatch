#ifndef NOLOSS__
#define NOLOSS__

#include <vector>
#include <fstream>

#include "problem_base.h"

class NProblem : public BProblem
{
public:
    explicit NProblem() { setName("NoLoss"); }
    ~NProblem() {}

    const double coeff(const int machine, const int coeffIndex) const { return coeff_[machine][coeffIndex]; }
    
    const double load(const int period) const { return load_[period]; }

    virtual int read(const std::string&);

protected:
    std::vector<std::vector<double>> coeff_;
    std::vector<double> load_;

    void setCoeff(std::ifstream&, const int, const int);
    void setLoad(std::ifstream&, const int);
};

#endif