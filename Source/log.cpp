
#include <algorithm>
#include <filesystem>

#include "log.h"
#include "population.h"
#include "individual.h"
#include "indicator.h"
#include "alg_sorting.h"
#include "alg_env_selection.h"

#define FINAL_SIZE 20

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
    BasicEnvSelection env;
    ofstream all(pname_ + "/all.avg", ios::out);

    Population paretoSet(FINAL_SIZE);

    size_t frontSize = env(paretoSet, pop);

    sort(paretoSet.begin(), paretoSet.begin() + frontSize, firstComp);

    for (size_t i = 0; i < frontSize; ++i)
    {
        all << "(";
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
        {
            all << paretoSet[i].objs()[j] << " ";
        }
        all << "), ";
    }
    all << "\n\n";

    Compromise comp;
    size_t index = comp(paretoSet);

    all << "Best Cost : " << paretoSet[0].objs()[0] << ", " << paretoSet[0].objs()[1] << endl;
    all << "Compromise : " << paretoSet[index].objs()[0] << ", " << paretoSet[index].objs()[1] << endl;
    all << "Best Emission : " << paretoSet[paretoSet.size() - 1].objs()[0] << ", " << paretoSet[paretoSet.size() - 1].objs()[1] << endl << endl;

    for (size_t i = 0; i < paretoSet.size(); ++i)
    {
        all << paretoSet[i] << endl;
    }

    //trend_ << "IGD : " << igd_(paretoPop) << endl;

    all.close();
    return;
}