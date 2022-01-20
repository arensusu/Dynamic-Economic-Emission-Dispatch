#include <iostream>

#include "individual.h"

using namespace std;

// Initialize the problem of individuals.
const BProblem* Individual::problem_ = 0;

// Encode the specific period.
void Individual::Encoder(const size_t t, const vector<double>& powers)
{
    // Parameters.
    size_t numMachines = problem_->numMachines();

    // Normalize.
    for (size_t i = 0; i < numMachines; ++i)
    {
        const double Pmin = problem_->limit(i, 0);
        const double Pmax = problem_->limit(i, 1);

        encoding_[t * numMachines + i] = (powers[i] - Pmin) / (Pmax - Pmin);
    }
}

// Encode all periods.
void Individual::Encoder(const vector<double>& powers)
{
    // Parameters.
    size_t numPeriods = problem_->numPeriods();
    size_t numMachines = problem_->numMachines();

    // Normalize.
    for (size_t t = 0; t < numPeriods; ++t)
    {
        vector<double> period(powers.begin() + t * numMachines, powers.begin() + (t + 1) * numMachines);

        Encoder(t, period);
    }
}

// Decode the specific period.
vector<double> Individual::Decoder(const size_t t) const
{
    // Parameters.
    size_t numMachines = problem_->numMachines();

    // Denormalize.
    vector<double> powers(numMachines);
    for (size_t i = 0; i < numMachines; ++i)
    {
        const double Pmin = problem_->limit(i, 0);
        const double Pmax = problem_->limit(i, 1);

        powers[i] = Pmin + encoding_[t * numMachines + i] * (Pmax - Pmin);
    }

    return powers;
}

// Decode all periods.
vector<double> Individual::Decoder() const
{
    // Parameters.
    size_t numPeriods = problem_->numPeriods();
    size_t numMachines = problem_->numMachines();
    size_t numVariables = numPeriods * numMachines;

    // Denormalize.
    vector<double> powers(numVariables);
    for (size_t t = 0; t < numPeriods; ++t)
    {
        vector<double> power_t = Decoder(t);
        for (size_t i = 0; i < numMachines; ++i)
        {
            powers[t * numMachines + i] = power_t[i];
        }
    }

    return powers;
}

// Get the output of the specific vector.
const double Individual::PowerOutput(const std::vector<double>& powers)
{
    // Calculate the supply.
    double supply = 0;
    for (size_t i = 0; i < powers.size(); ++i)
    {
        supply += powers[i];
    }

    double loss = 0;

    // Calculate the transmission loss.
    if (problem_->B2(0, 0) != numeric_limits<double>::min())
    {
        // Quadratic term.
        for (size_t i = 0; i < powers.size(); ++i)
        {
            for (size_t j = 0; j < powers.size(); ++j)
            {
                loss += powers[i] * problem_->B2(i, j) * powers[j];
            }
        }
    }

    if (problem_->B0() != numeric_limits<double>::min())
    {
        // Linear term.
        for (size_t i = 0; i < powers.size(); ++i)
        {
            loss += powers[i] * problem_->B1(i);
        }

        // Constant.
        loss += problem_->B0();
    }

    supply -= loss;

    return supply;
}

// Get the output of the specific period.
const double Individual::PowerOutput(const size_t t) const
{
    const vector<double> powers = Decoder(t);

    return PowerOutput(powers);
}

// Get the output of all periods.
const vector<double> Individual::PowerOutput() const
{
    // Parameters.
    size_t numPeriods = problem_->numPeriods();

    // Calculate the supplies.
    vector<double> supplies(numPeriods);
    for (size_t t = 0; t < numPeriods; ++t)
    {
        supplies[t] = PowerOutput(t);
    }

    return supplies;
}

// Check with the threshold.
bool Individual::Check(const double threshold)
{
    // Parameters.
    size_t numPeriods = problem_->numPeriods();

    bool isFeasible = true;

    // Calculate the gap.
    for (size_t t = 0; t < numPeriods; ++t)
    {
        const double supply = PowerOutput(t);
        const double demand = problem_->load(t);

        if (abs(supply - demand) > threshold)
        {
            isFeasible = false;
            break;
        }
    }

    feasible_ = isFeasible;

    return isFeasible;
}

// Print.
ostream& operator<<(ostream& os, const Individual& ind)
{
    //os << "Objectives: " << ind.objs()[0] << ", " << ind.objs()[1] << endl;
    os << ind.objs()[0] << " " << ind.objs()[1];
    //os << "Feasible: " << ind.Check();

    vector<double> powers = ind.Decoder();

    for (size_t i = 0; i < powers.size(); ++i)
    {
        os << " " << powers[i];
    }

    //if (ind.F() != -1 || ind.CR() != -1)
    //{
    //    os << " " << ind.F() << " " << ind.CR();
    //}

    os << endl;

    return os;
}