
#include <cmath>
#include <random>
#include <chrono>

#include "individual.h"
#include "tool.h"

using namespace std;

double DistanceNorm2(const vector<double>& l, const vector<double>& r)
{
    double sum = 0.0;
    for (size_t i = 0; i < l.size(); ++i)
    {
        sum += pow((l[i] - r[i]), 2);
    }

    return sqrt(sum);
}

int IntRandomNumber(const size_t num)
{
    uniform_int_distribution<int> dis(0, num - 1);
    return dis(gen);
}

void Permutation(vector<size_t>& order)
{
    for (size_t i = 0; i < order.size(); ++i)
    {
        order[i] = i;
    }
}

void RandomPermutation(vector<size_t>& order)
{
    Permutation(order);

    shuffle(order.begin(), order.end(), gen);
}

double Tchebycheff(const Individual& ind, const vector<double>& weights,
                   const vector<double>& reference, const vector<double>& nadir)
{
    double max = 0.0;

    for (size_t i = 0; i < ind.objs().size(); ++i)
    {
        double tmp = std::max(0.00001, weights[i]) * abs((ind.objs()[i] - reference[i]) / (nadir[i] - reference[i])) + 10000 * ind.violation();

        if (tmp > max)
        {
            max = tmp;
        }
    }

    return max;
}

double Tchebycheff(const Individual& ind, const vector<double>& weights, const vector<double>& reference)
{
    double max = 0.0;

    for (size_t i = 0; i < ind.objs().size(); ++i)
    {
        double tmp = std::max(0.00001, weights[i]) * abs((ind.objs()[i] - reference[i])) + 10000 * ind.violation();

        if (tmp > max)
        {
            max = tmp;
        }
    }

    return max;
}

double WeightedSum(const Individual& ind, const vector<double>& weights)
{
    double total = 0.0;

    for (size_t i = 0; i < ind.objs().size(); ++i)
    {
        total += weights[i] * ind.objs()[i];
    }

    return total;
}

size_t Tournament(const vector<double>& values, const size_t num)
{
    vector<size_t> index(values.size());
    RandomPermutation(index);

    size_t winner = 0;

    for (size_t i = 1; i < num; ++i)
    {
        if (values[index[i]] > values[index[winner]])
        {
            winner = i;
        }
    }

    return index[winner];
}