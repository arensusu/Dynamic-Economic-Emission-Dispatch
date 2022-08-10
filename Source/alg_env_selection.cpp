
#include <vector>
#include <algorithm>

#include "alg_env_selection.h"
#include "alg_sorting.h"
#include "population.h"
#include "individual.h"

using namespace std;

size_t EnvSelection::Basic(Population& next, Population& curr)
{
    this->feasibility_ = false;

    return this->Select(next, curr);
}

size_t EnvSelection::CDP(Population& next, const Population& curr)
{
    this->feasibility_ = true;

    return this->Select(next, curr);
}

size_t EnvSelection::PF(Population& next, Population& curr)
{
    this->feasibility_ = false;

    for (size_t i = 0; i < curr.size(); ++i)
    {
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
        {
            curr[i].modifiedObjs()[j] = curr[i].objs()[j] + 10000 * curr[i].violation();
        }
    }

    return this->Select(next, curr);
}

size_t EnvSelection::Select(Population& next, const Population& curr) const
{
    vector<vector<size_t>> fronts;
    if (feasibility_ == false)
    {
        fronts = NondominatedSort(curr, Dominated);
    }
    else
    {
        fronts = NondominatedSort(curr, FeasibleDominated);
    }

    size_t remains = next.size();
    size_t rank = 0;
    
    while (remains > 0 && rank < fronts.size())
    {
        // Space is enough.
        if (remains > fronts[rank].size())
        {
            for (size_t i = 0; i < fronts[rank].size(); ++i)
            {
                size_t pos = next.size() - remains;
                size_t index = fronts[rank][i];

                next[pos] = curr[index];
                remains--;
            }

            rank++;
        }
        else
        {
            CrowdingDistanceSort(fronts[rank], curr);
            
            for (size_t i = 0; i < remains; ++i)
            {
                size_t pos = next.size() - remains + i;
                size_t index = fronts[rank][i];

                next[pos] = curr[index];
            }

            remains = 0;
        }
    }
    
    // Return the size of pareto fronts.
    return fronts[0].size() > next.size() ? next.size() : fronts[0].size();
}

size_t GreedyEnvSelection::Select(Population& next, const Population& curr) const
{
    if (feasibility_ == false)
    {
        for (size_t i = 0; i < next.size(); ++i)
        {
            if (Dominated(curr[i], next[i]))
            {
                next[i] = curr[i];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < next.size(); ++i)
        {
            if (FeasibleDominated(curr[i], next[i]))
            {
                next[i] = curr[i];
            }
        }
    }

    return 0;
}

bool FeasibleCompare(vector<double>& l, vector<double>& r)
{
    if (l[1] == 1 && r[1] == 0)
    {
        return true;
    }
    else if (l[1] == 0 && r[1] == 1)
    {
        return false;
    }
    else
    {
        if (l[0] < r[0])
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void OneDimEnvSelection::operator()(Population& children, const Population& parents, const size_t obj) const
{
    vector<vector<double>> values(parents.size(), vector<double>(3));

    for (size_t i = 0; i < values.size(); ++i)
    {
        values[i][0] = parents[i].objs()[obj];
        values[i][1] = parents[i].feasible();
        values[i][2] = i;
    }

    sort(values.begin(), values.end(), FeasibleCompare);

    for (size_t i = 0; i < children.size(); ++i)
    {
        children[i] = parents[values[i][2]];
    }
}

Population SPEA2EnvSelection::operator()(const Population& archive, const Population& pop) const
{
    Population tmp(archive.size());

    size_t totalSize = archive.size() + pop.size();

    vector<vector<double>> list(totalSize, vector<double>(2));
    for (size_t i = 0; i < totalSize; ++i)
    {
        const Individual* ind;
        if (i < archive.size())
        {
            ind = &(archive[i]);
        }
        else
        {
            ind = &(pop[i - archive.size()]);
        }

        list[i][0] = ind->fitness();
        list[i][1] = i;
    }

    sort(list.begin(), list.end());

    size_t count = 0;
    size_t i = 0;
    while (count < tmp.size())
    {
        if (list[i][0] != -1)
        {
            const Individual* ind;
            if (list[i][1] < archive.size())
            {
                ind = &(archive[list[i][1]]);
            }
            else
            {
                ind = &(pop[list[i][1] - archive.size()]);
            }

            tmp[count] = *ind;

            count++;
        }
        i++;
    }

    return tmp;
}