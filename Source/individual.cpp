#include <iostream>

#include "individual.h"

using namespace std;

const BProblem* Individual::problem_ = 0;

void Individual::Encoder(const size_t t, const vector<double>& powers)
{
    for (size_t i = 0; i < problem_->numMachines(); ++i)
    {
        encoding_[t * problem_->numMachines() + i] = (powers[i] - problem_->limit(i, 0)) / (problem_->limit(i, 1) - problem_->limit(i, 0));
    }
}

void Individual::Encoder(const vector<double>& powers)
{
    for (size_t t = 0; t < problem_->numPeriods(); ++t)
    {
        Encoder(t, vector<double>(powers.begin() + t * problem_->numMachines(), powers.begin() + (t + 1) * problem_->numMachines()));
    }
}

vector<double> Individual::Decoder(const size_t t) const
{
    const BProblem& prob = Individual::prob();
    vector<double> powers(prob.numMachines());

    for (size_t j = 0; j < prob.numMachines(); ++j)
    {
        powers[j] = prob.limit(j, 0) + encoding_[t * prob.numMachines() + j] * (prob.limit(j, 1) - prob.limit(j, 0));
    }

    return powers;
}

vector<double> Individual::Decoder() const
{
    const BProblem& prob = Individual::prob();
    vector<double> powers(prob.numVariables());

    for (size_t t = 0; t < prob.numPeriods(); ++t)
    {
        vector<double> power_t = Decoder(t);
        for (size_t i = 0; i < prob.numMachines(); ++i)
        {
            powers[t * prob.numMachines() + i] = power_t[i];
        }
    }

    return powers;
}

const double Individual::PowerOutput(const size_t t) const
{
    const vector<double> powers = Decoder(t);

    double provide = 0;
    for (size_t i = 0; i < powers.size(); ++i)
    {
        provide += powers[i];
    }

    if (problem_->B2(0, 0) != numeric_limits<double>::min())
    {
        double loss = 0;
        for (size_t i = 0; i < powers.size(); ++i)
        {
            for (size_t j = 0; j < powers.size(); ++j)
            {
                loss += powers[i] * problem_->B2(i, j) * powers[j];
            }
        }

        if (problem_->B0() != numeric_limits<double>::min())
        {
            for (size_t i = 0; i < powers.size(); ++i)
            {
                loss += powers[i] * problem_->B1(i);
            }
        }

        provide -= loss;
    }

    return provide;
}

const vector<double> Individual::PowerOutput() const
{
    vector<double> provides(problem_->numPeriods());

    for (size_t t = 0; t < problem_->numPeriods(); ++t)
    {
        provides[t] = PowerOutput(t);
    }

    return provides;
}

bool Individual::Check() const
{
    bool isFeasible = true;

    for (size_t t = 0; t < problem_->numPeriods(); ++t)
    {
        if (abs(PowerOutput(t) - problem_->load(t)) > 0.00001)
        {
            isFeasible = false;
        }
    }

    return isFeasible;
}

ostream& operator<<(ostream& os, const Individual& ind)
{
    os << "Objectives: " << ind.objs()[0] << ", " << ind.objs()[1] << endl;

    os << "Feasible: " << ind.Check() << endl;

    vector<double> powers = ind.Decoder();

    for (size_t i = 0; i < powers.size(); ++i)
    {
        os << ", " << powers[i];
    }
    os << endl;

    return os;
}