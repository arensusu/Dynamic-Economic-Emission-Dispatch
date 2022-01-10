
#include <chrono>
#include <random>

#include "alg_crossover.h"
#include "individual.h"
#include "population.h"

using namespace std;

void BinaryCrossover::operator()(const Individual& target, Individual& mutant, const double CR) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();

    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < CR)
        {
            mutant[i] = mutant[i];
        }
        else
        {
            mutant[i] = target[i];
        }
    }
}

void BinaryCrossover::operator()(Population& pop) const
{
    size_t Psize = pop.size() / 2;
    for (size_t i = 0; i < Psize; ++i)
    {
        (*this)(pop[i], pop[Psize + i], pop[i].CR());
    }
}

void BinaryCrossover::operator()(Population& pop, const double CR) const
{
    size_t Psize = pop.size() / 2;
    for (size_t i = 0; i < Psize; ++i)
    {
        (*this)(pop[i], pop[Psize + i], CR);
    }
}