#ifndef MATING__
#define MATING__

#include <vector>

class RandomMatingSelection
{
public:
    std::vector<size_t> operator()(const std::vector<size_t>& pool, const std::size_t num) const;

};
#endif