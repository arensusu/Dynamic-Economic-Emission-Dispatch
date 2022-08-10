
#include <numbers>
#include <cmath>

#include "problem_MW6.h"
#include "individual.h"

using namespace std;

bool MW6::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(1);
    ind.violations()[0] = C(ind);

    Check(ind, 0.0001);

    return true;
}

double MW6::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind) * 1.099;
}

double MW6::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * sqrt(pow(1.1, 2) - pow(f1 / g, 2));
}

double MW6::C(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = pow(cos(6.0 * pow(atan(f2 / f1), 4)), 10);
    double violation = 1 - pow(f1 / (1.0 + 0.15 * l), 2) - pow(f2 / (1.0 + 0.75 * l), 2);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW6::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = 1 - exp(-10 * pow(ind.encoding()[i] - double(i) / numVariables_, 2));
        total += 1.5 + (0.1 / numVariables_ * pow(z, 2)) - 1.5 * cos(2 * numbers::pi * z);
    }

    return 1 + total;
}