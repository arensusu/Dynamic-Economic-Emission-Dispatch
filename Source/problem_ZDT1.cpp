
#include <numbers>
#include <cmath>

#include "problem_ZDT1.h"
#include "individual.h"

using namespace std;

bool ZDT1::Evaluate(Individual& ind) const
{
    ind.objs()[0] = F1(ind);
    ind.objs()[1] = F2(ind);

    ind.violations().resize(1);
    ind.violations()[0] = 0;

    Check(ind, 0.0001);

    return true;
}

double ZDT1::F1(const Individual& ind) const
{
    return ind.encoding()[0];
}

double ZDT1::F2(const Individual& ind) const
{
    double g = 1.0;

    for (size_t i = 1; i < numVariables_; ++i)
    {
        g += 9.0 / (numVariables_ - 1) * ind.encoding()[i];
    }

    double f1 = ind.objs()[0];

    return g * (1 - pow(f1 / g, 0.5));
}
