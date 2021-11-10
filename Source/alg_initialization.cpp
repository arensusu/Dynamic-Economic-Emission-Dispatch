
#include <cstdlib>
#include <ctime>

#include "alg_initialization.h"
#include "problem_base.h"
#include "population.h"
#include "individual.h"

using namespace std;

void RandomInitialization::operator()(Population& pop, const BProblem& prob) const
{
    srand(time(NULL));
    for (size_t i = 0; i < pop.size(); ++i)
    {
        vector<double> encoding = pop[i].encoding();
        for (size_t j = 0; j < prob.numVariables(); ++j)
        {
            encoding[j] = double(rand() % 100) / 100;
        }
    }
    return;
}