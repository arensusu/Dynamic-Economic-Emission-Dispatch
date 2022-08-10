
#include <numbers>
#include <cmath>

#include "problem_MW3.h"
#include "individual.h"

using namespace std;

bool MW3::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(2);
    ind.violations()[0] = C1(ind);
    ind.violations()[1] = C2(ind);

    Check(ind, 0.0001);

    return true;
}

double MW3::F1(const Individual& ind) const
{
    return ind.encoding()[0];
}

double MW3::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * (1 - f1 / g);
}

double MW3::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = sqrt(2.0) * f2 - sqrt(2.0) * f1;
    double violation = 1.05 - f1 - f2 + 0.45 * pow(sin(0.75 * numbers::pi * l), 6);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW3::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = sqrt(2.0) * f2 - sqrt(2.0) * f1;
    double violation = 0.85 - f1 - f2 + 0.3 * pow(sin(0.75 * numbers::pi * l), 2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW3::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double curr = ind.encoding()[i];
        double prev = ind.encoding()[i - 1];
        total += 2.0 * pow(curr + pow(prev - 0.5, 2) - 1, 2);
    }

    return 1 + total;
}