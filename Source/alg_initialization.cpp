
#include <chrono>
#include <random>

#include "alg_initialization.h"
#include "problem_base.h"
#include "population.h"
#include "individual.h"

#include "tool.h"

using namespace std;

void RandomInitialization::operator()(Population& pop, const BProblem& prob) const
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        (*this)(pop[i], prob);
    }
    return;
}

void RandomInitialization::operator()(Individual& ind, const BProblem& prob) const
{
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < prob.numVariables(); ++i)
    {
        ind[i] = dis(gen);
    }

    return;
}