#ifndef XOVER__
#define XOVER__

class Individual;

class BinaryCrossover
{
public:
    void operator()(const Individual& target, Individual& mutant, const double CR) const;
};

#endif