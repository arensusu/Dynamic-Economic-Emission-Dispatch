#ifndef XOVER__
#define XOVER__

class Individual;
class Population;

class BinaryCrossover
{
public:
    void operator()(Population& pop) const;
    void operator()(Population& pop, const double CR) const;
    void operator()(const Individual& target, Individual& mutant, const double CR) const;
};

#endif