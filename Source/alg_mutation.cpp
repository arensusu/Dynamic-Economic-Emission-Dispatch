
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>

#include "alg_mutation.h"
#include "population.h"
#include "individual.h"
#include "indicator.h"

using namespace std;


void RandOneMutation::operator()(Population& pop, const double F, const size_t pos) const
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

void BestOneMutation::operator() (Population& pop, const double F, const size_t pos) const
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

    const Individual& r2 = pop[index[0]];
    const Individual& r3 = pop[index[1]];

    // Get best(compromise) individual.
    Compromise comp;
    const Individual& best = pop[comp(pop)];

    // Create mutant vector.
    Individual& mutant = pop[Psize + pos];
    for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
    {
        mutant[i] = best[i] + F * (r2[i] - r3[i]);
    }

    return;
}

void CurrentToBestMutation::operator()(Population& pop, const double F, const size_t pos) const
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

    const Individual& r2 = pop[index[0]];
    const Individual& r3 = pop[index[1]];

    // Get best individual.
    Compromise comp;
    const Individual& best = pop[comp(pop)];

    // Create mutant vector.
    Individual& mutant = pop[Psize + pos];
    for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
    {
        mutant[i] = mutant[i] + F * (mutant[i] - best[i]) + F * (r2[i] - r3[i]);
    }
}