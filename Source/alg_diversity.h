#ifndef DIVERSITY__
#define DIVERSITY__

class Individual;

class PolynomialMutation
{
public:
    bool operator()(Individual& ind, const double pm = 0.1, const double eta = 20) const;
};

#endif
