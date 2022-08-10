
#include <numbers>
#include <cmath>

#include "problem_MW5.h"
#include "individual.h"

using namespace std;

bool MW5::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(3);
    ind.violations()[0] = C1(ind);
    ind.violations()[1] = C2(ind);
    ind.violations()[2] = C3(ind);

    Check(ind, 0.0001);

    return true;
}

double MW5::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind);
}

double MW5::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * sqrt(1 - pow(f1 / g, 2));
}

double MW5::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = atan(f2 / f1);
    double violation = pow(1.7 - 0.2 * sin(2.0 * l), 2) - pow(f1, 2) - pow(f2, 2);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW5::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = 0.5 * numbers::pi - 2.0 * abs(atan(f2 / f1) - 0.25 * numbers::pi);
    double violation = pow(1.0 + 0.5 * sin(6 * pow(l, 3)), 2) - pow(f1, 2) - pow(f2, 2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW5::C3(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double l = 0.5 * numbers::pi - 2.0 * abs(atan(f2 / f1) - 0.25 * numbers::pi);
    double violation = pow(1.0 - 0.45 * sin(6 * pow(l, 3)), 2) - pow(f1, 2) - pow(f2, 2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW5::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = pow(ind.encoding()[i], numVariables_ - numObjectives_);
        total += 1.0 - exp(-10.0 * pow(z - 0.5 - double(i) / (2.0 * numVariables_), 2));
    }

    return 1 + total;
}