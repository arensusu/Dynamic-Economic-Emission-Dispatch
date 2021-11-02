
#include <fstream>

#include "algorithm_DE.h"
#include "population.h"
#include "problem_base.h"

using namespace std;

bool DE::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> name_;
    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;

    return true;
}

void DE::Solve(Population& sol, const BProblem& prob)
{
    Population pop[2] = { Population(Psize_) };
    int cur = 0, next = 1;
    int ffe = 0;

    //initialization
    for (size_t i = 0; i < Psize_; ++i)
    {
        //CH
        prob.Evaluate(pop[cur][i]);
        ffe++;
    }

    while (ffe < maxffe_)
    {
        pop[cur].resize(Psize_ * 2);
        for (size_t i = 0; i < Psize_; ++i)
        {
            //mutation

            //crossover

            //CH
            prob.Evaluate(pop[cur][Psize_ + i]);
            ffe++;
        }
        //selection

        //print objectives
    }
    return;
}