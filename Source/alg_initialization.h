#ifndef INIT__
#define INIT__

class Population;
class BProblem;

class RandomInitialization
{
public:
    void operator()(Population&, const BProblem&) const;
};

#endif