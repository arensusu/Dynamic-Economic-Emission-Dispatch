
#include <numbers>
#include <cmath>

#include "problem_MW2.h"
#include "individual.h"

using namespace std;

bool MW2::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(1);
    ind.violations()[0] = C(ind);

    Check(ind, 0.0001);

    return true;
}

double MW2::F1(const Individual& ind) const
{
    return ind.encoding()[0];
}

double MW2::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * (1 - f1 / g);
}

double MW2::C(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = sqrt(2.0) * f2 - sqrt(2.0) * f1;
    double violation = 1 - f1 - f2 + 0.5 * pow(sin(3 * numbers::pi * l), 8);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW2::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = 1 - exp(-10 * pow(ind.encoding()[i] - double(i) / numVariables_, 2));
        total += 1.5 + (0.1 / numVariables_ * pow(z, 2)) - 1.5 * cos(2 * numbers::pi * z);
    }

    return 1 + total;
}