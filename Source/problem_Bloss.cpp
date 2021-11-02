
#include "problem_Bloss.h"

using namespace std;

bool BLProblem::SetB(ifstream& file, const int numMachine)
{
    B2_.resize(numMachine, vector<double>(numMachine, 0));
    string dummy;

    file >> dummy;
    int i = -1, j = -1;
    for (i = 0; i < numMachine; ++i)
    {
        for (j = 0; j < numMachine; ++j)
        {
            file >> B2_[i][j];
        }
    }

    if (i < numMachine || j < numMachine)
    {
        return false;
    }

    if (file.eof())
    {
        return true;
    }

    B1_.resize(numMachine, 0);

    file >> dummy;
    for (int i = 0; i < numMachine; ++i)
    {
        file >> B1_[i];
    }

    if (i < numMachine)
    {
        return false;
    }

    file >> dummy;
    file >> B0_;

    if (file.eof())
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool BLProblem::Read(const string& name)
{
    //modify path
    ifstream fileC(name, ios::in);

    if (!SetCoeff(fileC, numMachines_))
    {
        return false;
    }

    if (!SetLoad(fileC, numPeriods_))
    {
        return false;
    }

    //modify path
    ifstream fileB(name, ios::in);

    if (!SetB(fileB, numMachines_))
    {
        return false;
    }
    return true;
}