
#include <algorithm>

#include "log.h"
#include "population.h"
#include "individual.h"

using namespace std;

bool firstComp(const Individual l, const Individual r)
{
    if (l.objs()[0] < r.objs()[0]) return true;
    else return false;
}

Log::Log(const string& name, const int run)
{
    string fname = "./Output/" + name + "/" + to_string(run);
    trend_.open(fname + "_trend.txt", ios::out);
    final_.open(fname + "_final.txt", ios::out);
}

void Log::All(const Population& pop)
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        trend_ << "(";
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
        {
            trend_ << pop[i].objs()[j] << " ";
        }
        trend_ << "), ";
    }
    trend_ << endl;

    return;
}

void Log::Trend(const Population& pop, const size_t numPareto)
{
    Population paretoPop(pop.begin(), pop.begin() + numPareto);
    sort(paretoPop.begin(), paretoPop.end(), firstComp);

    for (size_t i = 0; i < numPareto; ++i)
    {
        trend_ << "(";
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
        {
            trend_ << paretoPop[i].objs()[j] << " ";
        }
        trend_ << "), ";
    }
    trend_ << endl;

    return;
}

void Log::Final(const Population& pop)
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        final_ << pop[i] << endl;
    }

    return;
}