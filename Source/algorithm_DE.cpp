
#include <fstream>

#include "algorithm_DE.h"
#include "population.h"
#include "individual.h"
#include "problem_base.h"

#include "alg_constraint_handling.h"
#include "alg_initialization.h"
#include "alg_crossover.h"
#include "alg_mutation.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"

#include "log.h"

#include <algorithm>

using namespace std;

bool DE::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;

    return true;
}

void DE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    //DivisionCH ch;
    FineTuningCH ch;
    RandomInitialization initialization;
    //BestOneMutation mutation;
    //CurrentToBestMutation mutation;
    RandOneMutation mutation;
    //PolynToCurMutation mutation;
    BinaryCrossover crossover;
    BasicEnvSelection envSelection;
    PolynamialMutation diversity;

    Population pop[2] = { Population(Psize_) };
    size_t curr = 0, next = 1;
    int ffe = 0;

    //initialization
    initialization(pop[curr], prob);
    for (size_t i = 0; i < Psize_; ++i)
    {
        //CH
        ch(pop[curr][i]);
        prob.Evaluate(pop[curr][i]);
        ffe++;
    }

    while (true)
    {
        pop[next].clear();
        pop[next].resize(Psize_);

        pop[curr].resize(Psize_ * 2);

        for (size_t i = 0; i < Psize_; ++i)
        {

            //mutation
            mutation(pop[curr], F_, i);

            //crossover
            crossover(pop[curr][i], pop[curr][Psize_ + i], CR_);

            //CH
            ch(pop[curr][Psize_ + i]);
            prob.Evaluate(pop[curr][Psize_ + i]);
            ffe++;
        }
        //selection
        envSelection(pop[next], pop[curr]);

        //print objectives
        log.All(pop[next]);
        log.Front(pop[next]);

        swap(pop[curr], pop[next]);

        if (ffe >= maxffe_)
        {
            break;
        }

        //diversity
        
        size_t numVariables = prob.numVariables();
        for (size_t i = 0; i < Psize_; ++i)
        {
            diversity(pop[curr][i], 1.0 / double(numVariables));
            ch(pop[curr][i]);
            prob.Evaluate(pop[curr][i]);
            ffe++;
        }
        
    }

    sol = pop[curr];
    log.RecordIGD();
    
    return;
}