#ifndef POPULATION__
#define POPULATION__

#include <vector>

class Individual;

class Population
{
public:
    explicit Population(size_t s) : pop_(s) {}
    Population() {}
    ~Population() {}

    const Individual& operator[] (const int i) const { return pop_[i]; }
    Individual& operator[] (const int i) { return pop_[i]; }

    const size_t size() const { return pop_.size(); }

    void resize(const int s) { pop_.resize(s); }

    void clear() { pop_.clear(); }

private:
    std::vector<Individual> pop_;

};
#endif