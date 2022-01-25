
#include <vector>
#include <random>
#include <chrono>

#include "alg_mating_selection.h"
#include "tool.h"

using namespace std;

vector<size_t> RandomMatingSelection::operator()(const vector<size_t>& pool, const size_t num) const
{
    vector<size_t> index(pool.size());
    RandomPermutation(index);

    vector<size_t> chosen(num);
    for (size_t i = 0; i < num; ++i)
    {
        chosen[i] = pool[index[i]];
    }

    return chosen;
}