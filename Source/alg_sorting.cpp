
#include <limits>
#include <algorithm>
#include <vector>

#include "alg_sorting.h"
#include "population.h"
#include "individual.h"
#include "problem_base.h"

using namespace std;

bool Dominated(const Individual& l, const Individual& r)
{
    bool better = false;
    for (size_t i = 0; i < l.objs().size(); ++i)
    {
        if (l.objs()[i] > r.objs()[i])
        {
            return false;
        }
        else if (l.objs()[i] < r.objs()[i])
        {
            better = true;
        }
    }
    return better;
}

vector<vector<size_t>> NondominatedSort(const Population& pop)
{
    vector<vector<size_t>> fronts;
    vector<size_t> curFront;

    vector<vector<size_t>> dominatedSet(pop.size());
    vector<int> beDominated(pop.size(), 0);

    for (size_t i = 0; i < pop.size(); ++i)
    {
        for (size_t j = 0; j < pop.size(); ++j)
        {
            if (i == j) continue;

            if (Dominated(pop[i], pop[j]))
            {
                dominatedSet[i].push_back(j);
            }
            else if (Dominated(pop[j], pop[i]))
            {
                beDominated[i]++;
            }
        }

        if (beDominated[i] == 0)
        {
            curFront.push_back(i);
        }
    }

    int rank = 1;

    while (curFront.size() != 0)
    {
        fronts.push_back(curFront);
        size_t prevFrontSize = curFront.size();
        curFront.clear();

        for (size_t i = 0; i < prevFrontSize; ++i)
        {
            vector<size_t>& curSet = dominatedSet[fronts[rank - 1][i]];
            for (size_t j = 0; j < curSet.size(); ++j)
            {
                if ((--beDominated[curSet[j]]) == 0)
                {
                    curFront.push_back(curSet[j]);
                }
            }
        }
        rank++;
    }

    return fronts;
}



void CrowdingDistanceSort(vector<size_t> &front, const Population &pop)
{
    vector<vector<double>> objIndex(front.size(), vector<double>(3));
    for (size_t i = 0; i < front.size(); ++i)
    {
        objIndex[i][0] = 0;
        objIndex[i][1] = front[i];
        objIndex[i][2] = 0;
    }
    
    for (size_t i = 0; i < Individual::prob().numObjectives(); ++i)
    {
        for (size_t j = 0; j < objIndex.size(); ++j)
        {
            objIndex[j][0] = pop[size_t(objIndex[j][1])].objs()[i];
        }

        sort(objIndex.begin(), objIndex.end());

        objIndex[0][2] = numeric_limits<double>::max();
        objIndex[objIndex.size() - 1][2] = numeric_limits<double>::max();
        for (size_t j = 1; j < objIndex.size() - 1; ++j)
        {
            objIndex[j][2] += (objIndex[j + 1][0] - objIndex[j - 1][0]) / (objIndex[objIndex.size() - 1][0] - objIndex[0][0]);
        }
    }

    for (size_t i = 0; i < objIndex.size(); ++i)
    {
        objIndex[i][0] = objIndex[i][2];
    }

    sort(objIndex.begin(), objIndex.end());

    for (size_t i = 0; i < objIndex.size(); ++i)
    {
        front[i] = size_t(objIndex[objIndex.size() - 1 - i][1]);
    }
}