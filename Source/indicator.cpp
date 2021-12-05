
#include <fstream>
#include <limits>
#include <cmath>
#include <algorithm>

#include "indicator.h"
#include "population.h"

using namespace std;

IGD::IGD(const string& name)
{/*
    string fname = "./Indicator/IGD/" + name;
    ifstream file(fname, ios::in);

    size_t num;
    file >> num;

    double obj1, obj2;
    for (size_t i = 0; i < num; ++i)
    {
        file >> obj1 >> obj2;
        refs_.push_back({ obj1, obj2 });
    }
*/
}

double IGD::operator()(const Population& pop) const
{
    double igd = 0;
    for (size_t i = 0; i < refs_.size(); ++i)
    {        
        double min = numeric_limits<double>::max();
        for (size_t j = 0; j < pop.size(); ++j)
        {
            double obj1 = pop[j].objs()[0];
            double obj2 = pop[j].objs()[1];
            
            double dis = pow(pow(obj1 - refs_[i][0], 2) + pow(obj2 - refs_[i][1], 2), 0.5);

            if (dis < min)
            {
                min = dis;
            }
        }

        igd += min;
    }

    return igd / pop.size();
}

size_t Compromise::operator()(const Population& pop) const
{
    vector<double> fmin(2);
    vector<double> fmax(2);

    for (size_t i = 0; i < Individual::prob().numObjectives(); ++i)
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
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
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