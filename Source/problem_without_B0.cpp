
#include "problem_without_B0.h"

using namespace std;

bool WOBProblem::SetB(ifstream& file)
{
    B2_.resize(numMachines_, vector<double>(numMachines_, 0));
    string dummy;

    file >> dummy;
    int i = -1, j = -1;
    for (i = 0; i < numMachines_; ++i)
    {
        for (j = 0; j < numMachines_; ++j)
        {
            file >> B2_[i][j];
        }
    }

    if (i < numMachines_ || j < numMachines_)
    {
        return false;
    }
    return true;
}

bool WOBProblem::Read(const string& name)
{
    //modify path
    ifstream fileC(name, ios::in);

    if (!SetCoeff(fileC))
    {
        return false;
    }

    if (!SetLoad(fileC))
    {
        return false;
    }

    //modify path
    ifstream fileB(name, ios::in);

    if (!SetB(fileB))
    {
        return false;
    }
    return true;
}