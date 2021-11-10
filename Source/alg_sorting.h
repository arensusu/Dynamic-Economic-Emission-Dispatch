#ifndef NS__
#define NS__

#include <vector>

class Individual;
class Population;

bool Dominated(const Individual& l, const Individual& r);

std::vector<std::vector<std::size_t>> NondominatedSort(const Population&);

void CrowdingDistanceSort(std::vector<std::size_t>&, const Population&);

#endif