#ifndef NSGAIII__
#define NSGAIII__

#include <cstddef>
#include <string>
#include <fstream>
#include <vector>

#include "../algorithm_base.h"

// ----------------------------------------------------------------------------------
//		NSGAIII
//
// Deb and Jain, "An Evolutionary Many-Objective Optimization Algorithm Using 
// Reference-point Based Non-dominated Sorting Approach, Part I: Solving Problems with 
// Box Constraints," IEEE Transactions on Evolutionary Computation, to appear.
//
// http://dx.doi.org/10.1109/TEVC.2013.2281535
// ----------------------------------------------------------------------------------

class BProblem;
class Population;
class Log;

class CNSGAIII : public BaseEA
{
public:
	explicit CNSGAIII();
	bool Setup(std::ifstream &ifile);
	void Solve(Population &solutions, const BProblem &prob, Log& log);

	const std::string & name() const { return name_; }
private:
	std::string name_;
	std::vector<std::size_t> obj_division_p_;
	std::size_t maxFFE_ = 0;
	double	F_ = 0.5,
			CR_ = 0.5;

};


#endif