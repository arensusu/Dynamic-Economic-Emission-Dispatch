
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
        Adaptive(ffe);

        pop[next].clear();
        pop[next].resize(Psize_);

        pop[curr].resize(Psize_ * 2);

        for (size_t i = 0; i < Psize_; ++i)
        {
            // Diversity.
            
            Individual temp = pop[curr][i]; //
            diversity(pop[curr][i], 1.0 / double(prob.numVariables()));   //
            

            //mutation
            mutation(pop[curr], F_, i);

            //crossover
            crossover(pop[curr][i], pop[curr][Psize_ + i], CR_);

            //CH
            ch(pop[curr][Psize_ + i]);
            prob.Evaluate(pop[curr][Psize_ + i]);
            ffe++;

            // If individual after mutated is better, replaced the origin one.
            
            ch(pop[curr][i]);   //
            prob.Evaluate(pop[curr][i]);    //
            ffe++;  //
            if (!Dominated(pop[curr][i], temp))
            {
                pop[curr][i] = temp;    //
            }
            
        }
        //selection
        size_t frontSize = envSelection(pop[next], pop[curr]);

        //print objectives
        log.All(pop[next]);
        log.Front(pop[next]);

        swap(pop[curr], pop[next]);

        if (ffe >= maxffe_)
        {
            break;
        }
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