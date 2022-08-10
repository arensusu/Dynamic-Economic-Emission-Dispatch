
#include <numbers>
#include <cmath>

#include "problem_MW12.h"
#include "individual.h"

using namespace std;

bool MW12::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(2);
    ind.violations()[0] = C1(ind);
    ind.violations()[1] = C2(ind);

    Check(ind, 0.0001);

    return true;
}

double MW12::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind);
}

double MW12::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * (0.85 - 0.8 * f1 / g - 0.08 * abs(sin(3.2 * numbers::pi * f1 / g)));
}

double MW12::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double T1 = 1.0 - 0.8 * f1 - f2 + 0.08 * sin(2.0 * numbers::pi * (f2 - f1 / 1.5));
    double T4 = 1.8 - 1.125 * f1 - f2 + 0.08 * sin(2.0 * numbers::pi * (f2 / 1.8 - f1 / 1.6));

    double violation = T1 * T4;

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW12::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double T2 = 1.0 - 0.625 * f1 - f2 + 0.08 * sin(2.0 * numbers::pi * (f2 - f1 / 1.6));
    double T3 = 1.4 - 0.875 * f1 - f2 + 0.08 * sin(2.0 * numbers::pi * (f2 / 1.4 - f1 / 1.6));

    double violation = T2 * T3;

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW12::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = pow(ind.encoding()[i], numVariables_ - numObjectives_);
        total += 1.0 - exp(-10.0 * pow(z - 0.5 - double(i) / (2.0 * numVariables_), 2));
    }

    return 1 + total;
}