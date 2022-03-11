
#include <string>
#include <algorithm>

#include "algorithm_MOEADPPS-DE.h"
#include "alg_initialization.h"
#include "alg_constraint_handling.h"
#include "alg_mating_selection.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"

#include "population.h"
#include "individual.h"
#include "problem_base.h"
#include "log.h"

#include "tool.h"
#include "alg_sorting.h"

using namespace std;

bool MOEADPPSDE::Setup(ifstream& file)
{
    string dummy;

    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;
    file >> dummy >> dummy >> neighborhood_;

    return true;
}

void MOEADPPSDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    //InequalityCH ch;
    ProportionDivisionCH ch;

    RandomInitialization initialize;

    RandomMatingSelection mating;

    RandOneMutation mutate;

    BinaryCrossover crossover;

    PolynamialMutation diversity;

    Population pop(Psize_), archive;
    size_t ffe = 0;

    controlGeneration_ = maxffe_ * 4 / 5;

    WeightVectorInitialization(Psize_, prob.numObjectives());

    initialize(pop, prob);
    for (size_t i = 0; i < Psize_; ++i)
    {
        ch(pop[i]);
        prob.Evaluate(pop[i]);
        ffe++;

        UpdateArchive(archive, pop[i]);
    }

    size_t iter = 1;
    while (true)
    {
        log.All(archive);
        log.Detail(archive);

        if (ffe >= maxffe_)
        {
            break;
        }

        UpdateNadir(pop);
        UpdateReference(pop);

        if (references_.size() == length_)
        {
            changeRate_ = CalMaxChange();
        }
        else
        {
            references_.push(referencePoint_);
            nadirs_.push(nadir_);
        }

        if (iter < controlGeneration_)
        {
            if (changeRate_ < threshold_ && pushStage_ == true)
            {
                pushStage_ = false;

                for (size_t i = 0; i < Psize_; ++i)
                {
                    double violation = pop[i].violation();
                    if (violation > epsilon0_)
                    {
                        epsilon0_ = violation;
                    }
                }

                epsilon_ = epsilon0_;
            }
            else if (pushStage_ == false)
            {
                UpdateEpsilon(pop, iter);
            }
        }
        else
        {
            epsilon_ = 0.0;
        }

        Individual children;
        
        for (size_t i = 0; i < Psize_; ++i)
        {
            mutate(children, pop, neighborIndice_[i], F_);

            crossover(children, pop[i], CR_);

            ch(children);
            prob.Evaluate(children);
            ffe++;

            UpdateReference(children.objs());

            // Update neighbor.
            vector<size_t> orders(neighborhood_);
            RandomPermutation(orders);

            size_t count = 0;
            for (size_t n = 0; n < orders.size(); ++n)
            {
                if (count >= maxReplace_)
                {
                    break;
                }

                size_t neighborIndex = neighborIndice_[i][orders[n]];

                if (pushStage_ == true)
                {
                    Push(pop[neighborIndex], children, neighborIndex);
                }
                else
                {
                    Pull(pop[neighborIndex], children, epsilon_, neighborIndex);
                }
                count++;
            }
        }

        UpdateArchive(archive, pop);

        iter++;
    }

    sol = pop;
    log.RecordIGD();
}

void MOEADPPSDE::WeightVectorInitialization(const size_t size, const size_t objs)
{
    weightVectors_.resize(size, vector<double>(objs));

    ifstream weightFile("./Weightvector/" + to_string(size) + "_" + to_string(objs) + ".txt");
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < objs; ++j)
        {
            weightFile >> weightVectors_[i][j];
        }
    }

    neighborIndice_.resize(size, vector<size_t>(neighborhood_));

    vector<vector<double>> dis(size, vector<double>(2));
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            dis[j][0] = DistanceNorm2(weightVectors_[i], weightVectors_[j]);
            dis[j][1] = j;
        }

        sort(dis.begin(), dis.end());

        for (size_t j = 0; j < neighborhood_; ++j)
        {
            neighborIndice_[i][j] = size_t(dis[j][1]);
        }
    }
}

void MOEADPPSDE::UpdateReference(const vector<double>& objs)
{
    if (referencePoint_.size() == 0)
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            referencePoint_.push_back(objs[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            double& min = referencePoint_[i];
            double now = objs[i];

            if (now < min)
            {
                min = now;
            }
        }
    }
}

void MOEADPPSDE::UpdateReference(const Population& pop)
{
    for (size_t i = 0; i < pop.size(); ++i)
    {
        UpdateReference(pop[i].objs());
    }
}

void MOEADPPSDE::UpdateNadir(const vector<double>& objs)
{
    if (nadir_.size() == 0)
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            nadir_.push_back(objs[i]);
        }
    }
    else
    {
        for (size_t i = 0; i < objs.size(); ++i)
        {
            double& max = nadir_[i];
            double now = objs[i];

            if (now > max)
            {
                max = now;
            }
        }
    }
}

void MOEADPPSDE::UpdateNadir(const Population& pop)
{
    nadir_.clear();

    for (size_t i = 0; i < pop.size(); ++i)
    {
        UpdateNadir(pop[i].objs());
    }
}

void MOEADPPSDE::UpdateNeighbor(Population& pop, const Individual& ind, const size_t index) const
{
    size_t count = 0;
    for (size_t i = 0; i < neighborhood_; ++i)
    {
        size_t neighborIndex = neighborIndice_[index][i];
        Individual& target = pop[neighborIndex];

        double now = Tchebycheff(ind.objs(), weightVectors_[neighborIndex], referencePoint_, nadir_);
        double origin = Tchebycheff(target.objs(), weightVectors_[neighborIndex], referencePoint_, nadir_);
        if (now <= origin && count < maxReplace_)
        {
            target = ind;
            count++;
        }
    }
}

double MOEADPPSDE::CalMaxChange()
{
    double delta = 1e-6;

    vector<double> prevReference = references_.front();
    vector<double> prevNadir = nadirs_.front();
    references_.pop();
    nadirs_.pop();

    double rzMax = 0.0, rnMax = 0.0;
    for (size_t i = 0; i < referencePoint_.size(); ++i)
    {
        rzMax = max(abs(referencePoint_[i] - prevReference[i]) / max(prevReference[i], delta), rzMax);
        rnMax = max(abs(nadir_[i] - prevNadir[i]) / max(prevNadir[i], delta), rnMax);
    }

    references_.push(referencePoint_);
    nadirs_.push(nadir_);

    return max(rzMax, rnMax);
}

void MOEADPPSDE::UpdateEpsilon(const Population& pop, const size_t k)
{
    const double alpha = 0.95;
    const double tau = 0.1;
    const double cp = 2;

    double feasible = 0.0, infeasible = 0.0;
    for (size_t i = 0; i < Psize_; ++i)
    {
        if (pop[i].violation() <= epsilon_)
        {
            feasible += 1;
        }
        else
        {
            infeasible += 1;
        }
    }

    if ((feasible + 1.0 / infeasible + 1.0) < alpha)
    {
        epsilon_ = (1 - tau) * epsilon_;
    }
    else
    {
        epsilon_ = epsilon0_ * pow(1 - double(k) / double(controlGeneration_), cp);
    }
}

bool MOEADPPSDE::Push(Individual& neighbor, const Individual& ind, const size_t index) const
{
    double origin = Tchebycheff(neighbor.objs(), weightVectors_[index], referencePoint_, nadir_);
    double trial = Tchebycheff(ind.objs(), weightVectors_[index], referencePoint_, nadir_);

    if (trial <= origin)
    {
        neighbor = ind;
        return true;
    }

    return false;
}

bool MOEADPPSDE::Pull(Individual& neighbor, const Individual& ind, const double epsilon, const size_t index) const
{
    double origin = Tchebycheff(neighbor.objs(), weightVectors_[index], referencePoint_, nadir_);
    double trial = Tchebycheff(ind.objs(), weightVectors_[index], referencePoint_, nadir_);

    if ((neighbor.violation() < epsilon && ind.violation() < epsilon) || (neighbor.violation() == ind.violation()))
    {
        if (trial <= origin)
        {
            neighbor = ind;
            return true;
        }
    }
    else if (ind.violation() < neighbor.violation())
    {
        neighbor = ind;
        return true;
    }

    return false;
}

void MOEADPPSDE::UpdateArchive(Population& arch, const Individual& ind) const
{
    if (arch.size() == 0)
    {
        arch.push_back(ind);
    }
    else
    {
        bool beDominated = false;
        vector<size_t> dominate;

        for (size_t i = 0; i < arch.size(); ++i)
        {
            if (FeasibleDominated(ind, arch[i]))
            {
                dominate.push_back(i);
            }
            else if (FeasibleDominated(arch[i], ind) || ind == arch[i])
            {
                beDominated = true;
            }
        }

        if (!beDominated)
        {
            for (int i = dominate.size() - 1; i >= 0; --i)
            {
                arch.erase(dominate[i]);
            }

            arch.push_back(ind);
        }
    }

    if (arch.size() > Psize_)
    {
        vector<size_t> orders(arch.size());
        Permutation(orders);

        CrowdingDistanceSort(orders, arch);

        size_t removeNum = arch.size() - Psize_;

        for (size_t i = 0; i < removeNum; ++i)
        {
            arch.erase(orders[orders.size() - 1 - i]);
        }
    }
}

void MOEADPPSDE::UpdateArchive(Population& arch, const Population& pop) const
{
    Population hybrid(arch, pop);
    arch.clear();

    vector<vector<size_t>> fronts = NondominatedSort(hybrid);

    if (fronts[0].size() <= Psize_)
    {
        arch.resize(fronts[0].size());
    }
    else
    {
        arch.resize(Psize_);
        
        CrowdingDistanceSort(fronts[0], hybrid);
    }

    for (size_t i = 0; i < arch.size(); ++i)
    {
        arch[i] = hybrid[fronts[0][i]];
    }
}
