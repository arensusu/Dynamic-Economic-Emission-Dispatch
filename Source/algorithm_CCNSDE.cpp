
#include <vector>
#include <algorithm>


#include "algorithm_CCNSDE.h"

#include "alg_initialization.h"
#include "alg_constraint_handling.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"
#include "alg_sorting.h"

#include "population.h"
#include "individual.h"
#include "log.h"
#include "tool.h"

using namespace std;

void CCNSDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    SubProblemInitialization init;

    RandOneMutation mutate;

    BinaryCrossover crossover;

    EnvSelection localEnv;
    EnvSelection globalEnv;

    size_t ffe = 0;

    Population archive(Psize_);
    vector<Population> subPop(prob.numPeriods(), Population(10));
    Individual bestIndv;

    for (size_t i = 0; i < subPop.size(); ++i)
    {
        init(subPop[i], i);
        
        size_t index = IntRandomNumber(subPop[i].size());
        Combine(bestIndv, subPop[i][index]);
    }

    Population pop;
    for (size_t i = 0; i < subPop.size(); ++i)
    {
        for (size_t j = 0; j < subPop[i].size(); ++j)
        {
            Individual tmp = PartialEvaluate(subPop[i][j], bestIndv, i);
            pop.push_back(tmp);
            ffe++;
        }
    }
    // Update bestIndv.
    UpdateBestIndv(bestIndv, subPop);

    // Update archive.
    globalEnv.CDP(archive, pop);

    while (true)
    {
        log.All(archive);
        log.Detail(archive);

        if (ffe >= maxffe_)
        {
            break;
        }

        // Reproduction.
        pop.clear();
        for (size_t i = 0; i < subPop.size(); ++i)
        {
            Population offspring(subPop[i].size());

            //mutate(offspring, subPop[i], F_);

            crossover(offspring, subPop[i], CR_);

            for (size_t j = 0; j < offspring.size(); ++j)
            {
                Individual tmp = PartialEvaluate(offspring[j], bestIndv, i);
                pop.push_back(tmp);
                ffe++;
            }

            // Sub-population environmental selection.
            Population hybrid(subPop[i], offspring);
            subPop[i].clear();
            subPop[i].resize(10);
            localEnv.CDP(subPop[i], hybrid);
        }

        // Update bestIndv.
        UpdateBestIndv(bestIndv, subPop);

        // Update archive.
        Population hybrid(pop, archive);
        archive.clear();
        archive.resize(Psize_);
        globalEnv.CDP(archive, hybrid);
    }

    log.RecordIGD();
    sol = archive;
}

void CCNSDE::Combine(Individual& target, const Individual& source) const
{
    size_t numVariables = Individual::prob().numVariables();

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (source[i] != -1.0)
        {
            target[i] = source[i];
        }
    }
}

Individual CCNSDE::PartialEvaluate(Individual& target, const Individual& best, const size_t index) const
{
    size_t numPeriods = Individual::prob().numPeriods();
    size_t numMachines = Individual::prob().numMachines();
    for (size_t i = 0; i < numPeriods; ++i)
    {
        if (i == index)
        {
            for (size_t j = 0; j < numMachines; ++j)
            {
                target[i * numMachines + j] = max(0.0, min(1.0, target[i * numMachines + j]));
            }
        }
    }

    Individual tmp = best;
    Combine(tmp, target);
    Individual::prob().Evaluate(tmp);

    target.objs() = tmp.objs();
    target.violation() = tmp.violations()[index];

    return tmp;
}

void CCNSDE::UpdateBestIndv(Individual& target, const vector<Population>& subPop) const
{
    for (size_t i = 0; i < subPop.size(); ++i)
    {
        size_t index = 0;
        for (size_t j = 1; j < subPop[i].size(); ++j)
        {
            if (FeasibleDominated(subPop[i][j], subPop[i][index]))
            {
                index = j;
            }
            else if (!FeasibleDominated(subPop[i][index], subPop[i][j]))
            {
                if ((index + j / 2) == 0)
                {
                    index = j;
                }
            }
        }

        Combine(target, subPop[i][index]);
    }
}