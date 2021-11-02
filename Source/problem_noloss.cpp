
#include <cmath>

#include "problem_noloss.h"
#include "individual.h"

using namespace std;

bool NProblem::SetCoeff(ifstream& file, const int numMachine)
{
    int numCoeff = 14;
    coeff_.resize(numMachine, vector<double>(numCoeff, 0));
    string dummy;

    file >> dummy;
    int i = -1, j = -1;
    for (i = 0; i < numMachine; ++i)
    {
        for (j = 0; j < numCoeff; ++j)
        {
            file >> coeff_[i][j];
        }
    }

    if (i < numMachine || j < numCoeff)
    {
        return false;
    }
    return true;
}

bool NProblem::SetLoad(ifstream& file, const int numPeriod)
{
    load_.resize(numPeriod, 0);
    string dummy;

    file >> dummy;
    int i;
    for (i = 0; i < numPeriod; ++i)
    {
        file >> load_[i];
    }

    if (i < numPeriod)
    {
        return false;
    }
    return true;
}

bool NProblem::Read(const string& filename)
{
    //modify path
    ifstream file(filename, ios::in);
    
    if (!SetCoeff(file, numMachines_))
    {
        return false;
    }

    if(!SetLoad(file, numPeriods_))
    {
        return false;
    }
    return true;
}

bool NProblem::Evaluate(Individual& ind) const
{
    const vector<double> encoding = ind.encoding();
    double cost = 0, emission = 0;

    int i = -1, j = -1;
    for (i = 0; i < numPeriods_; ++i)
    {
        for (j = 0; j < numMachines_; ++j)
        {
            double power = encoding[i * numPeriods_ + numMachines_];
            cost += coeff(j, 2) + coeff(j, 3) * power + coeff(j, 4) * pow(power, 2.0) + abs(coeff(j, 5) * sin(coeff(j, 6) * (coeff(j, 1) - power)));
            emission += coeff(j, 7) + coeff(j, 8) * power + coeff(j, 9) * pow(power, 2.0) + coeff(j, 10) * exp(coeff(j, 11) * power);
        }
    }

    if (i < numPeriods_ || j < numMachines_)
    {
        return false;
    }
    ind.objective(0) = cost;
    ind.objective(1) = emission;

    return true;
}