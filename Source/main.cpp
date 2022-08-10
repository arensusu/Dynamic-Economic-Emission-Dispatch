#include <iostream>
#include <fstream>
#include <cstdlib>

#include "problem_base.h"
#include "population.h"
#include "algorithm_base.h"
#include "log.h"
#include "experiment.h"
#include "indicator.h"

#include <vector>
#include <ctime>
#include "individual.h"
#include "alg_sorting.h"
#include "tool.h"

using namespace std;

void TestEvaluated(const BProblem* prob)
{
    Individual::SetProblem(*prob);
    Individual ind;
    vector<double> enc(prob->numVariables());

    ifstream file("test.txt", ios::in);
    for (size_t i = 0; i < prob->numPeriods(); ++i)
    {
        for (size_t j = 0; j < prob->numMachines(); ++j)
        {
            double x;
            file >> x;
            enc[i * prob->numMachines() + j] = x;
        }
    }

    ind.Encoder(enc);
    prob->Evaluate(ind);
    prob->Check(ind, 0.1);

    if (!ind.feasible())
    {
        const vector<double> output = ind.PowerOutput();
        for (size_t i = 0; i < output.size(); ++i)
        {
            cout << output[i] << " ";
        }
        cout << endl;

        cout << ind.violation() << endl;
    }

    cout << ind.objs()[0] << " " << ind.objs()[1] << endl;
}

void TestDominated()
{
    Population pop;
    pop.resize(10);

    pop[0].objs() = vector<double>({ 4, 5 });
    pop[1].objs() = vector<double>({ 3, 3 });
    pop[2].objs() = vector<double>({ 2, 5 });
    pop[3].objs() = vector<double>({ 5, 0 });
    pop[4].objs() = vector<double>({ 6, 3 });
    pop[5].objs() = vector<double>({ 1, 4 });
    pop[6].objs() = vector<double>({ 2, 2 });
    pop[7].objs() = vector<double>({ 0, 6 });
    pop[8].objs() = vector<double>({ 3, 1 });
    pop[9].objs() = vector<double>({ 4, 2 });

    vector<vector<size_t>> front = NondominatedSort(pop, FeasibleDominated);
}

#include "alg_initialization.h"
#include "alg_constraint_handling.h"
void RepairTest(const vector<string>& probList)
{
    for (size_t i = 0; i < probList.size(); ++i)
    {
        BProblem* prob = nullptr;
        SetProblemType(&prob, probList[i]);

        Individual::SetProblem(*prob);

        int repairTimes = 0;

        RandomInitialization init;
        ProportionDivisionCH ch;

        Population pop(5000);
        init(pop);
        for (size_t i = 0; i < pop.size(); ++i)
        {
            repairTimes += ch(pop[i], 40);
        }

        cout << double(repairTimes) << endl;
    }
}

default_random_engine gen;

int main()
{
    //TestDominated();

    ifstream exp("exp.ini", ios::in);
    vector<string> probList;
    BaseEA* ea = nullptr;
    SetExperiment(&ea, probList, exp);
    exp.close();

    //RepairTest(probList);
    
    //BProblem* prob = nullptr;
    //SetProblemType(&prob, "6_24_WB");
    //TestEvaluated(prob);
    
    for (size_t i = 0; i < probList.size(); ++i)
    {
        BProblem* prob = nullptr;
        SetProblemType(&prob, probList[i]);
        IGD igd(probList[i]);

        Individual::SetProblem(*prob);

        Population avg;

        int RUN = 20;
        Log log(probList[i], RUN);

        for (int r = 0; r < RUN; ++r)
        {
            gen.seed(r);
            log(r);

            Population sol;
            ea->Solve(sol, *prob, log);

            avg.push_back(sol);
            cout << "Run " << r << " finished." << endl;
        }

        for (size_t i = 0; i < avg.size(); ++i)
        {
            prob->Check(avg[i], 1);
        }

        log.Average(avg);
        cout << "Problem " << probList[i] << " finished." << endl;
    }
    
}