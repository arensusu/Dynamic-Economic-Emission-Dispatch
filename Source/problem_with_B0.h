#ifndef WB__
#define WB__

#include "problem_without_B0.h"

class WBProblem : public WOBProblem
{
public:
    explicit WBProblem(const std::string& name = "WithB0") : WOBProblem(name), B0_(-1) {}
    virtual ~WBProblem() {}

    virtual const double B2(const std::size_t i, const std::size_t j) const { return B2_[i][j]; }
    virtual const double B1(const std::size_t i) const { return B1_[i]; }
    virtual const double B0() const { return B0_; }

    virtual bool Read(const std::string&);

private:
    std::vector<std::vector<double>> B2_;
    std::vector<double> B1_;
    double B0_;

    bool SetB(std::ifstream&);
};

#endif