
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
    Population paretoPop;
    for (size_t i = 0; i < numPareto; ++i)
    {
        //if (pop[i].Check())
        {
            paretoPop.push_back(pop[i]);
        }
    }

    sort(paretoPop.begin(), paretoPop.end(), firstComp);

    for (size_t i = 0; i < paretoPop.size(); ++i)
    {
        trend_ << "(";
        for (size_t j = 0; j < Individual::prob().numObjectives(); ++j)
        {
            trend_ << paretoPop[i].objs()[j] << " ";
        }
        trend_ << "), ";
    }
    trend_ << endl;

    trend_ << "IGD : " << igd_(paretoPop) << endl;

    return;
}

void Log::FinalFront(const Population& pop)
{
    ofstream all(pname_ + "/all.avg", ios::out);

    //igd
    sort(igdVals_.begin(), igdVals_.end());

    double avgIgd = 0;
    for (size_t i = 0; i < igdVals_.size(); ++i)
    {
        avgIgd += igdVals_[i];
    }
    avgIgd = avgIgd / igdVals_.size();

    all << "Min_IGD : " << igdVals_[0] << endl;
    all << "Mean_IGD : " << (igdVals_[igdVals_.size() / 2] + igdVals_[igdVals_.size() / 2 + 1]) / 2 << endl;
    all << "Max_IGD : " << igdVals_[igdVals_.size() - 1] << endl;
    all << "Avg_IGD : " << avgIgd << endl;

    //front
    Population feasiblePop;
    for (size_t i = 0; i < pop.size(); ++i)
    {
        if (pop[i].Check())
        {
            feasiblePop.push_back(pop[i]);
        }
    }

    size_t frontSize;
    Population paretoSet(FINAL_SIZE);

    BasicEnvSelection env;

    if (feasiblePop.size() == 0)
    {
        frontSize = env(paretoSet, pop);
    }
    else
    {
        frontSize = env(paretoSet, feasiblePop);
    }
    
    paretoSet.resize(frontSize);

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

    all.close();
    return;
}