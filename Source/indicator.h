#ifndef INDICATOR__
#define INDICATOR__

#include <vector>
#include <string>

class Population;

class IGD
{
public:
    explicit IGD(const std::string&);
    IGD() {}

    double operator()(const Population&) const;

private:
    std::vector<std::vector<double>> refs_;

};

#endif