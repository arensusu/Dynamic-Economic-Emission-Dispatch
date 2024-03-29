
#include <cmath>

#include "problem_noloss.h"
#include "individual.h"

using namespace std;

bool NProblem::SetCoeff(ifstream& file)
{
    file >> numMachines_;

    size_t numCoeffs = 10;
    limits_.resize(numMachines_, vector<double>(2, 0));
    coeffs_.resize(numMachines_, vector<double>(numCoeffs, 0));
    ramps_.resize(numMachines_, vector<double>(2, 0));

    size_t i = -1, j = -1;
    for (i = 0; i < numMachines_; ++i)
    {
        // Pmin, Pmax.
        file >> limits_[i][0] >> limits_[i][1];

        // Upper, Lower.
        file >> ramps_[i][0] >> ramps_[i][1];

        // Coefficients of cost and emission.
        for (j = 0; j < numCoeffs; ++j)
        {
            file >> coeffs_[i][j];
        }
    }

    // Legally check.
    if (i < numMachines_ || j < numCoeffs)
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
        // Loads.
        file >> loads_[i];
    }

    // Legally check.
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

bool NProblem::Evaluate(vector<double>& objs, const vector<double>& powers) const
{
    double cost = 0, emission = 0;

    size_t i = -1, j = -1;
    for (i = 0; i < numPeriods_; ++i)
    {
        double tmpCost = 0.0;
        double tmpEmission = 0.0;
        for (j = 0; j < numMachines_; ++j)
        {
            tmpCost += coeff(j, 0)
                  + coeff(j, 1) * powers[i * numMachines_ + j]
                  + coeff(j, 2) * pow(powers[i * numMachines_ + j], 2.0)
                  + abs(coeff(j, 3) * sin(coeff(j, 4) * (limit(j, 0) - powers[i * numMachines_ + j])));
            
            tmpEmission += coeff(j, 5)
                      + coeff(j, 6) * powers[i * numMachines_ + j]
                      + coeff(j, 7) * pow(powers[i * numMachines_ + j], 2.0)
                      + coeff(j, 8) * exp(coeff(j, 9) * powers[i * numMachines_ + j]);
        }
        cost += tmpCost;
        emission += tmpEmission;
    }

    objs[0] = cost;
    objs[1] = emission;

    return true;
}

bool NProblem::Evaluate(Individual& ind) const
{
    Check(ind, 1.0);
    return Evaluate(ind.objs(), ind.Decoder());
}

void NProblem::Check(Individual& ind, const double threshold) const
{
    // Parameters.
    const vector<double> vars = ind.Decoder();

    ind.violations().clear();
    // Demand constraint.
    for (size_t t = 0; t < numPeriods_; ++t)
    {
        const double supply = ind.PowerOutput(t);
        const double demand = this->load(t);

        double diff = max(0.0, abs(supply - demand) - threshold);

        ind.violations().push_back(diff);
    }

    // Load and ramp-rate constraint.
    for (size_t i = 0; i < numMachines_; ++i)
    {
        double output = vars[i];
        double diff = max(0.0, output - this->limit(i, 1)) + max(0.0, this->limit(i, 0) - output);
        ind.violations().push_back(diff);

        double prev = vars[i];

        for (size_t t = 1; t < numPeriods_; ++t)
        {
            double curr = vars[t * numMachines_ + i];

            diff = max(0.0, curr - min(prev + this->ramp(i, 0), this->limit(i, 1))) + max(0.0, max(prev - this->ramp(i, 1), this->limit(i, 0)) - curr);

            ind.violations().push_back(max(0.0, diff - threshold));

            prev = curr;
        }
    }

    double totalViolation = 0.0;
    for (size_t i = 0; i < ind.violations().size(); ++i)
    {
        totalViolation += ind.violations()[i];
    }
    ind.violation() = totalViolation;

    if (totalViolation <= 0)
    {
        ind.feasible() = true;
    }
    else
    {
        ind.feasible() = false;
    }
}
