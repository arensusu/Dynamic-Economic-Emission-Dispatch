#ifndef MUT__
#define MUT__

class Population;
class Individual;

class RandOneMutation
{
public:
    void operator()(Population&, const double, const int) const;
};


class BestOneMutation
{
public:
    void operator()(Population&, const double, const int) const;
};

class PolynToCurMutation
{
public:
    void operator()(Population&, const double, const std::size_t, const double eta = 20) const;
};

class PolynamialMutation
{
public:
    void operator()(Individual&, const double eta = 20) const;
};

#endif