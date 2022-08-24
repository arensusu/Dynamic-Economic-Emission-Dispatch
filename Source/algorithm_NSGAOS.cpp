
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include "algorithm_NSGAOS.h"

#include "alg_initialization.h"
#include "alg_constraint_handling.h"
#include "alg_mutation.h"
#include "alg_crossover.h"
#include "alg_env_selection.h"
#include "alg_diversity.h"

#include "population.h"
#include "individual.h"
#include "log.h"
#include "tool.h"

using namespace std;


bool NSGAOS::Setup(ifstream& file)
{
    string dummy;
    file >> dummy >> dummy >> maxffe_;
    file >> dummy >> dummy >> Psize_;
    file >> dummy >> dummy >> F_;
    file >> dummy >> dummy >> CR_;

    return true;
}

void NSGAOS::Solve(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;

    RandomInitialization initialization;

    Mutation* reproduct[3];
    reproduct[0] = new RandOneMutation();
    reproduct[1] = new BestOneMutation();
    reproduct[2] = new RandTwoMutation();

    PolynomialMutation mutation;

    EnvSelection env;

    uniform_int_distribution<int> dis(0, numOperators_ - 1);
    Population pop[2] = { Population(Psize_) };
    size_t curr = 0, next = 1;
    int ffe = 0;

    // Set parameters of reproductions.
    vector<double> params(3, 0.0);
    params[0] = 0.5;
    params[1] = 0.5;

    //initialization
    initialization(pop[curr]);
    for (size_t i = 0; i < Psize_; ++i)
    {
        //CH
        ch(pop[curr][i]);
        prob.Evaluate(pop[curr][i]);
        ffe++;
    }
    pop[next] = pop[curr];

    while (true)
    {
        //print objectives
        log.All(pop[curr]);
        log.Detail(pop[curr]);

        if (ffe >= maxffe_)
        {
            break;
        }

        CreditAssign(pop[curr]);
        //log.OpCount(opCounts_);

        for (size_t i = 0; i < Psize_; ++i)
        {
            int index = 0;
            //int index = dis(gen);

            reproduct[index]->AllNeigh(pop[next][i], pop[curr], params);

            mutation(pop[next][i], 1.0 / double(prob.numVariables()), eta_);

            //CH
            ch(pop[next][i]);
            prob.Evaluate(pop[next][i]);
            ffe++;

            pop[next][i].op() = index;
        }
        //log.Child(pop[next]);

        //selection
        Population hybrid(pop[next], pop[curr]);
        pop[next].clear();
        pop[next].resize(Psize_);

        env.CDP(pop[next], hybrid);

        pop[curr] = pop[next];

    }

    sol = pop[curr];
    log.RecordIGD();
}

int NSGAOS::AOS() const
{
    vector<double> probability(numOperators_);
    for (size_t i = 0; i < numOperators_; ++i)
    {
        probability[i] = 0.1 + (1.0 - 0.1 * numOperators_) * double(opCounts_[i]) / double(Psize_);
    }

    uniform_real_distribution<double> dis(0.0, 1.0);
    double rand = dis(gen);

    int op = 0;
    for (size_t i = 0; i < numOperators_; ++i)
    {
        if (rand < probability[i])
        {
            op = i;
            break;
        }
        else
        {
            rand -= probability[i];
        }
    }

    return op;
}

void NSGAOS::CreditAssign(const Population& pop)
{
    opCounts_.clear();
    opCounts_.resize(numOperators_);

    for (size_t i = 0; i < pop.size(); ++i)
    {
        if (pop[i].op() != -1)
        {
            opCounts_[pop[i].op()] += 1;
        }
    }
}