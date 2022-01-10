
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
#include "alg_sorting.h"

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
    ProportionDivisionCH ch;
    //DivisionCH ch;
    //FineTuningCH ch;
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
        // Adaptive control.
        Adaptive(ffe);

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

            // Re-initialize the infeasible solution.
            while (!pop[curr][Psize_ + i].Check())
            {
                log++;
                initialization(pop[curr][Psize_ + i], prob);
                ch(pop[curr][Psize_ + i]);
                ffe++;
            }

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
        //size_t start = 0;
        //for (size_t i = start; i < Psize_; ++i)
        //{
        //    diversity(pop[curr][i], 1.0 / double(prob.numVariables()));
        //    ch(pop[curr][i]);

        //    while (!pop[curr][i].Check())
        //    {
        //        log++;
        //        initialization(pop[curr][i], prob);
        //        ch(pop[curr][i]);
        //        ffe++;
        //    }

        //    prob.Evaluate(pop[curr][i]);
        //    ffe++;
        //}
    }

    sol = pop[curr];
    log.RecordIGD();
    
    return;
}

void DE::Adaptive(const int ffe)
{
    // Exponetial.
    F_ = Fmin_ + (Fmax_ - Fmin_) * exp(-2.0 * double(ffe) / double(maxffe_));
    CR_ = CRmin_ + (CRmax_ - CRmin_) * exp(-2.0 * (1.0 - double(ffe) / double(maxffe_)));
    
    // Linear.
    //F_ = Fmax_ - (Fmax_ - Fmin_) * (double(ffe) / double(maxffe_));
    //CR_ = CRmin_ + (CRmax_ - CRmin_) * (double(ffe) / double(maxffe_));
}