#ifndef POPULATION__
#define POPULATION__

#include <vector>

#include "individual.h"

class Population
{
public:
    explicit Population(size_t s) : pop_(s) {}
    Population(std::vector<Individual>::const_iterator start, std::vector<Individual>::const_iterator end) : pop_(start, end) {}
    Population(const Population& p1, const Population& p2) : Population(p1) { this->push_back(p2); }
    Population() {}
    ~Population() {}

    const Individual& operator[] (const int i) const { return pop_[i]; }
    Individual& operator[] (const int i) { return pop_[i]; }

    std::vector<Individual>::const_iterator begin() const { return pop_.begin(); }
    std::vector<Individual>::const_iterator end() const { return pop_.end(); }
    std::vector<Individual>::iterator begin() { return pop_.begin(); }
    std::vector<Individual>::iterator end() { return pop_.end(); }

    void push_back(const Individual& ind)
    {
        bool same = false;
        for (size_t i = 0; i < pop_.size(); ++i)
        {
            if (ind == pop_[i])
            {
                same = true;
            }
        }

        if (!same)
        {
            pop_.push_back(ind);
        }
    }

    void push_back(const Population& p) 
    { 
        for (size_t i = 0; i < p.size(); ++i)
        {
            bool same = false;
            for (size_t j = 0; j < pop_.size(); ++j)
            {
                if (p[i] == pop_[j])
                {
                    same = true;
                }
            }

            if (!same) pop_.push_back(p[i]);
        }
    }

    const size_t size() const { return pop_.size(); }

    void resize(const int s) { pop_.resize(s); }

    void erase(const size_t index) { pop_.erase(pop_.begin() + index); }

    void clear() { pop_.clear(); }

private:
    std::vector<Individual> pop_;

};

#endif