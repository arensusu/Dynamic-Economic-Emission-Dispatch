#ifndef MUT__
#define MUT__

#include <vector>

class Population;
class Individual;

class Mutation
{
public:
    virtual void AllNeigh(Individual& mutant, const Population& parents, const std::vector<double> params) const;

    virtual void PartNeigh(Individual& mutant,
        const Population& parents,
        const std::vector<std::size_t> neighborhood,
        const std::vector<double> params) const;

    virtual void operator()(Individual& mutant, const std::vector<Individual>& rands, const std::vector<double> params) const = 0;

protected:
    int nums_ = 0;
};

class RandOneMutation : public Mutation
{
public:
    RandOneMutation() { nums_ = 3; }

private:
    void operator()(Individual& mutant, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class RandTwoMutation : public Mutation
{
public:
    RandTwoMutation() { nums_ = 5; }

private:
    void operator()(Individual& mutant, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class CurrToRandOneMutation : public Mutation
{
public:
    CurrToRandOneMutation() { nums_ = 3; }

private:
    void operator()(Individual& mutant, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class CurrToRandTwoMutation : public Mutation
{
public:
    CurrToRandTwoMutation() { nums_ = 5; }

private:
    void operator()(Individual& mutant, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class SBXMutation : public Mutation
{
public:
    SBXMutation() { nums_ = 2; }

private:
    void operator()(Individual& child, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class NonLinearMutation : public Mutation
{
public:
    NonLinearMutation() { nums_ = 2; }

private:
    void operator()(Individual& child, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class CurrToBestOneMutation : public Mutation
{
public:
    CurrToBestOneMutation() { nums_ = 2; }

    void PartNeigh(Individual& mutant, const Population& parents,
        const std::vector<std::size_t> neighborhood,
        const std::vector<double> params) const;

private:
    void operator()(Individual& child, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

class BestOneMutation : public Mutation
{
public:
    BestOneMutation() { nums_ = 2; }

    void PartNeigh(Individual& mutant, const Population& parents,
        const std::vector<std::size_t> neighborhood,
        const std::vector<double> params) const;

private:
    void operator()(Individual& child, const std::vector<Individual>& rands, const std::vector<double> params) const;
};

#endif