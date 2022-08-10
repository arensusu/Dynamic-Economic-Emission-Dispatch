#ifndef INIT__
#define INIT__

class Population;
class Individual;
class BProblem;

class RandomInitialization
{
public:
    void operator()(Population& pop) const;
    void operator()(Individual& ind) const;
};

class SubProblemInitialization
{
public:
    void operator()(Population& pop, std::size_t index) const;
    void operator()(Individual& ind, std::size_t index) const;
};

#endif