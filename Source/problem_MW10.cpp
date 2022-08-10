
#include <numbers>
#include <cmath>

#include "problem_MW10.h"
#include "individual.h"

using namespace std;

bool MW10::Evaluate(Individual& ind) const
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

double MW10::F1(const Individual& ind) const
{
    return G(ind) * pow(ind.encoding()[0], numVariables_);
}

double MW10::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * (1.0 - pow(f1 / g, 2));
}

double MW10::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (2.0 - 4.0 * pow(f1, 2) - f2) * (2.0 - 8.0 * pow(f1, 2) - f2);

    if (violation >= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW10::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (2.0 - 2.0 * pow(f1, 2) - f2) * (2.0 - 16.0 * pow(f1, 2) - f2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW10::C3(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double violation = (1.0 - pow(f1, 2) - f2) * (1.2 - 1.2 * pow(f1, 2) - f2);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW10::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = 1 - exp(-10 * pow(ind.encoding()[i] - double(i) / numVariables_, 2));
        total += 1.5 + (0.1 / numVariables_ * pow(z, 2)) - 1.5 * cos(2 * numbers::pi * z);
    }

    return 1 + total;
}