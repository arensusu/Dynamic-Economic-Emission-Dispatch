#ifndef MUT__
#define MUT__

class Population;
class Individual;

class RandOneMutation
{
public:
    void operator()(Population& pop) const;
    void operator()(Population& pop, const double F) const;
    void operator()(Population& pop, const std::size_t pos, const double F) const;
};

class BestOneMutation
{
public:
    void operator()(Population& pop) const;
    void operator()(Population& pop, const double F) const;
};

class CurrentToBestMutation
{
public:
    void operator()(Population& pop) const;
    void operator()(Population& pop, const double F) const;
};

#endif