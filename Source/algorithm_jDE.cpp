
#include <fstream>
#include <random>
#include <chrono>

#include "algorithm_jDE.h"
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

using namespace std;

bool JDE::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;

    file >> dummy >> dummy >> Fmin_;
    file >> dummy >> dummy >> Fmax_;

    file >> dummy >> dummy >> tau1_;
    file >> dummy >> dummy >> tau2_;

    return true;
}

void JDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    //DivisionCH ch;
    //ProportionDivisionCH ch;
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
    SelfInitialization(pop[curr]);
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

        //mutation
        mutation(pop[curr]);

        //crossover
        crossover(pop[curr]);

        // Self-adaptive.
        SelfAdaptive(pop[curr]);

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
                SelfInitialization(pop[curr][Psize_ + i]);
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
        //for (size_t i = Psize_ - Psize_ / 10; i < Psize_; ++i)
        //{
        //    diversity(pop[curr][i], 1.0 / double(prob.numVariables()));
        //    ch(pop[curr][i]);

        //    while (!pop[curr][i].Check())
        //    {
        //        log++;
        //        initialization(pop[curr][i], prob);
        //        ch(pop[curr][i]);
        //        SelfInitialization(pop[curr][i]);
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

void JDE::SelfInitialization(Population& pop) const
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        SelfInitialization(pop[i]);
    }
}

void JDE::SelfInitialization(Individual& ind) const
{
    ind.F() = 0.5;
    ind.CR() = 0.5;
}

void JDE::SelfAdaptive(const Individual& parent, Individual& offspring) const
{
    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dis(0.0, 1.0);

    if (dis(gen) < tau1_)
    {
        offspring.F() = Fmin_ + dis(gen) * Fmax_;
    }
    else
    {
        offspring.F() = parent.F();
    }

    if (dis(gen) < tau2_)
    {
        offspring.CR() = dis(gen);
    }
    else
    {
        offspring.CR() = parent.CR();
    }
}

void JDE::SelfAdaptive(Population& pop) const
{
    for (size_t i = 0; i < Psize_; ++i)
    {
        this->SelfAdaptive(pop[i], pop[Psize_ + i]);
    }
}