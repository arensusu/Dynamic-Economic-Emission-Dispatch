#include <iostream>
#include <fstream>

#include "problem_base.h"
#include "population.h"
#include "algorithm_base.h"
#include "log.h"
#include "experiment.h"

using namespace std;

int main()
{
    ifstream exp("exp.ini", ios::in);
    vector<string> probList;
    BaseEA* ea = nullptr;
    SetExperiment(ea, probList, exp);
    exp.close();

    for (size_t i = 0; i < probList.size(); ++i)
    {
        BProblem* prob = nullptr;

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