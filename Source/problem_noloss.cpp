
#include <cmath>

#include "problem_noloss.h"
#include "individual.h"

using namespace std;

bool NProblem::SetCoeff(ifstream& file)
{
    file >> numMachines_;

    size_t numCoeff = 10;
    limits_.resize(numMachines_, vector<double>(2, 0));
    coeffs_.resize(numMachines_, vector<double>(numCoeff, 0));
    ramps_.resize(numMachines_, vector<double>(2, 0));

    size_t i = -1, j = -1;
    for (i = 0; i < numMachines_; ++i)
    {
        file >> limits_[i][0] >> limits_[i][1];

        for (j = 0; j < numCoeff; ++j)
        {
            file >> coeffs_[i][j];
        }

        file >> ramps_[i][0] >> ramps_[i][1];
    }

    if (i < numMachines_ || j < numCoeff)
    {
        return false;
    }

    return true;
}

bool NProblem::SetLoad(ifstream& file)
{
    file >> numPeriods_;

    loads_.resize(numPeriods_, 0);

    size_t i;
    for (i = 0; i < numPeriods_; ++i)
    {
        file >> loads_[i];
    }

    if (i < numPeriods_)
    {
        return false;
    }
    return true;
}

bool NProblem::Read(const string& fname)
{
    //modify path
    ifstream file("./Dataset/Generator/" + fname + ".txt", ios::in);
    
    if (!SetCoeff(file))
    {
        return false;
    }

    if(!SetLoad(file))
    {
        return false;
    }

    file >> numObjectives_;

    return true;
}

bool NProblem::Evaluate(Individual& ind) const
{
    const vector<double> encoding = ind.encoding();
    double cost = 0, emission = 0;

    size_t i = -1, j = -1;
    for (i = 0; i < numPeriods_; ++i)
    {
        for (j = 0; j < numMachines_; ++j)
        {
            double power = limits_[j][0] + encoding[i * numMachines_ + j] * (limits_[j][1] - limits_[j][0]);
            cost += coeff(j, 0)
                  + coeff(j, 1) * power
                  + coeff(j, 2) * pow(power, 2.0)
                  + abs(coeff(j, 3) * sin(coeff(j, 4) * (limit(j, 0) - power)));
            emission += coeff(j, 5)
                      + coeff(j, 6) * power
                      + coeff(j, 7) * pow(power, 2.0)
                      + coeff(j, 8) * exp(coeff(j, 9) * power);
        }
    }

    if (i < numPeriods_ || j < numMachines_)
    {
        return false;
    }
    ind.objs().push_back(cost);
    ind.objs().push_back(emission);

    return true;
}