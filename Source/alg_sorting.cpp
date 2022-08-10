
#include <limits>
#include <algorithm>
#include <vector>

#include "alg_sorting.h"
#include "population.h"
#include "individual.h"
#include "problem_base.h"

using namespace std;

// Left dominated right.
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

bool FeasibleDominated(const Individual& l, const Individual& r)
{
    bool better = false;

    bool feasibleL = l.feasible();
    bool feasibleR = r.feasible();

    if (feasibleL && !feasibleR)
    {
        better = true;
    }
    else if (!feasibleL && feasibleR)
    {
        better = false;
    }
    else if (!feasibleL && !feasibleR)
    {
        if (l.violation() < r.violation())
        {
            better = true;
        }
    }
    else
    {
        better = Dominated(l, r);
    }

    return better;
}

vector<vector<size_t>> NondominatedSort(const Population& pop, bool (*Dominate)(const Individual&, const Individual&))
{
    vector<vector<size_t>> fronts;
    vector<size_t> curFront;

    vector<vector<size_t>> dominatedSet(pop.size());

    vector<size_t> beDominated(pop.size(), 0);

    for (size_t i = 0; i < pop.size(); ++i)
    {
        for (size_t j = 0; j < pop.size(); ++j)
        {
            if (i == j) continue;

            if (Dominate(pop[i], pop[j]))          // I dominate the other.
            {
                dominatedSet[i].push_back(j);
            }
            else if (Dominate(pop[j], pop[i]))     // The orther dominates me.
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
        // Store the completed front.
        fronts.push_back(curFront);
        size_t prevFrontSize = curFront.size();
        curFront.clear();

        // Remove the previous front and construct the new one.
        for (size_t i = 0; i < prevFrontSize; ++i)
        {
            size_t index = fronts[rank - 1][i];
            vector<size_t>& curSet = dominatedSet[index];

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



void CrowdingDistanceSort(vector<size_t>& front, const Population& pop)
{
    // Parameters.
    size_t numObjectives = Individual::prob().numObjectives();

    vector<vector<double>> objIndex(front.size(), vector<double>(3));
    for (size_t i = 0; i < front.size(); ++i)
    {
        objIndex[i][0] = 0;
        objIndex[i][1] = front[i];
        objIndex[i][2] = 0;
    }
    
    for (size_t i = 0; i < numObjectives; ++i)
    {
        // Get one of the objectives.
        for (size_t j = 0; j < objIndex.size(); ++j)
        {
            size_t index = size_t(objIndex[j][1]);

            objIndex[j][0] = pop[index].objs()[i];
        }

        sort(objIndex.begin(), objIndex.end());

        // Endpoints are infinite.
        objIndex[0][2] = numeric_limits<double>::max();
        objIndex[objIndex.size() - 1][2] = numeric_limits<double>::max();

        // Euclidean distance.
        for (size_t j = 1; j < objIndex.size() - 1; ++j)
        {
            objIndex[j][2] += (objIndex[j + 1][0] - objIndex[j - 1][0]) / (objIndex[objIndex.size() - 1][0] - objIndex[0][0]);
        }
    }

    // Sort by crowding distance.
    for (size_t i = 0; i < objIndex.size(); ++i)
    {
        objIndex[i][0] = objIndex[i][2];
    }

    sort(objIndex.begin(), objIndex.end());

    for (size_t i = 0; i < objIndex.size(); ++i)
    {
        size_t index = size_t(objIndex.size() - 1 - i);

        front[i] = size_t(objIndex[index][1]);
    }
}

void CrowdingEntropySort(vector<size_t>& front, const Population& pop)
{
    // Parameters.
    size_t numObjectives = Individual::prob().numObjectives();

    vector<vector<double>> objIndex(front.size(), vector<double>(3));
    for (size_t i = 0; i < front.size(); ++i)
    {
        objIndex[i][0] = pop[front[i]].objs()[0];
        objIndex[i][1] = front[i];
        objIndex[i][2] = pop[front[i]].objs()[1];
    }

    sort(objIndex.begin(), objIndex.end());

    // Euclidean distance.
    double sum = 0;
    for (size_t j = 0; j < objIndex.size() - 1; ++j)
    {
        double dl = (objIndex[j + 1][0] - objIndex[j][0]) / (objIndex[objIndex.size() - 1][0] - objIndex[0][0]) * 100.0;
        dl += (objIndex[j + 1][2] - objIndex[j][2]) / (objIndex[objIndex.size() - 1][2] - objIndex[0][2]) * 100.0;

        if (j != 0)
        {
            objIndex[j][2] = dl;
        }

        sum += dl;
    }

    for (size_t j = 0; j < objIndex.size(); ++j)
    {
        double tmp = objIndex[j][2] / sum;
        objIndex[j][2] = -1 * tmp * log2(tmp);
    }

    // Endpoints are infinite.
    objIndex[0][2] = 1;
    objIndex[objIndex.size() - 1][2] = 1;

    // Sort by crowding distance.
    for (size_t i = 0; i < objIndex.size(); ++i)
    {
        objIndex[i][0] = objIndex[i][2];
    }

    sort(objIndex.begin(), objIndex.end());

    for (size_t i = 0; i < objIndex.size(); ++i)
    {
        size_t index = size_t(objIndex.size() - 1 - i);

        front[i] = size_t(objIndex[index][1]);
    }
}