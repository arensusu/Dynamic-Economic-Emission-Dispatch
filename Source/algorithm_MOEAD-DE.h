#ifndef MOEADDE__
#define MOEADDE__

#include <vector>

#include "algorithm_base.h"
#include "algorithm_DE.h"

class Individual;

class MOEADDE : public DE
{
public:
    explicit MOEADDE() : DE("MOEADDE") {}
    ~MOEADDE() {}

    virtual bool Setup(std::ifstream& file);
    virtual void Solve(Population& sol, const BProblem& prob, Log& log);

    void WeightVectorInitialization(const std::size_t size, const std::size_t objs);

    void UpdateReference(const std::vector<double>& objs);
    void UpdateReference(const Population& pop);

    void UpdateNadir(const std::vector<double>& objs);
    void UpdateNadir(const Population& pop);

    virtual void UpdateNeighbor(Population& pop, const Individual& ind, const std::size_t index);

    void UpdateArchive(Population& arch, const Individual& ind) const;
    void UpdateArchive(Population& arch, const Population& pop) const;

protected:
    std::size_t neighborhood_ = 10;
    std::size_t maxReplace_ = 1;

    std::vector<std::vector<double>> weightVectors_;
    std::vector<std::vector<std::size_t>> neighborIndice_;
    std::vector<double> pi_;
    std::vector<double> referencePoint_;
    std::vector<double> nadir_;

};

#endif