#ifndef WOB__
#define WOB__

#include "problem_noloss.h"

class WOBProblem : public NProblem
{
public:
    explicit WOBProblem(const int m, const int p, const int o, const std::string &name = "WithoutB0") : NProblem(m, p, o, name) {}
    ~WOBProblem() {}

    virtual const double B2(const int i, const int j) const { return B2_[i][j]; }

    virtual bool Read(const std::string&);

private:
    std::vector<std::vector<double>> B2_;

    virtual bool SetB(std::ifstream&);
};

#endif