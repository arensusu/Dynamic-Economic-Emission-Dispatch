#ifndef INDIVIDUAL__
#define INDIVIDUAL__

#include <vector>
#include <iostream>

#include "problem_base.h"

class Individual
{
public:
    explicit Individual() { encoding_.resize(problem_->numVariables(), -1.0); objectives_.resize(problem_->numObjectives()); modifiedObjs_.resize(problem_->numObjectives()); }
    ~Individual() {}

    // Access the private data.
    const std::vector<double>& encoding() const { return encoding_; }
    std::vector<double>& encoding() { return encoding_; }

    const std::vector<double>& objs() const { return objectives_; }
    std::vector<double>& objs() { return objectives_; }

    const std::vector<double>& violations() const { return violations_; }
    std::vector<double>& violations() { return violations_; }

    const bool feasible() const { return feasible_; }
    bool& feasible() { return feasible_; }
    
    const double violation() const { return totalViolation_; }
    double& violation() { return totalViolation_; }

    const double operator[](const int i) const { return encoding_[i]; }
    double& operator[](const int i) { return encoding_[i]; }

    bool operator==(const Individual& ind) const { return (this->objs()[0] == ind.objs()[0]) && (this->objs()[1] == ind.objs()[1]) && (this->violation() == ind.violation()); }

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

    // Problem of all individuals.
    static void SetProblem(const BProblem& prob) { problem_ = &prob; }
    static const BProblem& prob() { return *problem_; }

    const double F() const { return F_; }
    double& F() { return F_; }

    const double CR() const { return CR_; }
    double& CR() { return CR_; }

    const int op() const { return op_; }
    int& op() { return op_; }

    const double fitness() const { return fitness_; }
    double& fitness() { return fitness_; }

    const std::vector<double>& modifiedObjs() const { return modifiedObjs_; }
    std::vector<double>& modifiedObjs() { return modifiedObjs_; }

protected:
    std::vector<double> encoding_;
    std::vector<double> objectives_;
    std::vector<double> violations_;
    std::vector<double> modifiedObjs_;

    bool feasible_ = false;

    double totalViolation_ = 0.0;

    int op_ = -1;

    // Self-adaptive.
    double F_ = -1;
    double CR_ = -1;

    // SPEA2.
    double fitness_ = -1;

    static const BProblem* problem_;
};

// Print the individuals.
std::ostream& operator<<(std::ostream& os, const Individual& ind);

#endif