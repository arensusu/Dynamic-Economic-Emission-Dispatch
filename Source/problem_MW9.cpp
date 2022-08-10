
#include <numbers>
#include <vector>
#include <cmath>

#include "problem_MW9.h"
#include "individual.h"

using namespace std;

bool MW9::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(1);
    ind.violations()[0] = C(ind);

    Check(ind, 0.0001);

    return true;
}

double MW9::F1(const Individual& ind) const
{
    return ind.encoding()[0] * G(ind);
}

double MW9::F2(const Individual& ind) const
{
    double g = G(ind);
    double f1 = ind.objs()[0];

    return g * (1.0 - pow(f1 / g, 0.6));
}

double MW9::C(const Individual& ind) const
{
    double f1 = ind.objs()[0];
    double f2 = ind.objs()[1];

    double T1 = (1.0 - 0.64 * pow(f1, 2) - f2) * (1.0 - 0.36 * pow(f1, 2) - f2);
    double T2 = pow(1.35, 2) - pow(f1 + 0.35, 2) - f2;
    double T3 = pow(1.15, 2) - pow(f1 + 0.15, 2) - f2;

    double violation = min(T1, T2 * T3);

    if (violation <= 0)
    {
        return 0;
    }
    else
    {
        return abs(violation);
    }
}

double MW9::G(const Individual& ind) const
{
    double total = 0.0;

    for (size_t i = numObjectives_ - 1; i < numVariables_; ++i)
    {
        double z = pow(ind.encoding()[i], numVariables_ - numObjectives_);
        total += 1.0 - exp(-10.0 * pow(z - 0.5 - double(i) / (2.0 * numVariables_), 2));
    }

    return 1 + total;
}