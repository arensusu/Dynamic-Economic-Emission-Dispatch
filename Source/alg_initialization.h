#ifndef INIT__
#define INIT__

class Population;
class Individual;
class BProblem;

class RandomInitialization
{
public:
    void operator()(Population& pop, const BProblem& prob) const;
    void operator()(Individual& ind, const BProblem& prob) const;
};

#endif