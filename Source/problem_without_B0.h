#ifndef WOB__
#define WOB__

#include "problem_noloss.h"

class WOBProblem : public NProblem
{
public:
    explicit WOBProblem(const std::string &name = "WithoutB0") : NProblem(name) {}
    virtual ~WOBProblem() {}

    virtual const double B2(const std::size_t i, const std::size_t j) const { return B2_[i][j]; }

    virtual bool Read(const std::string& fname);

private:
    std::vector<std::vector<double>> B2_;

    virtual bool SetB(std::ifstream& file);
};

#endif