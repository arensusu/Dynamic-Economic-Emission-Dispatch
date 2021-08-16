

#include "algorithm_DE.h"
#include "population.h"

using namespace std;

void DE::setup(ifstream& file)
{
    if (file.eof())
    {
        iteration = 100;
        F_ = 0.5;
        CR_ = 0.5;
    }
}

void DE::operator() (Population& pop)
{
    initialization();
    pop = pop_;

    for (int i = 0; i < iteration_; ++i)
    {
        pop_.clear();
        for (size_t j = 0; j < pop_.size(); ++j)
        {
            mutation();
            crossover();
            selection();
        }

        pop = pop_;
    }
}

void DE::initialization()
{
    
}

void DE::mutation()
{

}

void DE::crossover()
{

}

void DE::selection()
{

}