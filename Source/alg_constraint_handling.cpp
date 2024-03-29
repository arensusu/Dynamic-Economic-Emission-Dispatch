
#include <limits>
#include <algorithm>
#include <random>

#include "alg_constraint_handling.h"
#include "individual.h"
#include "problem_base.h"

#include "tool.h"

using namespace std;

void ZeroOneCH::operator()(Individual& ind) const
{
    size_t numVariables = Individual::prob().numVariables();

    for (size_t i = 0; i < numVariables; ++i)
    {
        if (ind[i] < 0.0)
        {
            ind[i] = 0.0;
        }

        if (ind[i] > 1.0)
        {
            ind[i] = 1.0;
        }
    }
}

void InequalityCH::operator()(Individual& ind) const
{
    size_t numPeriods = Individual::prob().numPeriods();

    vector<double> prev;
    for (size_t t = 0; t < numPeriods; ++t)
    {
        const double demand = Individual::prob().load(t);

        vector<double> power_t = ind.Decoder(t);

        (*this)(power_t, prev, Individual::prob());

        ind.Encoder(t, power_t);

        prev = power_t;
    }
}

void InequalityCH::operator()(vector<double>& curr, const vector<double>& prev, const BProblem &prob) const
{
    for (size_t i = 0; i < curr.size(); ++i)
    {
        const double Pmin = prob.limit(i, 0);
        const double Pmax = prob.limit(i, 1);

        if (prev.size() == 0)
        {
            if (curr[i] < Pmin)
            {
                curr[i] = Pmin;
            }

            if (curr[i] > Pmax)
            {
                curr[i] = Pmax;
            }
        }
        else
        {
            const double rampUp = prob.ramp(i, 0);
            const double rampDown = prob.ramp(i, 1);

            double lower = max(Pmin, prev[i] - rampDown);
            if (curr[i] < lower)
            {
                curr[i] = lower;
            }

            double upper = min(Pmax, prev[i] + rampUp);
            if (curr[i] > upper)
            {
                curr[i] = upper;
            }
        }
    }
}

vector<double> PowerRemain(const vector<double>& curr, const vector<double>& prev, const int sign, const BProblem& prob)
{
    // Parameters.
    size_t numMachines = prob.numMachines();

    vector<double> remains(numMachines);
    
    for (size_t i = 0; i < numMachines; ++i)
    {
        const double Pmin = prob.limit(i, 0);
        const double Pmax = prob.limit(i, 1);

        double bound;
        if (prev.size() == 0)
        {
            if (sign == 1)
            {
                bound = Pmax;
            }
            else
            {
                bound = Pmin;
            }
        }
        else
        {
            const double rampUp = prob.limit(i, 0);
            const double rampDown = prob.limit(i, 1);

            if (sign == 1)
            {
                bound = min(Pmax, prev[i] + rampUp);
            }
            else
            {
                bound = max(Pmin, prev[i] - rampDown);
            }
        }

        remains[i] = abs(curr[i] - bound);
    }

    return remains;
}

size_t RouletteWheel(const vector<double>& remains)
{    
    double sum = 0.0;
    for (size_t i = 0; i < remains.size(); ++i)
    {
        sum += remains[i];
    }

    // Cannot adjust.
    if (sum == 0.0)
    {
        return remains.size();
    }

    // Random engine.
    uniform_real_distribution<double> dis(0.0, 1.0);

    double r1 = dis(gen);

    double range = 0;

    for (size_t i = 0; i < remains.size(); ++i)
    {
        range += remains[i] / sum;

        if (r1 < range)
        {
            return i;
        }
    }
}

int DivisionCH::operator()(Individual& ind, const size_t maxTry, const double threshold) const
{
    // Parameters.
    size_t numPeriods = Individual::prob().numPeriods();
    size_t numMachines = Individual::prob().numMachines();

    InequalityCH inequal;

    double supply;

    int repairTime = 0;

    vector<double> prev;

    for (size_t t = 0; t < numPeriods; ++t)
    {
        const double demand = Individual::prob().load(t);

        vector<double> power_t = ind.Decoder(t);

        inequal(power_t, prev, Individual::prob());
        supply = Individual::PowerOutput(power_t);

        double diff = demand - supply;

        size_t i = 0;
        while (abs(diff) >= threshold && i < maxTry)
        {
            // Adjust all machines.
            double div = diff / numMachines;
            for (size_t j = 0; j < numMachines; ++j)
            {
                power_t[j] += div;
            }

            inequal(power_t, prev, Individual::prob());
            supply = Individual::PowerOutput(power_t);

            diff = demand - supply;

            i++;
        }

        if (i == maxTry)
        {
            repairTime += 1;
        }

        ind.Encoder(t, power_t);

        prev = power_t;
    }

    return repairTime;
}

int FineTuningCH::operator()(Individual& ind, const size_t maxTry, const double threshold) const
{
    // Parameters.
    size_t numPeriods = Individual::prob().numPeriods();
    size_t numMachines = Individual::prob().numMachines();

    InequalityCH inequal;

    uniform_int_distribution<size_t> dis(0, numMachines - 1);

    double supply;

    int repairTime = 0;

    vector<double> prev;

    for (int t = 0; t < numPeriods; ++t)
    {
        const double demand = Individual::prob().load(t);

        vector<double> power_t = ind.Decoder(t);

        inequal(power_t, prev, Individual::prob());
        supply = Individual::PowerOutput(power_t);

        double diff = demand - supply;

        int i = 0;
        while (abs(diff) >= threshold && i < maxTry)
        {
            size_t j = dis(gen);

            power_t[j] = power_t[j] + diff;

            inequal(power_t, prev, Individual::prob());
            supply = Individual::PowerOutput(power_t);

            diff = demand - supply;

            i++;
        }

        if (i == maxTry)
        {
            repairTime += 1;
        }

        ind.Encoder(t, power_t);

        prev = power_t;
    }

    return repairTime;
}

const vector<double> Proportion(const BProblem& prob)
{
    // Parameters.
    const size_t numMachines = prob.numMachines();

    double sum = 0.0;

    vector<double> prop(numMachines);
    for (size_t i = 0; i < numMachines; ++i)
    {
        const double Pmin = prob.limit(i, 0);
        const double Pmax = prob.limit(i, 1);

        prop[i] = Pmax - Pmin;

        sum += prop[i];
    }

    for (size_t i = 0; i < numMachines; ++i)
    {
        prop[i] = prop[i] / sum;
    }

    return prop;
}

int ProportionDivisionCH::operator()(Individual& ind, const size_t maxTry, const double threshold) const
{
    // Parameters.
    size_t numPeriods = Individual::prob().numPeriods();
    size_t numMachines = Individual::prob().numMachines();

    InequalityCH inequal;

    const vector<double> proportion = Proportion(Individual::prob());

    double supply;

    int repairTime = 0;

    vector<double> prev;

    for (size_t t = 0; t < numPeriods; ++t)
    {
        const double demand = Individual::prob().load(t);

        vector<double> power_t = ind.Decoder(t);

        inequal(power_t, prev, Individual::prob());

        supply = Individual::PowerOutput(power_t);
        double diff = demand - supply;

        size_t i = 0;
        while (abs(diff) >= threshold && i < maxTry)
        {
            // Adjust all machines.
            for (size_t j = 0; j < numMachines; ++j)
            {
                power_t[j] += diff * proportion[j];
            }

            inequal(power_t, prev, Individual::prob());
            
            supply = Individual::PowerOutput(power_t);
            diff = demand - supply;

            i++;
        }

        if (i == maxTry)
        {
            repairTime += 1;
        }

        ind.Encoder(t, power_t);

        prev = power_t;
    }

    return repairTime;
}
