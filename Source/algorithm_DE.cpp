
#include <fstream>
#include <random>

#include "algorithm_DE.h"
#include "population.h"
#include "individual.h"
#include "problem_base.h"

#include "alg_constraint_handling.h"
#include "alg_initialization.h"
#include "alg_crossover.h"
#include "alg_mutation.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"
#include "alg_sorting.h"
#include "localsearch.h"

#include "log.h"
#include "tool.h"

#include <algorithm>

using namespace std;

bool DE::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;

    return true;
}

void DE::Adaptive(const int ffe)
{
    // Exponetial.
    F_ = Fmin_ + (Fmax_ - Fmin_) * exp(-2.0 * double(ffe) / double(maxffe_));
    CR_ = CRmin_ + (CRmax_ - CRmin_) * exp(-2.0 * (1.0 - double(ffe) / double(maxffe_)));
    
    // Linear.
    //F_ = Fmax_ - (Fmax_ - Fmin_) * (double(ffe) / double(maxffe_));
    //CR_ = CRmin_ + (CRmax_ - CRmin_) * (double(ffe) / double(maxffe_));
}