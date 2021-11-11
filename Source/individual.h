#ifndef INDIVIDUAL__
#define INDIVIDUAL__

#include <vector>
#include <iostream>

#include "problem_base.h"

class Individual
{
public:
    explicit Individual() {}
    ~Individual() {}

    const std::vector<double>& encoding() const { return encoding_; }
    std::vector<double>& encoding() { return encoding_; }

    const std::vector<double>& objs() const { return objectives_; }
    std::vector<double>& objs() { return objectives_; }

    static void SetProblem(const BProblem& prob) { problem_ = &prob; }
    static const BProblem& prob() { return *problem_; }
    
    const double operator[](const int i) const { return encoding_[i]; }
    double& operator[](const int i) { return encoding_[i]; }

private:
    std::vector<double> encoding_;
    std::vector<double> objectives_;

    static const BProblem* problem_;
};

std::ostream& operator<<(std::ostream&, const Individual&);
#endif