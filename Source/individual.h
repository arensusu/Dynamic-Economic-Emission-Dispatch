#ifndef INDIVIDUAL__
#define INDIVIDUAL__

#include <vector>
#include <iostream>

#include "problem_base.h"

class Individual
{
public:
    explicit Individual() { encoding_.resize(problem_->numVariables()); objectives_.resize(problem_->numObjectives()); }
    ~Individual() {}

    // Access the private data.
    const std::vector<double>& encoding() const { return encoding_; }
    std::vector<double>& encoding() { return encoding_; }

    const std::vector<double>& objs() const { return objectives_; }
    std::vector<double>& objs() { return objectives_; }

    const double operator[](const int i) const { return encoding_[i]; }
    double& operator[](const int i) { return encoding_[i]; }

    // Normalize to [0, 1].
    void Encoder(const std::size_t t, const std::vector<double>& powers);
    void Encoder(const std::vector<double>& powers);

    // Denormalize.
    std::vector<double> Decoder(const std::size_t t) const;
    std::vector<double> Decoder() const;

    // Sum the Output.
    static const double PowerOutput(const std::vector<double>& powers);
    const double PowerOutput(const std::size_t t) const;
    const std::vector<double> PowerOutput() const;

    // Check the individual is feasible or not.
    bool Check(const double threshold) const;

    // Problem of all individuals.
    static void SetProblem(const BProblem& prob) { problem_ = &prob; }
    static const BProblem& prob() { return *problem_; }

private:
    std::vector<double> encoding_;
    std::vector<double> objectives_;

    static const BProblem* problem_;
};

// Print the individuals.
std::ostream& operator<<(std::ostream& os, const Individual& ind);

#endif