
#include <algorithm>
#include <filesystem>

#include "log.h"
#include "population.h"
#include "individual.h"
#include "indicator.h"
#include "alg_sorting.h"

using namespace std;

bool firstComp(const Individual l, const Individual r)
{
    if (l.objs()[0] < r.objs()[0]) return true;
    else return false;
}

Log::Log(const string& name)
{
    igd_ = IGD(name);

    pname_ = "./Output/" + name;
    if (!filesystem::exists(pname_))
    {
        filesystem::create_directory(pname_);
    }
}

void Log::operator()(const int i)
{
    trend_.close();
    trend_.open(pname_ + "/" + to_string(i) + ".trend", ios::out);
}

void Log::All(const Population& pop)
{
    size_t half = pop.size() / 2;
    for (size_t i = 0; i < pop.size(); ++i)
    {
        if (i == half) trend_ << endl;

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

void Log::FinalFront(const Population& pop)
{
    ofstream all(pname_ + "/all.avg", ios::out);

    vector<size_t> fronts = NondominatedSort(pop)[0];

    Population paretoSet;
    for (size_t i = 0; i < fronts.size(); ++i)
    {
        paretoSet.push_back(pop[fronts[i]]);
    }

    sort(paretoSet.begin(), paretoSet.end(), firstComp);

    for (size_t i = 0; i < paretoSet.size(); ++i)
    {
        all << "(";
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
        {
            all << paretoSet[i].objs()[j] << " ";
        }
        all << "), ";
    }
    all << "\n\n";

    for (size_t i = 0; i < paretoSet.size(); ++i)
    {
        all << paretoSet[i] << endl;
    }

    //trend_ << "IGD : " << igd_(paretoPop) << endl;

    all.close();
    return;
}