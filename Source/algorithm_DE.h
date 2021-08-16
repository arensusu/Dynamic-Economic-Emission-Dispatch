#ifndef ALGO_DE__
#define ALGO_DE__

#include <fstream>

#include "algorithm_base.h"
#include "population.h"

class DE : public BaseEA
{
public:
    explicit DE() : BaseEA("DE"), F_(0), CR_(0) {}
    ~DE() {}

    void setup(std::ifstream&);

    virtual void operator() (Population&);

    void initialization();
    void mutation();
    void crossover();
    void selection();

private:
    Population pop_;
    Individual current_;
    Individual trial_;

    int iteration;
    double F_;
    double CR_;
};

#endif