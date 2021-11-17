
#include <vector>
#include <algorithm>

#include "alg_env_selection.h"
#include "alg_sorting.h"
#include "population.h"
#include "individual.h"

using namespace std;

size_t BasicEnvSelection::operator()(Population& children, Population& parents) const
{
    vector<vector<size_t>> fronts = NondominatedSort(parents);

    size_t remains = children.size();
    size_t rank = 0;
    
    while (remains > 0)
    {
        if (remains > fronts[rank].size())
        {
            for (size_t i = 0; i < fronts[rank].size(); ++i)
            {
                children[children.size() - remains] = parents[fronts[rank][i]];
                remains--;
            }
        }
        else
        {
            CrowdingDistanceSort(fronts[rank], parents);
            
            for (size_t i = 0; i < remains; ++i)
            {
                children[children.size() - remains + i] = parents[fronts[rank][i]];
            }
            remains = 0;
        }
    }
    
    return fronts[0].size() > children.size() ? children.size() : fronts[0].size();
}