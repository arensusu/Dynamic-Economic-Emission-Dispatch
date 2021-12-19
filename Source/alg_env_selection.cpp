
#include <vector>
#include <algorithm>

#include "alg_env_selection.h"
#include "alg_sorting.h"
#include "population.h"
#include "individual.h"

using namespace std;

size_t BasicEnvSelection::operator()(Population& children, const Population& parents) const
{
    vector<vector<size_t>> fronts = NondominatedSort(parents);

    size_t remains = children.size();
    size_t rank = 0;
    
    while (remains > 0 && rank < fronts.size())
    {
        // Space is enough.
        if (remains > fronts[rank].size())
        {
            for (size_t i = 0; i < fronts[rank].size(); ++i)
            {
                size_t pos = children.size() - remains;
                size_t index = fronts[rank][i];

                children[pos] = parents[index];
                remains--;
            }

            rank++;
        }
        else
        {
            CrowdingDistanceSort(fronts[rank], parents);
            
            for (size_t i = 0; i < remains; ++i)
            {
                size_t pos = children.size() - remains + i;
                size_t index = fronts[rank][i];

                children[pos] = parents[index];
            }

            remains = 0;
        }
    }
    
    // Return the size of pareto fronts.
    return fronts[0].size() > children.size() ? children.size() : fronts[0].size();
}