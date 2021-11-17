

#include "individual.h"

using namespace std;

const BProblem* Individual::problem_ = 0;

ostream& operator<<(ostream& os, const Individual& ind)
{
    vector<double> powers = ind.encoding();
    
    for (size_t t = 0; t < Individual::prob().numPeriods(); ++t)
    {
        for (size_t i = 0; i < Individual::prob().numMachines(); ++i)
        {
            powers[t * Individual::prob().numMachines() + i] = Individual::prob().limit(i, 0) + powers[t * Individual::prob().numMachines() + i] * (Individual::prob().limit(i, 1) - Individual::prob().limit(i, 0));
        }
    }

    os << "Objectives: " << ind.objs()[0] << ", " << ind.objs()[1] << endl;

    for (size_t i = 0; i < powers.size(); ++i)
    {
        os << ", " << powers[i];
    }
    os << endl;

    return os;
}