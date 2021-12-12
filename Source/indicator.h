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

    double operator()(const Population&);

    double val() const { return val_; }

private:
    std::vector<std::vector<double>> refs_;
    double val_ = 0;

};

class Compromise
{
public:
    std::size_t operator()(const Population&) const;

};

#endif