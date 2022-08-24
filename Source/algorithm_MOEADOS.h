#ifndef MOEADOS__
#define MOEADOS__

#include <vector>
#include <list>

#include "algorithm_MOEAD-DE.h"

class Individual;

class MOEADOS : public MOEADDE
{
public:
    // Basic of MOEA/D.
    explicit MOEADOS() : MOEADDE() 
    {
        opCounts_.resize(numOperators_, 0); 
        credits_.resize(numOperators_, 0); 
        n_.resize(numOperators_, 0); 
        rewards_.resize(numOperators_, 0);
        probability_.resize(numOperators_, 1.0 / double(numOperators_));
    }

    ~MOEADOS() {}

    virtual bool Setup(std::ifstream& file);
    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

    std::size_t FindSubproblem(const Individual& ind) const;

    bool UpdateNeighbor(Population& pop, const Individual& ind, const std::size_t index, Log& log);

    std::size_t FindtheBest(const std::size_t index, const Population& pop);

    // Operator selection.
    std::size_t AOS(const std::size_t index);
    void CreditAssign();

    void CreditAssign(const std::size_t op, const double credit);

    void CreditAssign(const Population& pop);

private:
    double eta_ = 20.0;

    std::size_t numOperators_ = 2;
    std::vector<int> opCounts_;
    
    std::vector<double> credits_;

    std::vector<double> rewards_;
    std::vector<int> n_;

    std::vector<double> probability_;

    std::vector<int> subCounts_;

    // Operator selection.
    double alpha_ = 0.1;
    double prmin_ = 0.0;

};

#endif