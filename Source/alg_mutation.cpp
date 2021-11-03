
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "alg_mutation.h"
#include "population.h"
#include "individual.h"

using namespace std;

void RandOneMutation::operator()(Population& pop, const double F, const int pos) const
{
    srand(time(NULL));
    size_t Psize = pop.size() / 2;
    vector<int> index(Psize);
    for (size_t i = 0; i < Psize; ++i)
    {
        index[i] = i;
    }
    random_shuffle(index.begin(), index.end());

    int r1 = index[0];
    int r2 = index[1];
    int r3 = index[2];

    vector<double>& mutant = pop[Psize + pos].encoding();
    mutant.resize(Individual::prob().numVariables());
    for (size_t i = 0; i < Psize; ++i)
    {
        mutant[i] = pop[r1][i] + F * (pop[r2][i] - pop[r3][i]);
    }

    return;
}