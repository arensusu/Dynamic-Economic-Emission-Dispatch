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
    void operator()(Population& mutants, 
                    const Population& parent, 
                    const std::vector<std::vector<size_t>>& neighborhood,
                    const double F) const;
    void operator()(Individual& mutant, const Population& rands, const double F) const;
    void operator()(Individual& mutant, const Population& parent, const std::vector<std::size_t> neighborhood, const double F) const;
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