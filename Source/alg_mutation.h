#ifndef MUT__
#define MUT__

class Population;

class RandOneMutation
{
public:
    void operator()(Population&, const double, const int) const;
};


class BestOneMutation
{
public:
    void operator() (Population&, const double, const int) const;
};

#endif