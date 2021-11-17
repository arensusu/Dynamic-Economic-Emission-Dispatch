#include <iostream>
#include <fstream>

#include "problem_base.h"
#include "population.h"
#include "algorithm_base.h"
#include "log.h"
#include "experiment.h"

#include <vector>
#include "individual.h"

using namespace std;

void TestEvaluated(const BProblem* prob)
{
    Individual::SetProblem(*prob);
    Individual ind;
    vector<double>& enc = ind.encoding();
    enc.resize(prob->numPeriods() * prob->numMachines());
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
    prob->Evaluate(ind);

    cout << ind.objs()[0] << " " << ind.objs()[1] << endl;
}

int main()
{
    ifstream exp("exp.ini", ios::in);
    vector<string> probList;
    BaseEA* ea = nullptr;
    SetExperiment(&ea, probList, exp);
    exp.close();

    for (size_t i = 0; i < probList.size(); ++i)
    {
        BProblem* prob = nullptr;
        SetProblemType(&prob, probList[i]);
        TestEvaluated(prob);
        /*
        int RUN = 20;
        for (int r = 0; r < RUN; ++r)
        {
            Log log(probList[i], r);
            Population sol;
            ea->Solve(sol, *prob, log);
        }
        */
    }
}