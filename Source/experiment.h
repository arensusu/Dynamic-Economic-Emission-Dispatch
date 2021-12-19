#ifndef EXP__
#define EXP__

#include <string>
#include <fstream>
#include <vector>

class BaseEA;
class BProblem;

void SetExperiment(BaseEA** ea, std::vector<std::string>& probList, std::ifstream& file);

void SetProblemType(BProblem** prob, const std::string& fname);

#endif