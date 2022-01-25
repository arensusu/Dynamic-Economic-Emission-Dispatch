
#include <cmath>
#include <random>
#include <chrono>

#include "tool.h"

using namespace std;

double DistanceNorm2(vector<double>& l, vector<double>& r)
{
    double sum = 0.0;
    for (size_t i = 0; i < l.size(); ++i)
    {
        sum += pow((l[i] - r[i]), 2);
    }

    return sqrt(sum);
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

    shuffle(order.begin(), order.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));
}

double Tchebycheff(const vector<double>& objs, const vector<double>& weights,
                   const vector<double>& reference, const vector<double>& nadir)
{
    double max = 0;

    for (size_t i = 0; i < objs.size(); ++i)
    {
        double tmp = weights[i] * abs((objs[i] - reference[i]) / (nadir[i] - reference[i]));

        if (tmp > max)
        {
            max = tmp;
        }
    }

    return max;
}