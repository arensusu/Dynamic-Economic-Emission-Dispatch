#ifndef XOVER__
#define XOVER__

class Individual;

class BinaryCrossover
{
public:
    void operator()(const Individual&, Individual&, const double) const;
};

#endif