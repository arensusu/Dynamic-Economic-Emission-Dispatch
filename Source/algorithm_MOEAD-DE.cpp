
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
    file >> dummy >> dummy >> neighborhood_;

    return true;
}

void MOEADDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    //InequalityCH ch;
    ProportionDivisionCH ch;

    RandomInitialization initialize;

    RandomMatingSelection mating;

    RandOneMutation mutate;

    BinaryCrossover crossover;

    PolynamialMutation diversity;

    Population pop(Psize_), archive;
    size_t ffe = 0;

    WeightVectorInitialization(Psize_, prob.numObjectives());

    initialize(pop, prob);
    for (size_t i = 0; i < Psize_; ++i)
    {
        ch(pop[i]);
        prob.Evaluate(pop[i]);
        ffe++;

        UpdateArchive(archive, pop[i]);
    }
    UpdateReference(pop);
    UpdateNadir(pop);

    while (true)
    {
        log.All(archive);
        log.Detail(pop);

        if (ffe >= maxffe_)
        {
            break;
        }

        Individual children;

        for (size_t i = 0; i < Psize_; ++i)
        {
            mutate(children, pop, neighborIndice_[i], F_);

            crossover(children, pop[i], CR_);

            ch(children);
            prob.Evaluate(children);
            ffe++;

            UpdateReference(children.objs());

            UpdateNeighbor(pop, children, i);
        }

        UpdateNadir(pop);
        UpdateReference(pop);

        UpdateArchive(archive, pop);
    }

    sol = pop;
    log.RecordIGD();
}

void MOEADDE::WeightVectorInitialization(const size_t size, const size_t objs)
{
    weightVectors_.resize(size, vector<double>(objs));

    ifstream weightFile("./Weightvector/" + to_string(size) + "_" + to_string(objs) + ".txt");
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < objs; ++j)
        {
            weightFile >> weightVectors_[i][j];
        }
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

void MOEADDE::UpdateNeighbor(Population& pop, const Individual& ind, const size_t index) const
{
    size_t count = 0;

    vector<size_t> order(neighborhood_);
    RandomPermutation(order);

    for (size_t i = 0; i < neighborhood_; ++i)
    {
        if (count < maxReplace_)
        {
            size_t neighborIndex = neighborIndice_[index][order[i]];
            Individual& target = pop[neighborIndex];

            if (ind.feasible() && !target.feasible())
            {
                target = ind;
                count++;
            }
            else if (ind.feasible() && target.feasible())
            {
                double now = Tchebycheff(ind.objs(), weightVectors_[neighborIndex], referencePoint_, nadir_);
                double origin = Tchebycheff(target.objs(), weightVectors_[neighborIndex], referencePoint_, nadir_);

                if (now <= origin)
                {
                    target = ind;
                    count++;
                }
            }
            else if (!ind.feasible() && !target.feasible())
            {
                if (ind.violation() < target.violation())
                {
                    target = ind;
                    count++;
                }
            }
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

    vector<vector<size_t>> fronts = NondominatedSort(hybrid);

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