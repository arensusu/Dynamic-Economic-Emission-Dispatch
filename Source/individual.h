#ifndef INDIVIDUAL__
#define INDIVIDUAL__

#include <vector>

class BaseProblem;

class Individual
{
public:
    explicit Individual() {}
    ~Individual() {}

private:
    std::vector<double> chromo_;
    double objective_;

    static BaseProblem* prob;
};

#endif