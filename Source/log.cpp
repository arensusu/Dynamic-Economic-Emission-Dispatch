
#include <algorithm>
#include <filesystem>

#include "tool.h"
#include "log.h"
#include "population.h"
#include "individual.h"
#include "indicator.h"
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
    trend_ << igd_(pop) << endl;
}

void Log::Front(const Population& pop)
{
    Population paretoPop;
    vector<vector<size_t>> fronts = NondominatedSort(pop, FeasibleDominated);
    for (size_t i = 0; i < fronts[0].size(); ++i)
    {
        paretoPop.push_back(pop[fronts[0][i]]);
    }

    sort(paretoPop.begin(), paretoPop.end(), firstComp);

    All(paretoPop);

    trend_ << "IGD : " << igd_(paretoPop) << endl;
}

void Log::Detail(const Population& pop)
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        detail_ << pop[i];
    }
    detail_ << endl;
}

void Log::OpCount(const vector<int>& counts)
{
    for (size_t i = 0; i < counts.size(); ++i)
    {
        op_ << counts[i] << " ";
    }
    op_ << endl;
}

void Log::OpCount(const vector<double>& counts)
{
    for (size_t i = 0; i < counts.size(); ++i)
    {
        op_ << counts[i] << " ";
    }
    op_ << endl;
}

void Log::Child(const Population& pop)
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        child_ << pop[i];
    }
    child_ << endl;
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

    // Pareto front.
    EnvSelection env;
    
    vector<vector<size_t>> fronts = NondominatedSort(pop, FeasibleDominated);

    Population paretoSet(fronts[0].size());

    env.CDP(paretoSet, pop);

    sort(paretoSet.begin(), paretoSet.begin() + paretoSet.size(), firstComp);

    for (size_t i = 0; i < paretoSet.size(); ++i)
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

    all << "Best-Cost : " << paretoSet[0].objs()[0] << ", " << paretoSet[0].objs()[1] << endl;
    all << "Compromise : " << paretoSet[index].objs()[0] << ", " << paretoSet[index].objs()[1] << endl;
    all << "Best-Emission : " << paretoSet[paretoSet.size() - 1].objs()[0] << ", " << paretoSet[paretoSet.size() - 1].objs()[1] << endl << endl;

    all << "Feasible : " ;
    for (size_t i = 0; i < paretoSet.size(); ++i)
    {
        all << paretoSet[i].feasible();
    }
    all << endl;

    for (size_t i = 0; i < paretoSet.size(); ++i)
    {
        all << paretoSet[i] << endl;
    }

    all.close();
}

void Log::operator()(const int i)
{
    trend_.close();
    trend_.open(pname_ + "/" + to_string(i) + ".igd", ios::out);

    detail_.close();
    detail_.open(pname_ + "/" + to_string(i) + ".detail", ios::out);

    op_.close();
    op_.open(pname_ + "/" + to_string(i) + ".op", ios::out);

    child_.close();
    child_.open(pname_ + "/" + to_string(i) + ".child", ios::out);
}