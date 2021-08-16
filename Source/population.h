#ifndef POPULATION__
#define POPULATION__

#include <vector>

#include "individual.h"

class Population
{
public:
    explicit Population() {}
    ~Population() {}

private:
    std::vector<Individual> pop;
};

#endif