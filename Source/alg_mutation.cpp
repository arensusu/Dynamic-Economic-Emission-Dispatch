
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

using namespace std;


void RandOneMutation::operator()(Population& pop, const size_t pos, const double F) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();
    size_t Psize = pop.size() / 2;

    // Get random individuals.
    vector<int> index(Psize);
    for (size_t i = 0; i < Psize; ++i)
    {
        index[i] = i;
    }

    shuffle(index.begin(), index.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));

    const Individual& r1 = pop[index[0]];
    const Individual& r2 = pop[index[1]];
    const Individual& r3 = pop[index[2]];

    // Create mutant vector.
    Individual& mutant = pop[Psize + pos];
    for (size_t i = 0; i < numVariables; ++i)
    {
        mutant[i] = r1[i] + F * (r2[i] - r3[i]);
    }
}

void RandOneMutation::operator()(Population& pop, const double F) const
{
    for (size_t i = 0; i < pop.size() / 2; ++i)
    {
        (*this)(pop, i, F);
    }
}

void RandOneMutation::operator()(Population& pop) const
{
    for (size_t i = 0; i < pop.size() / 2; ++i)
    {
        (*this)(pop, i, pop[i].F());
    }
}

void RandOneMutation::operator()(Individual& mutant, const Population& rands, const double F) const
{
    size_t numVariables = Individual::prob().numVariables();

    for (size_t i = 0; i < numVariables; ++i)
    {
        mutant[i] = rands[0][i] + F * (rands[1][i] - rands[2][i]);
    }
}

void RandOneMutation::operator()(Population& mutants,
                                 const Population& parent,
                                 const vector<vector<size_t>>& neighborhood,
                                 const double F) const
{
    for (size_t i = 0; i < mutants.size(); ++i)
    {
        (*this)(mutants[i], parent, neighborhood[i], F);
    }
}

void RandOneMutation::operator()(Individual& mutant, const Population& parent, const vector<size_t> neighborhood, const double F) const
{
    RandomMatingSelection matingSelect;

    vector<size_t> index = matingSelect(neighborhood, 3);

    Population rands;
    for (size_t i = 0; i < index.size(); ++i)
    {
        rands.push_back(parent[index[i]]);
    }

    (*this)(mutant, rands, F);
}

void BestOneMutation::operator() (Population& pop, const double F) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();
    size_t Psize = pop.size() / 2;

    Population parent(pop.begin(), pop.begin() + Psize);
    vector<vector<size_t>> fronts = NondominatedSort(parent);

    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<size_t> dis(0, fronts[0].size() - 1);

    for (size_t pos = 0; pos < Psize; ++pos)
    {
        // Get random individuals.
        vector<int> index(Psize);
        for (size_t i = 0; i < Psize; ++i)
        {
            index[i] = i;
        }
        shuffle(index.begin(), index.end(), gen);

        const Individual& r2 = pop[index[0]];
        const Individual& r3 = pop[index[1]];

        // Get best individual.
        const Individual& best = pop[fronts[0][dis(gen)]];

        // Create mutant vector.
        const Individual& curr = pop[pos];
        Individual& mutant = pop[Psize + pos];
        for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
        {
            mutant[i] = best[i] + F * (r2[i] - r3[i]);
        }
    }
}

void BestOneMutation::operator() (Population& pop) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();
    size_t Psize = pop.size() / 2;

    Population parent(pop.begin(), pop.begin() + Psize);
    vector<vector<size_t>> fronts = NondominatedSort(parent);

    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<size_t> dis(0, fronts[0].size() - 1);

    for (size_t pos = 0; pos < Psize; ++pos)
    {
        // Get random individuals.
        vector<int> index(Psize);
        for (size_t i = 0; i < Psize; ++i)
        {
            index[i] = i;
        }
        shuffle(index.begin(), index.end(), gen);

        const Individual& r2 = pop[index[0]];
        const Individual& r3 = pop[index[1]];

        // Get best individual.
        const Individual& best = pop[fronts[0][dis(gen)]];

        // Create mutant vector.
        const Individual& curr = pop[pos];
        Individual& mutant = pop[Psize + pos];
        for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
        {
            mutant[i] = best[i] + curr.F() * (r2[i] - r3[i]);
        }
    }
}

void CurrentToBestMutation::operator()(Population& pop, const double F) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();
    size_t Psize = pop.size() / 2;

    Population parent(pop.begin(), pop.begin() + Psize);
    vector<vector<size_t>> fronts = NondominatedSort(parent);

    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<size_t> dis(0, fronts[0].size() - 1);

    for (size_t pos = 0; pos < Psize; ++pos)
    {
        // Get random individuals.
        vector<int> index(Psize);
        for (size_t i = 0; i < Psize; ++i)
        {
            index[i] = i;
        }
        shuffle(index.begin(), index.end(), gen);

        const Individual& r2 = pop[index[0]];
        const Individual& r3 = pop[index[1]];

        // Get best individual.
        const Individual& best = pop[fronts[0][dis(gen)]];

        // Create mutant vector.
        const Individual& curr = pop[pos];
        Individual& mutant = pop[Psize + pos];
        for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
        {
            mutant[i] = curr[i] + F * (curr[i] - best[i]) + F * (r2[i] - r3[i]);
        }
    }
}

void CurrentToBestMutation::operator()(Population& pop) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();
    size_t Psize = pop.size() / 2;

    Population parent(pop.begin(), pop.begin() + Psize);
    vector<vector<size_t>> fronts = NondominatedSort(parent);

    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<size_t> dis(0, fronts[0].size() - 1);

    for (size_t pos = 0; pos < Psize; ++pos)
    {
        // Get random individuals.
        vector<int> index(Psize);
        for (size_t i = 0; i < Psize; ++i)
        {
            index[i] = i;
        }
        shuffle(index.begin(), index.end(), gen);

        const Individual& r2 = pop[index[0]];
        const Individual& r3 = pop[index[1]];

        // Get best individual.
        const Individual& best = pop[fronts[0][dis(gen)]];
        //Compromise comp;
        //const Individual& best = pop[comp(pop)];

        // Create mutant vector.
        const Individual& curr = pop[pos];
        Individual& mutant = pop[Psize + pos];
        for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
        {
            mutant[i] = curr[i] + curr.F() * (curr[i] - best[i]) + curr.F() * (r2[i] - r3[i]);
        }
    }
}