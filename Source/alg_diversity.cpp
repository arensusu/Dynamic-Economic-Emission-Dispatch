
#include <vector>
#include <chrono>
#include <random>

#include "alg_diversity.h"
#include "individual.h"

#include "tool.h"

using namespace std;

bool PolynomialMutation::operator()(Individual& ind, const double pm, const double eta) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();

    // Random generator.
    uniform_real_distribution<double> dis(0.0, 1.0);

    // Mutation.
    bool perturb = false;
    for (size_t i = 0; i < numVariables; ++i)
    {
        double x = ind.encoding()[i];
        if (dis(gen) < pm && (x >= 0.0 && x <= 1.0))
        {
            perturb = true;

            double delta;

            double r1 = dis(gen);
            if (r1 <= 0.5)
            {
                // (2 * r1) ^ ( 1 / (eta + 1)) - 1.
                delta = pow((2.0 * r1 + (1.0 - 2 * r1) * pow(1.0 - x, eta + 1.0)), (1.0 / (eta + 1.0))) - 1.0;
            }
            else
            {
                // 1 - (2 * (1 - r1)) ^ (1 / (eta + 1)).
                delta = 1.0 - pow((2.0 * (1 - r1) + 2 * (r1 - 0.5) * pow(1.0 - x, eta + 1)), (1.0 / (eta + 1.0)));
            }

            ind.encoding()[i] += delta;
        }
    }

    return perturb;
}

