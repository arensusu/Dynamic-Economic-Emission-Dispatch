#include "alg_nsgaiii.h"
#include "../problem_base.h"
#include "../individual.h"
#include "alg_reference_point.h"
#include "../population.h"

#include "../alg_initialization.h"
#include "../alg_constraint_handling.h"
#include "../alg_crossover.h"
#include "../alg_mutation.h"
#include "../alg_diversity.h"
#include "alg_environmental_selection.h"

#include "../log.h"

#include <vector>
#include <fstream>

using namespace std;

CNSGAIII::CNSGAIII(): 
	BaseEA("NSGAIII") {}
// ----------------------------------------------------------------------
bool CNSGAIII::Setup(ifstream &ifile)
{
	if (!ifile) return false;

	string dummy;

	size_t p1 = 0, p2 = 0;
	ifile >> dummy >> dummy >> p1 >> p2;

	obj_division_p_.push_back(p1);

	if (!ifile) ifile.clear();
	else obj_division_p_.push_back(p2);
	
	ifile >> dummy >> dummy >> maxFFE_;
	ifile >> dummy >> dummy >> F_;
	ifile >> dummy >> dummy >> CR_;

	return true;
}
// ----------------------------------------------------------------------
void CNSGAIII::Solve(Population &solutions, const BProblem &problem, Log& log)
{
	Individual::SetProblem(problem);
	
	vector<CReferencePoint> rps;
	GenerateReferencePoints(&rps, problem.numObjectives(), obj_division_p_); 
	size_t PopSize = rps.size();
	while (PopSize%4) PopSize += 1;

	Population pop[2]={Population(PopSize)};

	ProportionDivisionCH ch;
	RandomInitialization initialization;
	RandOneMutation reproduct;
	PolynomialMutation PolyMut;

	vector<double> params(2);
	params[0] = F_;
	params[1] = CR_;

	int cur = 0, next = 1;
	int ffe = 0;

	initialization(pop[cur]);
	for (size_t i=0; i<PopSize; i+=1)
	{
		ch(pop[cur][i]);
		problem.Evaluate(pop[cur][i]);
		ffe++;
	}

	while (true)
	{
		log.All(pop[cur]);
		log.Detail(pop[cur]);

		if (ffe >= maxFFE_)
		{
			break;
		}

		pop[next] = pop[cur];

		for (size_t i=0; i<PopSize; i+=1)
		{
			reproduct.AllNeigh(pop[next][i], pop[cur], params);
			PolyMut(pop[next][i], double(1.0 / problem.numVariables()));

			ch(pop[next][i]);
			problem.Evaluate(pop[next][i]);
			ffe++;
		}

		Population hybrid(pop[cur], pop[next]);
		pop[next].clear();

		EnvironmentalSelection(&pop[cur], &hybrid, rps, PopSize);

	}

	solutions = pop[cur];

	log.RecordIGD();
}