
#include "problem_noloss.h"

using namespace std;

void NProblem::setCoeff(ifstream& file, const int numMachine, const int numCoeff)
{
    for (int i = 0; i < numMachine; ++i)
    {
        for (int j = 0; j < numCoeff; ++j)
        {
            file >> coeff_[i][j];
        }
    }

    return;
}

void NProblem::setLoad(ifstream& file, const int numPeriod)
{
    for (int i = 0; i < numPeriod; ++i)
    {
        file >> load_[i];
    }

    return;
}