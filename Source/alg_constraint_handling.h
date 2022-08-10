#ifndef CH__
#define CH__

class Individual;
class BProblem;

class ZeroOneCH
{
public:
    void operator()(Individual& ind) const;
};

class InequalityCH
{
public:
    void operator()(Individual& ind) const;
    void operator()(std::vector<double>& curr, const std::vector<double>& prev, const BProblem& prob) const;
};

class DivisionCH
{
public:
    int operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

class FineTuningCH
{
public:
    int operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

class ProportionDivisionCH
{
public:
    int operator()(Individual& ind, const std::size_t maxTry = 100, const double threshold = 0.00001) const;
};

#endif