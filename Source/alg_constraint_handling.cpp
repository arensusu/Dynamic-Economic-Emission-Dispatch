
#include <limits>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "alg_constraint_handling.h"
#include "individual.h"
#include "problem_base.h"

using namespace std;

void InequalityConstraint(vector<double>& curr, 
                                         const vector<double>& prev, 
                                         const BProblem &prob)
{
    if (prev.size() == 0)
    {
        for (size_t i = 0; i < curr.size(); ++i)
        {
            if (curr[i] < prob.limit(i, 0))
            {
                curr[i] = prob.limit(i, 0);
            }

            if (curr[i] > prob.limit(i, 1))
            {
                curr[i] = prob.limit(i, 1);
            }
        }
    }
    else
    {
        for (size_t i = 0; i < curr.size(); ++i)
        {
            double lower = max(prob.limit(i, 0), prev[i] - prob.ramp(i, 1));
            if (curr[i] < lower)
            {
                curr[i] = lower;
            }

            double upper = min(prob.limit(i, 1), prev[i] + prob.ramp(i, 0));
            if (curr[i] > upper)
            {
                curr[i] = upper;
            }
        }
    }

    return;
}

double PowerOutput(const vector<double>& powers, const BProblem &prob)
{
    double provide = 0;
    for (size_t i = 0; i < powers.size(); ++i)
    {
        provide += powers[i];
    }

    if (prob.B2(0, 0) != numeric_limits<double>::min())
    {
        double loss = 0;
        for (size_t i = 0; i < powers.size(); ++i)
        {
            for (size_t j = 0; j < powers.size(); ++j)
            {
                loss += powers[i] * prob.B2(i, j) * powers[j];
            }
        }

        if (prob.B0() != numeric_limits<double>::min())
        {
            for (size_t i = 0; i < powers.size(); ++i)
            {
                loss += powers[i] * prob.B1(i);
            }
        }
        
        provide -= loss;
    }

    return provide;
}

void DivisionCH::operator()(Individual& ind) const
{
    const BProblem &prob = Individual::prob();
    double supply;

    vector<double> prev;
    for (int t = 0; t < prob.numPeriods(); ++t)
    {
        vector<double> power_t(ind.encoding().begin() + t * prob.numMachines(), ind.encoding().begin() + (t + 1) * prob.numMachines());

        for (size_t i = 0; i < power_t.size(); ++i)
        {
            power_t[i] = prob.limit(i, 0) + power_t[i] * (prob.limit(i, 1) - prob.limit(i, 0));
        }

        InequalityConstraint(power_t, prev, prob);
        supply = PowerOutput(power_t, prob);

        int i = 0;
        int MAXTRY = 100;
        while (i < MAXTRY && abs(supply - prob.load(t)) > 0.00001)
        {
            double div = (prob.load(t) - supply) / prob.numMachines();
            for (int j = 0; j < prob.numMachines(); ++j)
            {
                power_t[j] += div;
            }

            InequalityConstraint(power_t, prev, prob);
            supply = PowerOutput(power_t, prob);

            i++;
        }
        
        for (int j = 0; j < prob.numMachines(); ++j)
        {
            ind.encoding()[t * prob.numMachines() + j] = (power_t[j] - prob.limit(j, 0)) / (prob.limit(j, 1) - prob.limit(j, 0));
        }

        prev = power_t;
    }

    return;
}

void FineTuningCH::operator()(Individual& ind) const
{
    srand(time(NULL));

    const BProblem& prob = Individual::prob();
    double supply;

    vector<double> prev;
    for (int t = 0; t < prob.numPeriods(); ++t)
    {
        vector<double> power_t(ind.encoding().begin() + t * prob.numMachines(), ind.encoding().begin() + (t + 1) * prob.numMachines());

        for (size_t i = 0; i < power_t.size(); ++i)
        {
            power_t[i] = prob.limit(i, 0) + power_t[i] * (prob.limit(i, 1) - prob.limit(i, 0));
        }

        InequalityConstraint(power_t, prev, prob);
        supply = PowerOutput(power_t, prob);

        int i = 0;
        int MAXTRY = 100;
        while (i < MAXTRY && abs(supply - prob.load(t)) > 0.00001)
        {
            size_t j = rand() % prob.numMachines();
            
            power_t[j] += prob.load(t) - supply;

            InequalityConstraint(power_t, prev, prob);
            supply = PowerOutput(power_t, prob);

            i++;
        }

        for (int j = 0; j < prob.numMachines(); ++j)
        {
            ind.encoding()[t * prob.numMachines() + j] = (power_t[j] - prob.limit(j, 0)) / (prob.limit(j, 1) - prob.limit(j, 0));
        }

        prev = power_t;
    }

    return;
}