
#include <vector>
#include <chrono>
#include <random>

#include "alg_diversity.h"
#include "individual.h"

using namespace std;

void PolynamialMutation::operator()(Individual& ind, const double pm, const double eta) const
{
    // Parameters.
    size_t numVariables = Individual::prob().numVariables();

    // Random generator.
    default_random_engine gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dis(0.0, 1.0);

    // Mutation.
    for (size_t i = 0; i < numVariables; ++i)
    {
        if (dis(gen) < pm)
        {
            double delta;

            double r1 = dis(gen);
            if (r1 <= 0.5)
            {
                // (2 * r1) ^ ( 1 / (eta + 1)) - 1.
                delta = pow((2.0 * r1), (1.0 / (eta + 1))) - 1.0;
            }
            else
            {
                // 1 - (2 * (1 - r1)) ^ (1 / (eta + 1)).
                delta = 1.0 - pow((2.0 * (1 - r1)), (1.0 / (eta + 1.0)));
            }

            ind.encoding()[i] += + delta * 1.0;
        }
    }
}

