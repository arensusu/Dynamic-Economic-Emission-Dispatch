#ifndef XOVER__
#define XOVER__

class Individual;
class Population;

class BinaryCrossover
{
public:
    void operator()(Population& pop) const;
    void operator()(Population& pop, const double CR) const;
    void operator()(Individual& mutant, const Individual& target, const double CR) const;
    void operator()(Population& trial, const Population& target, const double CR) const;
};

#endif