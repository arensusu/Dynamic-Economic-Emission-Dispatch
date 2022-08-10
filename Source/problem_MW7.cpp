
#include <numbers>
#include <cmath>

#include "problem_MW7.h"
#include "individual.h"

using namespace std;

bool MW7::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(2);
    ind.violations()[0] = C1(ind);
    ind.violations()[1] = C2(ind);

    Check(ind, 0.0001);

    return true;
}

double MW7::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind);
}

double MW7::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * sqrt(1.0 - pow(f1 / g, 2));
}

double MW7::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = atan(f2 / f1);
    double violation = pow(1.2 + 0.4 * pow(sin(4.0 * l), 16), 2) - pow(f1, 2) - pow(f2, 2);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW7::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = atan(f2 / f1);
    double violation = pow(1.15 - 0.2 * pow(sin(4.0 * l), 8), 2) - pow(f1, 2) - pow(f2, 2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW7::G(const Individual& ind) const
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