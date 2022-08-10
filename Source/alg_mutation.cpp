
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>

#include "alg_mutation.h"
#include "alg_mating_selection.h"
#include "population.h"
#include "individual.h"
#include "alg_sorting.h"
#include "indicator.h"

#include "tool.h"

using namespace std;

// mutation.

void Mutation::AllNeigh(Individual& mutant, const Population& parents, const vector<double> params) const
{
    vector<size_t> index(parents.size());
    Permutation(index);

    (*this).PartNeigh(mutant, parents, index, params);
}

void Mutation::PartNeigh(Individual& mutant, const Population& parents, const vector<size_t> neighborhood, const vector<double> params) const
{
    RandomMatingSelection matingSelect;

    vector<size_t> index = matingSelect(neighborhood, nums_);

    vector<Individual> rands(nums_);
    for (size_t i = 0; i < nums_; ++i)
    {
        rands[i] = parents[index[i]];
    }

    (*this)(mutant, rands, params);
}

//------------------------------------------------------------------------------------------------------------------
// rand/1.
void RandOneMutation::operator()(Individual& mutant, const vector<Individual>& rands, const vector<double> params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < params[1])
        {
            mutant[i] = rands[0][i] + params[0] * (rands[1][i] - rands[2][i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// rand/2.

void RandTwoMutation::operator()(Individual& mutant, const vector<Individual>& rands, const vector<double> params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < params[1])
        {
            mutant[i] = rands[0][i] + params[0] * (rands[1][i] - rands[2][i] + rands[3][i] - rands[4][i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// curr-to-rand/1.

void CurrToRandOneMutation::operator()(Individual& mutant, const vector<Individual>& rands, const vector<double>params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < params[1])
        {
            mutant[i] = mutant[i] + params[0] * (mutant[i] - rands[0][i]) + params[0] * (rands[1][i] - rands[2][i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// curr-to-rand/2.

void CurrToRandTwoMutation::operator()(Individual& mutant, const vector<Individual>& rands, const vector<double>params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < params[1])
        {
            mutant[i] = mutant[i] + params[0] * (mutant[i] - rands[0][i]) + params[0] * (rands[1][i] - rands[2][i] + rands[3][i] - rands[4][i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// SBX.

void SBXMutation::operator()(Individual& child, const vector<Individual>& rands, const vector<double> params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        double mu = dis(gen);
        double beta;
        if (mu <= 0.5)
        {
            beta = pow(2 * mu, 1.0 / (params[0] + 1));
        }
        else
        {
            beta = pow(2 * (1 - mu), -1.0 / (params[0] + 1));
        }

        child[i] = 0.5 * ((1 + beta) * rands[0][i] + (1 - beta) * rands[1][i]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// NonLinear.

void NonLinearMutation::operator()(Individual& child, const vector<Individual>& rands, const vector<double> params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        double r1 = dis(gen), r2 = dis(gen);

        child[i] = rands[0][i] + r1 * (1 - pow(r2, -1.0 * pow(1 - params[0], 0.7))) * (rands[0][i] - rands[1][i]);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// Current-to-best/1.

void CurrToBestOneMutation::PartNeigh(Individual& mutant, const Population& parents, const vector<size_t> neighborhood, const vector<double> params) const
{
    RandomMatingSelection matingSelect;

    vector<size_t> index = matingSelect(neighborhood, neighborhood.size());

    vector<Individual> rands(nums_ + 1);

    rands[0] = parents[size_t(params[2])];

    size_t j = 0;
    for (size_t i = 0; i < nums_; ++i)
    {
        while (index[j] == params[2])
        {
            j++;
        }

        rands[i + 1] = parents[index[j]];
        j++;
    }

    (*this)(mutant, rands, params);
}

void CurrToBestOneMutation::operator()(Individual& mutant, const vector<Individual>& rands, const vector<double>params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < params[1])
        {
            mutant[i] = mutant[i] + params[0] * (rands[0][i] - mutant[i]) + params[0] * (rands[1][i] - rands[2][i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// Best/1.

void BestOneMutation::PartNeigh(Individual& mutant, const Population& parents, const vector<size_t> neighborhood, const vector<double> params) const
{
    RandomMatingSelection matingSelect;

    vector<size_t> index = matingSelect(neighborhood, neighborhood.size());

    vector<Individual> rands(nums_ + 1);

    rands[0] = parents[size_t(params[2])];

    size_t j = 0;
    for (size_t i = 0; i < nums_; ++i)
    {
        while (index[j] == params[2])
        {
            j++;
        }

        rands[i + 1] = parents[index[j]];
        j++;
    }

    (*this)(mutant, rands, params);
}

void BestOneMutation::operator()(Individual& mutant, const vector<Individual>& rands, const vector<double>params) const
{
    size_t numVariables = Individual::prob().numVariables();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < params[1])
        {
            mutant[i] = rands[0][i] + params[0] * (rands[1][i] - rands[2][i]);
        }
    }
}
