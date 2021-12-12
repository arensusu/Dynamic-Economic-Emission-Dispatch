
#include <ctime>
#include <random>
#include <algorithm>
#include <cmath>

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

    random_device rd;
    shuffle(index.begin(), index.end(), default_random_engine(rd() * time(NULL)));

    int r1 = index[0];
    int r2 = index[1];
    int r3 = index[2];
    
    //cout << r1 << " " << r2 << " " << r3 << endl;

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

void PolynToCurMutation::operator()(Population& pop, const double F, const size_t pos, const double eta) const
{
    //polynamial mutation
    random_device rd;
    default_random_engine gen(rd() * time(NULL));
    uniform_real_distribution<double> dis(0.0, 1.0);

    vector<double> polyn(Individual::prob().numVariables());

    for (size_t i = 0; i < polyn.size(); ++i)
    {
        double delta;

        double r1 = dis(gen);

        if (r1 <= 0.5)
        {
            delta = pow((2.0 * r1), (1.0 / (eta + 1))) - 1.0;
            polyn[i] = pop[pos][i] + delta * (pop[pos][i] - 0.0);
        }
        else
        {
            delta = 1.0 - pow((2.0 * (1 - r1)), (1.0 / (eta + 1.0)));
            polyn[i] = pop[pos][i] + delta * (1.0 - pop[pos][i]);
        }

    }

    //mutant vector
    size_t Psize = pop.size() / 2;
    vector<int> index(Psize);
    for (size_t i = 0; i < Psize; ++i)
    {
        index[i] = i;
    }

    shuffle(index.begin(), index.end(), default_random_engine(rd() * time(NULL)));

    int r2 = index[0];
    int r3 = index[1];

    vector<double>& mutant = pop[Psize + pos].encoding();
    mutant.resize(Individual::prob().numVariables());
    for (size_t i = 0; i < Individual::prob().numVariables(); ++i)
    {
        mutant[i] = pop[pos][i] + F * (pop[pos][i] - polyn[i]) + F * (pop[r2][i] - pop[r3][i]);
    }

    return;
}

void PolynamialMutation::operator()(Individual& ind, const double eta) const
{
    size_t num = Individual::prob().numVariables();

    random_device rd;
    default_random_engine gen(rd() * time(NULL));
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < num; ++i)
    {
        if (dis(gen) < (1.0 / num))
        {
            double delta;

            double r1 = dis(gen);
            if (r1 <= 0.5)
            {
                delta = pow((2.0 * r1), (1.0 / (eta + 1.0))) - 1.0;
                ind.encoding()[i] += delta * (ind.encoding()[i] - 0.0);
            }
            else
            {
                delta = 1.0 - pow((2.0 * (1.0 - r1)), (1.0 / (eta + 1.0)));
                ind.encoding()[i] += delta * (1.0 - ind.encoding()[i]);
            }
        }
    }
}