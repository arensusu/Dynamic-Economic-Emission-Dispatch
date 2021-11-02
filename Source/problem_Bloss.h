#ifndef BLOSS__
#define BLOSS__

#include "problem_noloss.h"

class BLProblem : public NProblem
{
public:
    explicit BLProblem(const int m, const int p, const int o) : NProblem(m, p, o, "Loss"), B0_(-1) {}
    ~BLProblem() {}

    const double B2(const int i, const int j) const { return B2_[i][j]; }
    const double B1(const int i) const { return B1_[i]; }
    const double B0() const { return B0_; }

    virtual bool Read(const std::string&);

private:
    std::vector<std::vector<double>> B2_;
    std::vector<double> B1_;
    double B0_;

    bool SetB(std::ifstream&, const int);
};

#endif