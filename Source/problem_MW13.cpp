
#include <numbers>
#include <cmath>

#include "problem_MW13.h"
#include "individual.h"

using namespace std;

bool MW13::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(2);
    ind.violations()[0] = C1(ind);
    ind.violations()[1] = C2(ind);

    Check(ind, 0.0001);

    return true;
}

double MW13::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind) * 1.5;
}

double MW13::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * (5.0 - exp(f1 / g) - 0.5 * abs(sin(3.0 * numbers::pi *f1 / g)));
}

double MW13::C1(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double T1 = 5.0 - exp(f1) - 0.5 * sin(3.0 * numbers::pi * f1) - f2;
    double T4 = 5.0 - (1.0 + 0.4 * f1) - 0.5 * sin(3.0 * numbers::pi * f1) - f2;

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

double MW13::C2(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];
    double T2 = 5.0 - (1.0 + f1 + 0.5 * pow(f1, 2)) - 0.5 * sin(3.0 * numbers::pi * f1) - f2;
    double T3 = 5.0 - (1.0 + 0.7 * f1) - 0.5 * sin(3.0 * numbers::pi * f1) - f2;

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

double MW13::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = 1 - exp(-10 * pow(ind.encoding()[i] - double(i) / numVariables_, 2));
        total += 1.5 + (0.1 / numVariables_ * pow(z, 2)) - 1.5 * cos(2 * numbers::pi * z);
    }

    return 1 + total;
}