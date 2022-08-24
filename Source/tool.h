#ifndef TOOL__
#define TOOL__

#include <vector>
#include <random>

class Individual;

extern std::default_random_engine gen;

double DistanceNorm2(const std::vector<double>& l, const std::vector<double>& r);

int IntRandomNumber(const std::size_t num);

void Permutation(std::vector<std::size_t>& order);

void RandomPermutation(std::vector<std::size_t>& order);

double Tchebycheff(const Individual& ind, const std::vector<double>& weights,
                   const std::vector<double>& reference, const std::vector<double>& nadir);

double Tchebycheff(const Individual& ind, const std::vector<double>& weights, const std::vector<double>& reference);

double WeightedSum(const Individual& ind, const std::vector<double>& weights,
                   const std::vector<double>& reference, const std::vector<double>& nadir);

double WeightedSum(const Individual& ind, const std::vector<double>& weights);

std::size_t Tournament(const std::vector<double>& values, const std::size_t num);

#endif
