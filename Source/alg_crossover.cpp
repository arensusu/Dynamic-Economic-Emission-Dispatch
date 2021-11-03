
#include <cstdlib>
#include <ctime>

#include "alg_crossover.h"
#include "individual.h"

using namespace std;

void BinaryCrossover::operator()(const Individual& target, Individual& mutant, const double CR) const
{
    srand(time(NULL));
    for (int i = 0; i < Individual::prob().numVariables(); ++i)
    {
        if (rand() % 10 < CR * 10)
        {
            mutant[i] = mutant[i];
        }
        else
        {
            mutant[i] = target[i];
        }
    }

    return;
}