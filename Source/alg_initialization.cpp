
#include <chrono>
#include <random>

#include "alg_initialization.h"
#include "problem_base.h"
#include "population.h"
#include "individual.h"

#include "tool.h"

using namespace std;

void RandomInitialization::operator()(Population& pop) const
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        (*this)(pop[i]);
    }
}

void RandomInitialization::operator()(Individual& ind) const
{
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
    {
        ind[i] = dis(gen);
    }
}

void SubProblemInitialization::operator()(Population& pop, size_t index) const
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        (*this)(pop[i], index);
    }
}

void SubProblemInitialization::operator()(Individual& ind, size_t index) const
{
    size_t numMachines = Individual::prob().numMachines();
    size_t numPeriods = Individual::prob().numPeriods();
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t t = 0; t < numPeriods; ++t)
    {
        if (t == index)
        {
            for (size_t i = 0; i < numMachines; ++i)
            {
                ind[t * numMachines + i] = dis(gen);
            }
        }
    }
}