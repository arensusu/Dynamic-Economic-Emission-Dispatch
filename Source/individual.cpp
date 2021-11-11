

#include "individual.h"

using namespace std;

const BProblem* Individual::problem_ = 0;

ostream& operator<<(ostream& os, const Individual& ind)
{
    vector<double> powers = ind.encoding();
    
    double sum = 0;
    for (size_t i = 0; i < powers.size(); ++i)
    {
        sum += powers[i];
    }
    os << sum;

    for (size_t i = 0; i < powers.size(); ++i)
    {
        os << ", " << powers[i];
    }
    os << endl;

    return os;
}