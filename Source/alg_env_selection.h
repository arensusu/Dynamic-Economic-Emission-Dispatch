#ifndef ENVSEL__
#define ENVSEL__

class Population;
class Individual;

class EnvSelection
{
public:
    std::size_t Basic(Population& next, Population& curr);
    std::size_t CDP(Population& next, const Population& curr);
    std::size_t PF(Population& next, Population& curr);

    virtual std::size_t Select(Population& next, const Population& curr) const;

    bool feasibility_ = false;
};

class GreedyEnvSelection : public EnvSelection
{
public:
    virtual std::size_t Select(Population& next, const Population& curr) const;
};

bool FeasibleCompare(std::vector<double>& l, std::vector<double>& r);

class OneDimEnvSelection
{
public:
    void operator()(Population& children, const Population& parents, const std::size_t obj = 0) const;

private:
};

class SPEA2EnvSelection
{
public:
    Population operator()(const Population& archive, const Population& pop) const;
};

#endif