


#include "log.h"
#include "population.h"
#include "individual.h"

using namespace std;

Log::Log(const string& name, const int run)
{
    string fname = "/Output/" + name + "/" + to_string(run);
    trend_.open(fname + "_trend.txt", ios::out);
    final_.open(fname + "_final.txt", ios::out);
}

void Log::Trend(const Population& pop, const size_t numPareto)
{
    for (size_t i = 0; i < numPareto; ++i)
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

void Log::Final(const Population& pop)
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        final_ << pop[i] << endl;
    }

    return;
}