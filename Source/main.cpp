#include <iostream>
#include <fstream>

#include "problem_base.h"
#include "population.h"
#include "algorithm_base.h"
#include "log.h"
#include "experiment.h"
#include "indicator.h"

#include <vector>
#include "individual.h"
#include "alg_sorting.h"

using namespace std;

void TestEvaluated(const BProblem* prob)
{
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

    vector<double> objs;
    prob->Evaluate(objs, enc);

    cout << objs[0] << " " << objs[1] << endl;
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

    vector<vector<size_t>> front = NondominatedSort(pop);
}

int main()
{
    //TestDominated();
    
    ifstream exp("exp.ini", ios::in);
    vector<string> probList;
    BaseEA* ea = nullptr;
    SetExperiment(&ea, probList, exp);
    exp.close();

    for (size_t i = 0; i < probList.size(); ++i)
    {
        BProblem* prob = nullptr;
        SetProblemType(&prob, probList[i]);
        IGD igd(probList[i]);
        
        //TestEvaluated(prob);
        
        Individual::SetProblem(*prob);
        
        Population avg;
        Log log(probList[i]);

        int RUN = 20;
        for (int r = 0; r < RUN; ++r)
        {
            log(r);
            Population sol;
            ea->Solve(sol, *prob, log);

            avg.push_back(sol);
            cout << "Run " << r << " finished." << endl;
        }
        
        log.FinalFront(avg);
    }
    
}