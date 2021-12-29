#ifndef CH__
#define CH__

class Individual;
class BProblem;

void InequalityConstraint(std::vector<double>& curr, const std::vector<double>& prev, const BProblem& prob);

class DivisionCH
{
public:
    void operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

class FineTuningCH
{
public:
    void operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

class TFineTuningCH
{
public:
    void operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

class ProportionDivisionCH
{
public:
    void operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

#endif