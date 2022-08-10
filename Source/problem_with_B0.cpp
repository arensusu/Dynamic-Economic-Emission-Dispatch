
#include "problem_with_B0.h"

using namespace std;

bool WBProblem::SetB(ifstream& file)
{
    B2_.resize(numMachines_, vector<double>(numMachines_, 0));
    
    string dummy;
    file >> dummy;

    size_t i = -1, j = -1;
    for (i = 0; i < numMachines_; ++i)
    {
        for (j = 0; j < numMachines_; ++j)
        {
            // Quadratic term.
            file >> B2_[i][j];
        }
    }

    // Legally check.
    if (i < numMachines_ || j < numMachines_)
    {
        return false;
    }

    B1_.resize(numMachines_, 0);

    for (size_t i = 0; i < numMachines_; ++i)
    {
        // Linear term.
        file >> B1_[i];
    }

    // Legally check.
    if (i < numMachines_)
    {
        return false;
    }

    // Constant term.
    file >> B0_;

    return true;
}

bool WBProblem::Read(const string& fname)
{
    ifstream fileC("./Dataset/Generator/" + fname + ".txt", ios::in);

    // Information of generators.
    if (!SetCoeff(fileC))
    {
        return false;
    }

    if (!SetLoad(fileC))
    {
        return false;
    }

    fileC >> numObjectives_;

    ifstream fileB("./Dataset/B_Coeff/" + fname + ".txt", ios::in);

    // Coefficients of loss.
    if (!SetB(fileB))
    {
        return false;
    }

    return true;
}