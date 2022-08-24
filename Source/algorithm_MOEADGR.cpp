


#include "algorithm_MOEADGR.h"
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

void MOEADGR::Solve(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;

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

            reproduct.PartNeigh(child, pop, neighborIndice_[i], params);
         
            mutate(child, 1.0 / double(prob.numVariables()), 20);

            ch(child);
            prob.Evaluate(child);
            ffe++;

            UpdateReference(child.objs());

            size_t near = FindSubproblem(child);
            UpdateNeighbor(pop, child, near);

            children.push_back(child);
        }
        //log.Child(children);
        children.clear();
    }

    sol = pop;
    log.RecordIGD();
}

size_t MOEADGR::FindSubproblem(const Individual& ind) const
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