
#include <numbers>
#include <cmath>

#include "problem_MW11.h"
#include "individual.h"

using namespace std;

bool MW11::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(4);
    ind.violations()[0] = C1(ind);
    ind.violations()[1] = C2(ind);
    ind.violations()[2] = C3(ind);
    ind.violations()[3] = C4(ind);

    Check(ind, 0.0001);

    return true;
}

double MW11::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind) * sqrt(1.9999);
}

double MW11::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * sqrt(2.0 - pow(f1 / g, 2));
}

double MW11::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (3.0 - pow(f1, 2) - f2) * (3.0 - 2.0 * pow(f1, 2) - f2);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW11::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (3.0 - 0.625 * pow(f1, 2) - f2) * (3.0 - 7.0 * pow(f1, 2) - f2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW11::C3(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (1.62 - 0.18 * pow(f1, 2) - f2) * (1.125 - 0.125 * pow(f1, 2) - f2);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW11::C4(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (2.07 - 0.23 * pow(f1, 2) - f2) * (0.63 - 0.07 * pow(f1, 2) - f2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW11::G(const Individual& ind) const
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