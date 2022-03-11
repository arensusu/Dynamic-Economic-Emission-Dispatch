
#include <vector>
#include <random>
#include <chrono>

#include "localsearch.h"
#include "population.h"
#include "alg_sorting.h"
#include "alg_constraint_handling.h"
#include "problem_base.h"
#include "indicator.h"

#include "tool.h"

using namespace std;

bool OneDimLS::operator()(Population& pop) const
{
    ProportionDivisionCH ch;
    Compromise comp;

    vector<vector<size_t>> fronts = NondominatedSort(pop);

    uniform_real_distribution<double> dis(0.0, 1.0);

    size_t minEcoIndex = fronts[0][0];
    size_t minEmiIndex = fronts[0][0];

    for (size_t i = 1; i < fronts[0].size(); ++i)
    {
        if (pop[fronts[0][i]].objs()[0] < pop[minEcoIndex].objs()[0])
        {
            minEcoIndex = fronts[0][i];
        }

        if (pop[fronts[0][i]].objs()[1] < pop[minEmiIndex].objs()[1])
        {
            minEmiIndex = fronts[0][i];
        }
    }

    size_t num = 0;
    for (size_t i = 0; i < fronts[0].size(); ++i)
    {
        if (fronts[0][i] != minEcoIndex && fronts[0][i] != minEmiIndex)
        {
            const Individual* best;
            //if (dis(gen) < 0.5)
            //{
            //    best = &(pop[minEcoIndex]);
            //}
            //else
            //{
            //    best = &(pop[minEmiIndex]);
            //}
            best = &(pop[comp(pop)]);

            Individual target = pop[fronts[0][i]];
            for (size_t j = 0; j < Individual::prob().numVariables(); ++j)
            {
                if (dis(gen) < 0.5)
                {
                    target[j] = (*best)[j];
                }
            }

            ch(target);
            Individual::prob().Evaluate(target);
            num++;

            if (Dominated(target, pop[fronts[0][i]]))
            {
                pop[fronts[0][i]] = target;
            }
        }
    }

    return num;
}