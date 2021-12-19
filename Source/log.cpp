
#include <algorithm>
#include <filesystem>

#include "log.h"
#include "population.h"
#include "individual.h"
#include "indicator.h"
#include "alg_sorting.h"
#include "alg_env_selection.h"
#include "alg_sorting.h"

using namespace std;

bool firstComp(const Individual l, const Individual r)
{
    if (l.objs()[0] < r.objs()[0]) return true;
    else return false;
}

Log::Log(const string& name, const size_t run)
{
    igd_ = IGD(name);

    pname_ = "./Output/" + name;
    if (!filesystem::exists(pname_))
    {
        filesystem::create_directory(pname_);
    }

    final_ = run * 5;
}

void Log::All(const Population& pop)
{
    // Parameters.
    size_t numObjectives = Individual::prob().numObjectives();

    for (size_t i = 0; i < pop.size(); ++i)
    {
        trend_ << "(";
        for (size_t j = 0; j < numObjectives; ++j)
        {
            trend_ << pop[i].objs()[j] << " ";
        }
        trend_ << "),";
    }
    trend_ << endl;

    return;
}

void Log::Front(const Population& pop)
{
    Population paretoPop;
    vector<vector<size_t>> fronts = NondominatedSort(pop);
    for (size_t i = 0; i < fronts[0].size(); ++i)
    {
        paretoPop.push_back(pop[fronts[0][i]]);
    }

    sort(paretoPop.begin(), paretoPop.end(), firstComp);

    All(paretoPop);

    trend_ << "IGD : " << igd_(paretoPop) << endl;

    return;
}

void Log::Average(const Population& pop)
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

    //Feasible check.
    Population feasiblePop;
    for (size_t i = 0; i < pop.size(); ++i)
    {
        if (pop[i].Check(0.00001))
        {
            feasiblePop.push_back(pop[i]);
        }
    }

    // Pareto front.
    size_t frontSize;
    Population paretoSet(final_);

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
        all << "),";
    }
    all << "\n\n";

    // Compromise.
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
}

void Log::operator()(const int i)
{
    trend_.close();
    trend_.open(pname_ + "/" + to_string(i) + ".trend", ios::out);
}