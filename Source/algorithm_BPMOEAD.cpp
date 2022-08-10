
#include <string>
#include <algorithm>
#include <limits>

#include "algorithm_BPMOEAD.h"
#include "alg_initialization.h"
#include "alg_constraint_handling.h"
#include "alg_mating_selection.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"

#include "population.h"
#include "individual.h"
#include "problem_base.h"
#include "log.h"

#include "tool.h"
#include "alg_sorting.h"

using namespace std;

bool BPMOEAD::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;

    neighborhood_ = size_t(Psize_ * 0.1);
    maxReplace_ = size_t(Psize_ * 0.01);

    return true;
}

void BPMOEAD::Solve(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;

    RandomInitialization initialize;

    Mutation* reproduct[3];
    reproduct[0] = new RandOneMutation();
    reproduct[1] = new BestOneMutation();
    reproduct[2] = new RandTwoMutation();

    PolynamialMutation mutate;

    vector<Population> pop(numOperators_, Population(Psize_));
    Population archive(Psize_), children;
    size_t ffe = 0;

    uniform_real_distribution<double> dis(0.0, 1.0);

    // Set parameters of reproductions.
    vector<double> params(3, 0.0);
    params[0] = F_;
    params[1] = CR_;

    WeightVectorInitialization(Psize_, prob.numObjectives());

    for (size_t i = 0; i < numOperators_; ++i)
    {
        initialize(pop[i]);
        for (size_t j = 0; j < Psize_; ++j)
        {
            ch(pop[i][j]);
            prob.Evaluate(pop[i][j]);
            ffe++;

        }

        UpdateReference(pop[i]);
        UpdateNadir(pop[i]);
    }

    vector<vector<double>> oldFitness(numOperators_, vector<double>(Psize_));
    for (size_t i = 0; i < numOperators_; ++i)
    {
        for (size_t j = 0; j < Psize_; ++j)
        {
            oldFitness[i][j] = Tchebycheff(pop[i][j], weightVectors_[i], referencePoint_, nadir_);
        }
    }

    bool isCommunicated = false;
    while (true)
    {
        if (ffe >= maxffe_)
        {
            break;
        }

        for (size_t i = 0; i < numOperators_; ++i)
        {
            log.Detail(pop[i]);

            Individual child;

            vector<size_t> index(Psize_);
            RandomPermutation(index);

            for (size_t g = 0; g < 5; ++g)
            {
                vector<size_t> I(Psize_ / 5);
                I[0] = 0;
                I[1] = 1;
                for (size_t j = 2; j < I.size(); ++j)
                {
                    I[j] = Tournament(pi_, 10);
                }

                for (size_t j = 0; j < I.size(); ++j)
                {
                    size_t subProblem = I[j];
                    int op = i;
                    
                    //
                    child = pop[i][subProblem];

                    if (op == 0)
                    {
                        params[2] = -1.0;
                    }
                    else if (op == 1)
                    {
                        params[2] = FindtheBest(subProblem, pop[i]);
                    }

                    //
                    if (dis(gen) < 0.9 || isCommunicated)
                    {
                        reproduct[op]->PartNeigh(child, pop[i], neighborIndice_[subProblem], params);
                    }
                    else
                    {
                        reproduct[op]->AllNeigh(child, pop[i], params);
                    }

                    mutate(child, 1.0 / double(prob.numVariables()), eta_);

                    ch(child);
                    prob.Evaluate(child);
                    ffe++;

                    UpdateReference(child.objs());

                    bool isUpdate = UpdateNeighbor(pop[i], child, subProblem, log);

                    children.push_back(child);
                }
            }

            if ((ffe / Psize_) % 10 == 0)
            {
                vector<double> newFitness(Psize_);
                for (size_t j = 0; j < Psize_; ++j)
                {
                    newFitness[j] = Tchebycheff(pop[i][j], weightVectors_[j], referencePoint_, nadir_);
                }

                for (size_t j = 0; j < Psize_; ++j)
                {
                    double delta = (oldFitness[i][j] - newFitness[j]) / oldFitness[i][j];

                    if (delta > 0.001 || 1)
                    {
                        pi_[j] = 1.0;
                    }
                    else
                    {
                        pi_[j] = pi_[j] * (0.95 + 0.05 * delta / 0.001);
                    }

                    oldFitness[i][j] = newFitness[j];
                }
            }
            children.clear();
        }

        if ((ffe / Psize_) % 160 == 0)
        {
            EnvSelection env;

            Population hybrid;
            for (size_t i = 0; i < numOperators_; ++i)
            {
                hybrid.push_back(pop[i]);
            }

            env.CDP(archive, hybrid);

            for (size_t i = 0; i < numOperators_; ++i)
            {
                pop[i] = archive;
            }

            log.All(archive);

            isCommunicated = true;
        }
        else
        {
            isCommunicated = false;
        }
    }

    sol = archive;
    log.RecordIGD();
}

bool BPMOEAD::UpdateNeighbor(Population& pop, const Individual& ind, const size_t index, Log& log)
{
    size_t count = 0;

    vector<size_t> order(neighborhood_);
    RandomPermutation(order);

    vector<double> tmpNadir(nadir_);
    for (size_t i = 0; i < Individual::prob().numObjectives(); ++i)
    {
        tmpNadir[i] = max(tmpNadir[i], ind.objs()[i]);
    }

    double FIR = 0.0;
    for (size_t i = 0; i < neighborhood_; ++i)
    {
        size_t neighborIndex = neighborIndice_[index][order[i]];
        Individual& target = pop[neighborIndex];

        double offspring = Tchebycheff(ind, weightVectors_[neighborIndex], referencePoint_, tmpNadir);
        double parent = Tchebycheff(target, weightVectors_[neighborIndex], referencePoint_, tmpNadir);

        if (count < maxReplace_)
        {
            if (offspring <= parent)
            {
                target = ind;
                count++;

                nadir_ = tmpNadir;
            }

        }
        else
        {
            break;
        }

    }

    if (count > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

size_t BPMOEAD::FindtheBest(const size_t index, const Population& pop)
{
    double bestTch = numeric_limits<double>::max();
    size_t bestIndex = 0;

    uniform_real_distribution<double> dis(0.0, 1.0);

    // Find from neighborhood.
    if (dis(gen) < 1.0)
    {
        for (size_t i = 0; i < neighborhood_; ++i)
        {
            double tmp = Tchebycheff(pop[neighborIndice_[index][i]], weightVectors_[index], referencePoint_, nadir_);
            //double tmp = 0.5 * pop[i].objs()[0] + 0.5 * pop[i].objs()[1];
            if (tmp < bestTch)
            {
                bestTch = tmp;
                bestIndex = neighborIndice_[index][i];
            }
        }
    }
    else
    {
        // Find from population.
        for (size_t i = 0; i < Psize_; ++i)
        {
            double tmp = Tchebycheff(pop[i], weightVectors_[index], referencePoint_, nadir_);
            //double tmp = 0.5 * pop[i].objs()[0] + 0.5 * pop[i].objs()[1];
            if (tmp < bestTch)
            {
                bestTch = tmp;
                bestIndex = i;
            }
        }
    }

    return bestIndex;
}
