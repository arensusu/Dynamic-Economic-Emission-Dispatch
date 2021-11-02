#ifndef XOVER__
#define XOVER__

class Population;

class BinaryCrossover
{
public:
    void operator()(Population&, const int) const;
};

#endif