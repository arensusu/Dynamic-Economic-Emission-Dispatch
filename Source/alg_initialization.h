#ifndef INIT__
#define INIT__

class Population;
class BProblem;

class RandomInitialization
{
public:
    void operator()(Population& pop, const BProblem& prob) const;
};

#endif