
#include <vector>
#include <algorithm>

#include "algorithm_NSDE.h"

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

void NSDE::Solve(Population& sol, const BProblem& prob, Log& log)
{
    ProportionDivisionCH ch;
    //ZeroOneCH ch;
    //InequalityCH ch;

    RandomInitialization initialization;

    RandOneMutation mutation;

    vector<double> params(2);
    params[0] = F_;
    params[1] = CR_;

    EnvSelection env;

    PolynomialMutation diversity;

    Population pop[2] = { Population(Psize_) };
    size_t curr = 0, next = 1;
    int ffe = 0;

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

    //print objectives
    log.All(pop[curr]);
    log.Detail(pop[curr]);

    while (true)
    {
        for (size_t i = 0; i < Psize_; ++i)
        {
            mutation.AllNeigh(pop[next][i], pop[curr], params);
            diversity(pop[next][i], double(1.0 / prob.numVariables()));

            //CH
            ch(pop[next][i]);
            prob.Evaluate(pop[next][i]);
            ffe++;
        }

        //selection
        Population hybrid(pop[next], pop[curr]);
        pop[next].clear();
        pop[next].resize(Psize_);

        env.CDP(pop[next], hybrid);

        //print objectives
        log.All(pop[next]);
        log.Detail(pop[next]);

        pop[curr] = pop[next];

        if (ffe >= maxffe_)
        {
            break;
        }
    }

    sol = pop[curr];
    log.RecordIGD();
}