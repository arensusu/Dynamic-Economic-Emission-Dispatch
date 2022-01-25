
#include <vector>
#include <algorithm>

#include "algorithm_NSDE.h"

#include "alg_initialization.h"
#include "alg_constraint_handling.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"

#include "population.h"
#include "individual.h"
#include "log.h"
#include "tool.h"

using namespace std;

void NSDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;

    RandomInitialization initialization;

    //BestOneMutation mutation;
    RandOneMutation mutation;

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
        // Adaptive control.
        //Adaptive(ffe);

        pop[next].clear();
        pop[next].resize(Psize_);

        pop[curr].resize(Psize_ * 2);

        //mutation
        
        mutation(pop[curr], F_);

        //crossover
        crossover(pop[curr], CR_);

        for (size_t i = 0; i < Psize_; ++i)
        {
            //CH
            ch(pop[curr][Psize_ + i]);
            prob.Evaluate(pop[curr][Psize_ + i]);
            ffe++;

        }
        //selection
        envSelection(pop[next], pop[curr]);

        //print objectives
        log.All(pop[next]);
        log.Detail(pop[next]);

        swap(pop[curr], pop[next]);

        if (ffe >= maxffe_)
        {
            break;
        }

        // Diversity control.
        //for (size_t i = 0; i < Psize_; ++i)
        //{
        //    if (diversity(pop[curr][i], 1.0 / double(prob.numVariables())))
        //    {
        //        ch(pop[curr][i]);
        //        prob.Evaluate(pop[curr][i]);
        //        ffe++;
        //    }
        //}
    }

    sol = pop[curr];
    log.RecordIGD();
}