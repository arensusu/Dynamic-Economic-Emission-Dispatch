


#include "experiment.h"
#include "algorithm_base.h"
#include "algorithm_DE.h"

using namespace std;

void SetExperiment(BaseEA* ea, vector<string>& probList, ifstream& file)
{
    string algoName, dummy;
    
    file >> dummy >> dummy >> algoName;

    if (algoName == "DE")
    {
        ea = new DE();
    }

    ea->Setup(file);

    string prob;

    file >> dummy >> dummy;
    
    while (file >> prob)
    {
        probList.push_back(prob);
    }

    return;
}