#ifndef TOOL__
#define TOOL__

#include <vector>

double DistanceNorm2(std::vector<double>& l, std::vector<double>& r);

void Permutation(std::vector<std::size_t>& order);

void RandomPermutation(std::vector<std::size_t>& order);

double Tchebycheff(const std::vector<double>& objs, const std::vector<double>& weights,
                   const std::vector<double>& reference, const std::vector<double>& nadir);

#endif
