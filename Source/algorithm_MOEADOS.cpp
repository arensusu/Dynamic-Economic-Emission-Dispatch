
#include <string>
#include <algorithm>
#include <limits>

#include "algorithm_MOEADOS.h"
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

bool MOEADOS::Setup(ifstream& file)
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

void MOEADOS::Solve(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;

    RandomInitialization initialize;

    Mutation* reproduct[3];
    reproduct[0] = new RandOneMutation();
    reproduct[1] = new BestOneMutation();
    reproduct[2] = new RandTwoMutation();

    double FCR[] = { 0.1, 0.5, 0.9 };

    PolynamialMutation mutate;

    Population pop(Psize_), archive, children;
    size_t ffe = 0;

    uniform_real_distribution<double> dis(0.0, 1.0);

    // Set parameters of reproductions.
    vector<double> params(3, 0.0);
    params[0] = F_;
    params[1] = CR_;

    for (size_t i = 0; i < numOperators_; ++i)
    {
        credits_[i] = 0.0;
    }

    WeightVectorInitialization(Psize_, prob.numObjectives());

    subCounts_.clear();
    subCounts_.resize(Psize_, 0);

    initialize(pop);
    for (size_t i = 0; i < Psize_; ++i)
    {
        ch(pop[i]);
        prob.Evaluate(pop[i]);
        ffe++;
    }

    UpdateReference(pop);
    UpdateNadir(pop);

    vector<double> oldFitness(Psize_);
    for (size_t i = 0; i < Psize_; ++i)
    {
        oldFitness[i] = Tchebycheff(pop[i], weightVectors_[i], referencePoint_, nadir_);
    }

    while (true)
    {
        log.All(pop);
        log.Detail(pop);

        if (ffe >= maxffe_)
        {
            break;
        }

        for (size_t i = 0; i < numOperators_; ++i)
        {
            opCounts_[i] = 0;
        }

        Individual child;

        vector<size_t> index(Psize_);
        RandomPermutation(index);

        for (size_t g = 0; g < 5; ++g)
        {
            vector<size_t> I(Psize_ / 5);
            I[0] = 0;
            I[1] = 1;
            for (size_t i = 2; i < I.size(); ++i)
            {
                I[i] = Tournament(pi_, 10);
            }

            for (size_t i = 0; i < I.size(); ++i)
            {
                size_t subProblem = I[i];
                subCounts_[subProblem] += 1;

                int op = 0;
                //if (ffe - Psize_ < Psize_)
                //{
                //    op = index[i + g * I.size()] % numOperators_;
                //}
                //else
                //{
                //    op = AOS(index[i + g * I.size()]);                 // Operator selection.
                //}

                //
                child = pop[subProblem];

                if (op == 0)
                {
                    params[0] = 0.5;
                    params[1] = 0.5;
                    params[2] = -1.0;
                }
                else if (op == 1)
                {
                    params[0] = 0.9;
                    params[1] = 0.1;
                    params[2] = FindtheBest(subProblem, pop);
                }

                //
                if (dis(gen) < 0.9)
                {
                    reproduct[op]->PartNeigh(child, pop, neighborIndice_[subProblem], params);
                }
                else
                {
                    reproduct[op]->AllNeigh(child, pop, params);
                }

                mutate(child, 1.0 / double(prob.numVariables()), eta_);

                ch(child);
                prob.Evaluate(child);
                ffe++;

                //
                child.op() = op;
                opCounts_[op] += 1;

                UpdateReference(child.objs());

                //size_t nearest = FindSubproblem(child);
                //bool isUpdate = UpdateNeighbor(pop, child, nearest, log);
                bool isUpdate = UpdateNeighbor(pop, child, I[i], log);

                double parent = Tchebycheff(pop[subProblem], weightVectors_[subProblem], referencePoint_, nadir_);
                double offspring = Tchebycheff(child, weightVectors_[subProblem], referencePoint_, nadir_);
                double IR = (parent - offspring) / parent;
                if (IR > 0)
                {
                    n_[op] += 1;
                    rewards_[op] += IR;
                }

                children.push_back(child);
            }
        }

        if ((ffe / Psize_) % 10 == 0)
        {
            vector<double> newFitness(Psize_);
            for (size_t i = 0; i < Psize_; ++i)
            {
                newFitness[i] = Tchebycheff(pop[i], weightVectors_[i], referencePoint_, nadir_);
            }

            for (size_t i = 0; i < Psize_; ++i)
            {
                double delta = (oldFitness[i] - newFitness[i]) / oldFitness[i];

                if (delta > 0.001)
                {
                    pi_[i] = 1.0;
                }
                else
                {
                    pi_[i] = pi_[i] * (0.95 + 0.05 * delta / 0.001);
                }

                oldFitness[i] = newFitness[i];
            }
        }
        CreditAssign();

        //log.OpCount(opCounts_);

        children.clear();
    }
    log.OpCount(subCounts_);

    sol = pop;
    log.RecordIGD();
}

bool MOEADOS::UpdateNeighbor(Population& pop, const Individual& ind, const size_t index, Log& log)
{
    size_t count = 0;

    vector<size_t> order(neighborhood_);
    RandomPermutation(order);

    vector<double> tmpNadir(nadir_);
    for (size_t i = 0; i < Individual::prob().numObjectives(); ++i)
    {
        tmpNadir[i] = max(tmpNadir[i], ind.objs()[i]);
    }

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

size_t MOEADOS::FindtheBest(const size_t index, const Population& pop)
{
    double bestTch = numeric_limits<double>::max();
    size_t bestIndex = 0;

    uniform_real_distribution<double> dis(0.0, 1.0);

    // Find from neighborhood.
    if (dis(gen) < 0.0)
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
            if (tmp < bestTch && i != index)
            {
                bestTch = tmp;
                bestIndex = i;
            }
        }
    }

    return bestIndex;
}

size_t MOEADOS::AOS(const size_t index)
{
    int op = 0;
    double rand = double(index);
    for (size_t i = 0; i < numOperators_; ++i)
    {
        if (rand < probability_[i] * Psize_)
        {
            op = i;
            break;
        }
        else
        {
            rand -= probability_[i] * Psize_;
        }
    }
    
    return op;
}

// Credit assignment.
void MOEADOS::CreditAssign()
{
    for (size_t i = 0; i < numOperators_; ++i)
    {
        credits_[i] = credits_[i] * (1 - alpha_);

        if (n_[i] != 0)
        {
            credits_[i] += alpha_ * rewards_[i] / double(n_[i]);
        }

        //if (credits_[i] < 0.01)
        //{
        //    credits_[i] = 0.01;
        //}

        n_[i] = 0;
        rewards_[i] = 0;
    }

    // Probability matching.
    double creditSum = 0.0;
    for (size_t i = 0; i < numOperators_; ++i)
    {
        creditSum += credits_[i];
    }

    if (creditSum == 0)
    {
        for (size_t i = 0; i < numOperators_; ++i)
        {
            probability_[i] = 1.0 / numOperators_;
        }
    }
    else
    {
        for (size_t i = 0; i < numOperators_; ++i)
        {
            probability_[i] = 0.1 + (1.0 - 0.1 * numOperators_) * (credits_[i] / creditSum);
        }
    }
    

    //// Adaptive pursuit.
    //size_t best = 0;
    //for (size_t i = 1; i < numOperators_; ++i)
    //{
    //    if (credits_[i] > credits_[best])
    //    {
    //        best = i;
    //    }
    //}

    //for (size_t i = 0; i < numOperators_; ++i)
    //{
    //    if (i == best)
    //    {
    //        probability_[i] = probability_[i] + 0.6 * ((1.0 - 0.1 * double(numOperators_ - 1) - probability_[i]));
    //    }
    //    else
    //    {
    //        probability_[i] = probability_[i] + 0.6 * (0.1 - probability_[i]);
    //    }
    //}
}
/*----------------------------------------------------------------------------------------------------*/
// Credit assignment.
void MOEADOS::CreditAssign(const size_t op, const double credit)
{
    credits_[op] = (1 - alpha_) * credits_[op] + alpha_ * credit;

    // Probability matching.
    double creditSum = 0.0;
    for (size_t i = 0; i < numOperators_; ++i)
    {
        creditSum += credits_[i];
    }

    for (size_t i = 0; i < numOperators_; ++i)
    {
        probability_[i] = 0.1 + (1.0 - 0.1 * numOperators_) * (credits_[i] / creditSum);
    }
}

void MOEADOS::CreditAssign(const Population& pop)
{
    vector<Population> group(numOperators_);
    for (size_t i = 0; i < pop.size(); ++i)
    {
        int op = pop[i].op();
        group[op].push_back(pop[i]);
    }

    vector<double> minE(numOperators_, numeric_limits<double>::max());
    vector<double> minC(numOperators_, numeric_limits<double>::max());
    vector<double> disMean(numOperators_, 0.0);

    for (size_t op = 0; op < numOperators_; ++op)
    {
        for (size_t i = 0; i < group[op].size(); ++i)
        {
            const Individual& ind = group[op][i];
            
            minC[op] = min(minC[op], ind.objs()[0]);
            minE[op] = min(minE[op], ind.objs()[1]);

            double minDis = numeric_limits<double>::max();
            for (size_t j = 0; j < group[op].size(); ++j)
            {
                if (i != j)
                {
                    minDis = min(minDis, DistanceNorm2(group[op][i].objs(), group[op][j].objs()));
                }
            }

            disMean[op] += minDis;
        }

        disMean[op] = disMean[op] / double(group[op].size());
    }

    double cSum = 0.0;
    double eSum = 0.0;
    double disSum = 0.0;
    for (size_t op = 0; op < numOperators_; ++op)
    {
        cSum += minC[op];
        eSum += minE[op];
        disSum += disMean[op];
    }

    for (size_t op = 0; op < numOperators_; ++op)
    {
        credits_[op] = credits_[op] * alpha_ + (1.0 - (minC[op] / cSum + minE[op] / eSum) / 2) + 0.0 * (disMean[op] / disSum);
    }
}


size_t MOEADOS::FindSubproblem(const Individual& ind) const
{
    size_t index = 0;
    double tch = numeric_limits<double>::max();

    for (size_t i = 0; i < Psize_; ++i)
    {
        double tmp = Tchebycheff(ind, weightVectors_[i], referencePoint_, nadir_);

        if (tmp < tch)
        {
            index = i;
            tch = tmp;
        }
    }

    return index;
}