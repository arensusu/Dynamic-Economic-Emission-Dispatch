
#include "problem_test.h"
#include "individual.h"

using namespace std;

void TProblem::Check(Individual& ind, const double threshold) const
{
    double total = 0.0;

    for (size_t i = 0; i < ind.violations().size(); ++i)
    {
        total += ind.violations()[i];
    }

    ind.violation() = total;

    if (total > 0)
    {
        ind.feasible() = false;
    }
    else
    {
        ind.feasible() = true;
    }
}
