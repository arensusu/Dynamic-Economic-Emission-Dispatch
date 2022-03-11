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

    bool feasible() const { return feasible_; }
    
    const double violation() const { return violation_; }
    double& violation() { return violation_; }

    const double operator[](const int i) const { return encoding_[i]; }
    double& operator[](const int i) { return encoding_[i]; }

    bool operator==(const Individual& ind) const { return (this->objs()[0] == ind.objs()[0]) && (this->objs()[1] == ind.objs()[1]); }

    const double F() const { return F_; }
    double& F() { return F_; }

    const double CR() const { return CR_; }
    double& CR() { return CR_; }

    const double fitness() const { return fitness_; }
    double& fitness() { return fitness_; }

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
    bool Check(const double threshold = 0.001);

    // Problem of all individuals.
    static void SetProblem(const BProblem& prob) { problem_ = &prob; }
    static const BProblem& prob() { return *problem_; }


protected:
    std::vector<double> encoding_;
    std::vector<double> objectives_;

    bool feasible_ = false;

    double violation_ = 0.0;

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