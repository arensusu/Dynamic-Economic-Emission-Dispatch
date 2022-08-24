
#include <string>
#include <algorithm>

#include "algorithm_MOEAD-DE.h"
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

bool MOEADDE::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;

    neighborhood_ = Psize_ * 0.1;
    maxReplace_ = Psize_ * 0.01;

    return true;
}

void MOEADDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    FineTuningCH ch;
    //DivisionCH ch;
    //ProportionDivisionCH ch;

    RandomInitialization initialize;

    RandomMatingSelection mating;

    RandOneMutation reproduct;

    PolynomialMutation mutate;

    BinaryCrossover crossover;

    // Set parameters of reproductions.
    vector<double> params(2);
    params[0] = F_;
    params[1] = CR_;

    Population pop(Psize_), archive, children;
    size_t ffe = 0;

    uniform_real_distribution<double> dis(0.0, 1.0);

    WeightVectorInitialization(Psize_, prob.numObjectives());

    initialize(pop);
    for (size_t i = 0; i < Psize_; ++i)
    {
        ch(pop[i]);
        prob.Evaluate(pop[i]);
        ffe++;
    }

    UpdateReference(pop);
    UpdateNadir(pop);

    while (true)
    {
        log.All(pop);
        log.Detail(pop);

        if (ffe >= maxffe_)
        {
            break;
        }

        Individual child;

        for (size_t i = 0; i < Psize_; ++i)
        {
            child = pop[i];
            if (dis(gen) < 0.9)
            {
                reproduct.PartNeigh(child, pop, neighborIndice_[i], params);
            }
            else
            {
                reproduct.AllNeigh(child, pop, params);
            }

            mutate(child, 1.0 / double(prob.numVariables()), 20);

            ch(child);
            prob.Evaluate(child);
            ffe++;

            UpdateReference(child.objs());

            UpdateNeighbor(pop, child, i);

            children.push_back(child);
        }
        //log.Child(children);
        children.clear();
    }

    sol = pop;
    log.RecordIGD();
}

void MOEADDE::WeightVectorInitialization(const size_t size, const size_t objs)
{
    pi_.resize(size);
    weightVectors_.resize(size, vector<double>(objs));

    ifstream weightFile("./Weightvector/" + to_string(size) + "_" + to_string(objs) + ".txt");
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < objs; ++j)
        {
            weightFile >> weightVectors_[i][j];
        }

        pi_[i] = 1.0;
    }

    neighborIndice_.resize(size, vector<size_t>(neighborhood_));

    vector<vector<double>> dis(size, vector<double>(2));
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            dis[j][0] = DistanceNorm2(weightVectors_[i], weightVectors_[j]);
            dis[j][1] = j;
        }

        sort(dis.begin(), dis.end());

        for (size_t j = 0; j < neighborhood_; ++j)
        {
            neighborIndice_[i][j] = size_t(dis[j][1]);
        }
    }
}

void MOEADDE::UpdateReference(const vector<double>& objs)
{
    if (referencePoint_.size() == 0)
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            referencePoint_.push_back(objs[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            double& min = referencePoint_[i];
            double now = objs[i];

            if (now < min)
            {
                min = now;
            }
        }
    }
}

void MOEADDE::UpdateReference(const Population& pop)
{
    referencePoint_.clear();

    for (size_t i = 0; i < pop.size(); ++i)
    {
        UpdateReference(pop[i].objs());
    }
}

void MOEADDE::UpdateNadir(const vector<double>& objs)
{
    if (nadir_.size() == 0)
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            nadir_.push_back(objs[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            double& max = nadir_[i];
            double now = objs[i];

            if (now > max)
            {
                max = now;
            }
        }
    }
}

void MOEADDE::UpdateNadir(const Population& pop)
{
    nadir_.clear();

    for (size_t i = 0; i < pop.size(); ++i)
    {
        UpdateNadir(pop[i].objs());
    }
}

void MOEADDE::UpdateNeighbor(Population& pop, const Individual& ind, const size_t index)
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
        if (count < maxReplace_)
        {
            size_t neighborIndex = neighborIndice_[index][order[i]];
            Individual& target = pop[neighborIndex];

            //double offspring = Tchebycheff(ind, weightVectors_[neighborIndex], referencePoint_, tmpNadir);
            //double parent = Tchebycheff(target, weightVectors_[neighborIndex], referencePoint_, tmpNadir);
            double offspring = WeightedSum(ind, weightVectors_[neighborIndex], referencePoint_, tmpNadir);
            double parent = WeightedSum(target, weightVectors_[neighborIndex], referencePoint_, tmpNadir);

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
}

void MOEADDE::UpdateArchive(Population& arch, const Individual& ind) const
{
    if (arch.size() == 0)
    {
        arch.push_back(ind);
    }
    else
    {
        bool beDominated = false;
        vector<size_t> dominate;

        for (size_t i = 0; i < arch.size(); ++i)
        {
            if (FeasibleDominated(ind, arch[i]))
            {
                dominate.push_back(i);
            }
            else if (FeasibleDominated(arch[i], ind) || ind == arch[i])
            {
                beDominated = true;
            }
        }

        if (!beDominated)
        {
            for (int i = dominate.size() - 1; i >= 0; --i)
            {
                arch.erase(dominate[i]);
            }

            arch.push_back(ind);
        }
    }

    if (arch.size() > Psize_)
    {
        vector<size_t> orders(arch.size());
        Permutation(orders);

        CrowdingDistanceSort(orders, arch);

        size_t removeNum = arch.size() - Psize_;

        for (size_t i = 0; i < removeNum; ++i)
        {
            arch.erase(orders[orders.size() - 1 - i]);
        }
    }
}

void MOEADDE::UpdateArchive(Population& arch, const Population& pop) const
{
    Population hybrid(arch, pop);
    arch.clear();

    vector<vector<size_t>> fronts = NondominatedSort(hybrid, FeasibleDominated);

    if (fronts[0].size() <= Psize_)
    {
        arch.resize(fronts[0].size());
    }
    else
    {
        arch.resize(Psize_);

        CrowdingDistanceSort(fronts[0], hybrid);
    }

    for (size_t i = 0; i < arch.size(); ++i)
    {
        arch[i] = hybrid[fronts[0][i]];
    }
}