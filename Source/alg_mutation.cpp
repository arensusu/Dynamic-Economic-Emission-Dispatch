
#include <ctime>
#include <random>
#include <algorithm>

#include "alg_mutation.h"
#include "population.h"
#include "individual.h"

using namespace std;



void RandOneMutation::operator()(Population& pop, const double F, const int pos) const
{
    size_t Psize = pop.size() / 2;
    vector<int> index(Psize);
    for (size_t i = 0; i < Psize; ++i)
    {
        index[i] = i;
    }
    shuffle(index.begin(), index.end(), default_random_engine(time(NULL)));

    int r1 = index[0];
    int r2 = index[1];
    int r3 = index[2];

    vector<double>& mutant = pop[Psize + pos].encoding();
    mutant.resize(Individual::prob().numVariables());
    for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
    {
        mutant[i] = pop[r1][i] + F * (pop[r2][i] - pop[r3][i]);
    }

    return;
}

void BestOneMutation::operator() (Population& pop, const double F, const int pos) const
{
    size_t Psize = pop.size() / 2;
    vector<int> index(Psize);
    for (size_t i = 0; i < Psize; ++i)
    {
        index[i] = i;
    }
    shuffle(index.begin(), index.end(), default_random_engine(time(NULL)));

    int r2 = index[1];
    int r3 = index[2];

    size_t best = 0;
    for (size_t i = 1; i < Psize; ++i)
    {
        double objB = 0.5 * pop[best].objs()[0] + 0.5 * pop[best].objs()[1];
        double objN = 0.5 * pop[i].objs()[0] + 0.5 * pop[i].objs()[1];
        if (objN < objB)
        {
            best = i;
        }
    }

    vector<double>& mutant = pop[Psize + pos].encoding();
    mutant.resize(Individual::prob().numVariables());
    for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
    {
        mutant[i] = pop[best][i] + F * (pop[r2][i] - pop[r3][i]);
    }

    return;
}