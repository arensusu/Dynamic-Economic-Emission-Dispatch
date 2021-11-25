
#include <fstream>
#include <limits>
#include <cmath>

#include "indicator.h"
#include "population.h"

using namespace std;

IGD::IGD(const string& name)
{
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