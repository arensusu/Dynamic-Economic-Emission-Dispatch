


#include "experiment.h"
#include "algorithm_base.h"
#include "algorithm_DE.h"
#include "algorithm_jDE.h"

#include "problem_base.h"
#include "problem_noloss.h"
#include "problem_without_B0.h"
#include "problem_with_B0.h"

using namespace std;

void SetExperiment(BaseEA** ea, vector<string>& probList, ifstream& file)
{
    string algoName, dummy;
    
    file >> dummy >> dummy >> algoName;

    if (algoName == "DE")
    {
        *ea = new DE();
    }
    else if (algoName == "jDE")
    {
        *ea = new JDE();
    }

    (*ea)->Setup(file);

    string prob;    
    while (file >> dummy >> dummy >> prob)
    {
        probList.push_back(prob);
    }

    return;
}

void SetProblemType(BProblem** prob, const string& fname)
{
    if (fname.find("WB") != string::npos)
    {
        *prob = new WBProblem();
        (*prob)->Read(fname.substr(0, (fname.size() - 3)));
    }
    else if (fname.find("WOB") != string::npos)
    {
        *prob = new WOBProblem();
        (*prob)->Read(fname.substr(0, (fname.size() - 4)));
    }
    else if (fname.find("N") != string::npos)
    {
        *prob = new NProblem();
        (*prob)->Read(fname.substr(0, (fname.size() - 2)));
    }

    return;
}