#ifndef NS__
#define NS__

#include <vector>

class Individual;
class Population;

bool Dominated(const Individual& l, const Individual& r);

bool FeasibleDominated(const Individual& l, const Individual& r);

std::vector<std::vector<std::size_t>> NondominatedSort(const Population& pop);

void CrowdingDistanceSort(std::vector<std::size_t>& fronts, const Population& pop);

void CrowdingEntropySort(std::vector<std::size_t>& fronts, const Population& pop);

#endif