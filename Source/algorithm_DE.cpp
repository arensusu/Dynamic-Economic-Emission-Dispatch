
#include <fstream>
#include <random>
#include <chrono>

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
#include "localsearch.h"
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

    RandomInitialization initialization;

    RandOneMutation mutation;
    
    BinaryCrossover crossover;
    
    OneDimEnvSelection envSelection;

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
        if (ffe >= maxffe_)
        {
            break;
        }

        pop[next].clear();
        pop[next].resize(Psize_);
        pop[curr].resize(Psize_ * 2);

        mutation(pop[curr], F_);
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
    }

    sol = pop[curr];
    log.RecordIGD();
}

void FitnessAssignment(Population& arch, Population& pop)
{
    size_t totalSize = arch.size() + pop.size();

    vector<size_t> strength(totalSize, 0);
    vector<vector<size_t>> dominatedBy(totalSize);
    vector<vector<double>> distance(totalSize, vector<double>(totalSize));

    for (size_t i = 0; i < totalSize; ++i)
    {
        Individual* curr;
        if (i < arch.size())
        {
            curr = &(arch[i]);
        }
        else
        {
            curr = &(pop[i - arch.size()]);
        }

        for (size_t j = 0; j < totalSize; ++j)
        {
            Individual* other;
            if (i != j)
            {
                if (j < arch.size())
                {
                    other = &(arch[j]);
                }
                else
                {
                    other = &(pop[j - arch.size()]);
                }

                if (FeasibleDominated(*curr, *other))
                {
                    strength[i]++;
                }

                if (FeasibleDominated(*other, *curr))
                {
                    dominatedBy[i].push_back(j);
                }
            }
            else
            {
                other = curr;
            }

            distance[i][j] = sqrt(pow((curr->objs()[0] - other->objs()[0]), 2) + pow((curr->objs()[1] - other->objs()[1]), 2));
        }
    }

    size_t k = sqrt(totalSize);

    for (size_t i = 0; i < totalSize; ++i)
    {
        Individual* curr;
        if (i < arch.size())
        {
            curr = &(arch[i]);
        }
        else
        {
            curr = &(pop[i - arch.size()]);
        }
        
        vector<double> list(totalSize);
        for (size_t j = 0; j < totalSize; ++j)
        {
            list[j] = distance[i][j];
        }
        sort(list.begin(), list.end());

        curr->fitness() = 1.0 / (list[k] + 2);

        for (size_t j = 0; j < dominatedBy[i].size(); ++j)
        {
            curr->fitness() += strength[dominatedBy[i][j]];
        }
    }
}

Population MatingSelection(const Population& arch, const Population& pop)
{
    size_t totalSize = arch.size() + pop.size();
    
    vector<size_t> index(totalSize);
    for (size_t i = 0; i < totalSize; ++i)
    {
        index[i] = i;
    }

    shuffle(index.begin(), index.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));

    Population chosen(pop.size());
    for (size_t i = 0; i < chosen.size(); ++i)
    {
        const Individual* ind;
        if (index[i] < arch.size())
        {
            ind = &(arch[index[i]]);
        }
        else
        {
            ind = &(pop[index[i] - arch.size()]);
        }

        chosen[i] = *ind;
    }

    return chosen;
}

void DE::SPEA2(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;

    RandomInitialization initialize;

    RandOneMutation mutate;
    //BestOneMutation mutate;

    BinaryCrossover crossover;

    SPEA2EnvSelection envSelect;

    Population pop(Psize_);
    Population archive;

    int ffe = 0;

    // Initialization.
    initialize(pop, prob);

    while (true)
    {
        for (size_t i = 0; i < Psize_; ++i)
        {
            ch(pop[i]);
            prob.Evaluate(pop[i]);
            ffe++;
        }

        if (ffe >= maxffe_)
        {
            break;
        }

        // Fitness assignment.
        FitnessAssignment(archive, pop);

        archive.resize(100);
        // Environment selection.
        archive = envSelect(archive, pop);

        // Mating selection.
        Population parents = MatingSelection(archive, pop);

        parents.resize(Psize_ * 2);
        // Reproduction.
        mutate(parents, F_);
        crossover(parents, CR_);

        for (size_t i = 0; i < Psize_; ++i)
        {
            pop[i] = parents[Psize_ + i];
        }

        log.All(archive);
        log.Detail(archive);
    }

    sol = pop;
    log.RecordIGD();
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