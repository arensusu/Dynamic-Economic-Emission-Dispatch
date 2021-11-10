#ifndef WB__
#define WB__

#include "problem_without_B0.h"

class WBProblem : public WOBProblem
{
public:
    explicit WBProblem(const int m, const int p, const int o, const std::string& name = "WithB0") : WOBProblem(m, p, o, name), B0_(-1) {}
    ~WBProblem() {}

    virtual const double B2(const int i, const int j) const { return B2_[i][j]; }
    virtual const double B1(const int i) const { return B1_[i]; }
    virtual const double B0() const { return B0_; }

    virtual bool Read(const std::string&);

private:
    std::vector<std::vector<double>> B2_;
    std::vector<double> B1_;
    double B0_;

    bool SetB(std::ifstream&);
};

#endif