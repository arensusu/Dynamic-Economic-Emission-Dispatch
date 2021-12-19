#ifndef MUT__
#define MUT__

class Population;
class Individual;

class RandOneMutation
{
public:
    void operator()(Population& pop, const double F, const std::size_t pos) const;
};

class BestOneMutation
{
public:
    void operator()(Population& pop, const double F, const std::size_t pos) const;
};

class CurrentToBestMutation
{
public:
    void operator()(Population& pop, const double F, const std::size_t pos) const;
};

#endif