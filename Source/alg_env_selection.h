#ifndef ENVSEL__
#define ENVSEL__

class Population;
class Individual;

class BasicEnvSelection
{
public:
    std::size_t operator()(Population& children, const Population& parents) const;
};

class GreedyEnvSelection
{
public:
    void operator()(Population& children, const Population& parents) const;
};

bool FeasibleCompare(std::vector<double>& l, std::vector<double>& r);

class OneDimEnvSelection
{
public:
    void operator()(Population& children, const Population& parents, const std::size_t obj = 1) const;

private:
};

class SPEA2EnvSelection
{
public:
    Population operator()(const Population& archive, const Population& pop) const;
};

#endif