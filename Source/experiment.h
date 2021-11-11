#ifndef EXP__
#define EXP__

#include <string>
#include <fstream>
#include <vector>

class BaseEA;
class BProblem;

void SetExperiment(BaseEA*, std::vector<std::string>&, std::ifstream&);

void SetProblemConf(BProblem*, std::string& name);

#endif