
#include "problem_loss.h"

using namespace std;

void BLProblem::setB(ifstream& file, const int numMachine)
{
    for (int i = 0; i <= numMachine; ++i)
    {
        for (int j = 0; j <= numMachine; ++j)
        {
            file >> B_[i][j];
        }
    }

    return;
}