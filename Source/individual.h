#ifndef INDIVIDUAL__
#define INDIVIDUAL__

#include <vector>

#include "problem_base.h"

class Individual
{
public:
    explicit Individual() {}
    ~Individual() {}

    const std::vector<double>& encoding() const { return encoding_; }
    std::vector<double>& encoding() { return encoding_; }

    const double objective(const int i) const { return objective_[i]; }
    double& objective(const int i) { return objective_[i]; }

    static void SetProblem(const BProblem& prob) { problem_ = &prob; }
    static const BProblem& prob() { return *problem_; }
    
private:
    std::vector<double> encoding_;
    std::vector<double> objective_;

    static const BProblem* problem_;
};

#endif