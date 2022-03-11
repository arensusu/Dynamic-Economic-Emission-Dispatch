#ifndef MOEADPPSDE__
#define MOEADPPSDE__

#include <vector>
#include <queue>

#include "algorithm_base.h"
#include "algorithm_DE.h"

class Individual;

class MOEADPPSDE : public DE
{
public:
    explicit MOEADPPSDE() : DE("MOEADPPSDE"), neighborhood_(10) {}
    ~MOEADPPSDE() {}

    virtual bool Setup(std::ifstream& file);
    virtual void Solve(Population& sol, const BProblem& prob, Log& log);
    
    void WeightVectorInitialization(const std::size_t size, const std::size_t objs);

    void UpdateReference(const std::vector<double>& objs);
    void UpdateReference(const Population& pop);

    void UpdateNadir(const std::vector<double>& objs);
    void UpdateNadir(const Population& pop);

    void UpdateNeighbor(Population& pop, const Individual& ind, const std::size_t index) const;

    double CalMaxChange();

    void UpdateEpsilon(const Population& pop, const std::size_t k);

    bool Push(Individual& neighbor, const Individual& ind, const std::size_t index) const;
    bool Pull(Individual& neighbor, const Individual& ind, const double epsilon, const std::size_t index) const;

    void UpdateArchive(Population& arch, const Individual& ind) const;
    void UpdateArchive(Population& arch, const Population& pop) const;

private:
    std::size_t neighborhood_;
    std::size_t maxReplace_ = 1;

    std::vector<std::vector<double>> weightVectors_;
    std::vector<std::vector<std::size_t>> neighborIndice_;
    std::vector<double> referencePoint_;
    std::vector<double> nadir_;

    std::queue<std::vector<double>> references_;
    std::queue<std::vector<double>> nadirs_;

    bool pushStage_ = true;

    double changeRate_ = 1.0;
    double threshold_ = 1e-3;
    double length_ = 10;

    double epsilon_ = 0.0;
    double epsilon0_ = 0.0;

    std::size_t controlGeneration_ = 0;
};

#endif
