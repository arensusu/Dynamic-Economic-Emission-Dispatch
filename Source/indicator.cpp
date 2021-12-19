
#include <fstream>
#include <limits>
#include <cmath>
#include <algorithm>

#include "indicator.h"
#include "population.h"

using namespace std;

IGD::IGD(const string& name)
{
    string fname = "./Indicator/IGD/" + name + ".igd";
    ifstream file(fname, ios::in);

    size_t num;
    file >> num;

    double obj1, obj2;
    for (size_t i = 0; i < num; ++i)
    {
        file >> obj1 >> obj2;
        refs_.push_back({ obj1, obj2 });
    }
}

double IGD::operator()(const Population& pop)
{
    double igd = 0;

    if (pop.size() == 0)
    {
        return 1;
    }

    double Cmin = refs_[0][0];
    double Cmax = refs_[refs_.size() - 1][0];
    double Emin = refs_[refs_.size() - 1][1];
    double Emax = refs_[0][1];

    for (size_t i = 0; i < refs_.size(); ++i)
    {        
        double min = numeric_limits<double>::max();
        for (size_t j = 0; j < pop.size(); ++j)
        {
            double Cdiff = (pop[j].objs()[0] - refs_[i][0]) / (Cmax - Cmin);
            double Ediff = (pop[j].objs()[1] - refs_[i][1]) / (Emax - Emin);
            
            double dis = sqrt(pow(Cdiff, 2) + pow(Cdiff, 2));

            if (dis < min)
            {
                min = dis;
            }
        }

        igd += min;
    }

    val_ = igd / pop.size();

    return val_;
}

size_t Compromise::operator()(const Population& pop) const
{
    // Parameters.
    size_t numObjectives = Individual::prob().numObjectives();

    vector<double> fmin(2);
    vector<double> fmax(2);

    for (size_t i = 0; i < numObjectives; ++i)
    {
        double min = pop[0].objs()[i];
        double max = min;
        double cur;

        for (size_t j = 1; j < pop.size(); ++j)
        {
            cur = pop[j].objs()[i];

            if (cur > max)
            {
                max = cur;
            }

            if (cur < min)
            {
                min = cur;
            }
        }

        fmin[i] = min;
        fmax[i] = max;
    }

    vector<double> degree(pop.size(), 0);
    double degreeSum = 0;

    for (size_t i = 0; i < pop.size(); ++i)
    {
        double cur;
        for (size_t j = 0; j < numObjectives; ++j)
        {
            cur = pop[i].objs()[j];

            if (cur <= fmin[j])
            {
                degree[i] += 1;
            }
            else if (cur >= fmax[j])
            {
                degree[i] += 0;
            }
            else
            {
                degree[i] += ((fmax[j] - cur) / (fmax[j] - fmin[j]));
            }
        }

        degreeSum += degree[i];
    }

    for (size_t i = 0; i < pop.size(); ++i)
    {
        degree[i] = degree[i] / degreeSum;
    }

    return max_element(degree.begin(), degree.end()) - degree.begin();
}