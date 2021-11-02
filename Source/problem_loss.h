#ifndef BLOSS__
#define BLOSS__

#include "problem_noloss.h"

class BLProblem : public NProblem
{
public:
    explicit BLProblem() { setName("BLoss"); }
    ~BLProblem() {}

    const double B(const int i, const int j) const { return B_[i][j]; }

    virtual int read(const std::string&);

private:
    std::vector<std::vector<double>> B_;

    void setB(std::ifstream&, const int);
};

#endif