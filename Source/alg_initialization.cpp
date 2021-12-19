
#include <chrono>
#include <random>

#include "alg_initialization.h"
#include "problem_base.h"
#include "population.h"
#include "individual.h"

using namespace std;

void RandomInitialization::operator()(Population& pop, const BProblem& prob) const
{
    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < pop.size(); ++i)
    {
        for (size_t j = 0; j < prob.numVariables(); ++j)
        {
            pop[i][j] = dis(gen);
        }
    }
    return;
}