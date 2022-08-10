


#include "experiment.h"
#include "algorithm_base.h"
#include "algorithm_DE.h"
#include "algorithm_jDE.h"
#include "algorithm_MOEAD-DE.h"
#include "algorithm_NSDE.h"
#include "algorithm_CCNSDE.h"
#include "algorithm_NSGAOS.h"
#include "algorithm_MOEADOS.h"
#include "algorithm_MOEADGR.h"
#include "algorithm_BPMOEAD.h"

#include "problem_base.h"
#include "problem_noloss.h"
#include "problem_without_B0.h"
#include "problem_with_B0.h"
#include "problem_MW1.h"
#include "problem_MW2.h"
#include "problem_MW3.h"
#include "problem_MW5.h"
#include "problem_MW6.h"
#include "problem_MW7.h"
#include "problem_MW9.h"
#include "problem_MW10.h"
#include "problem_MW11.h"
#include "problem_MW12.h"
#include "problem_MW13.h"
#include "problem_ZDT1.h"

using namespace std;

void SetExperiment(BaseEA** ea, vector<string>& probList, ifstream& file)
{
    string algoName, dummy;
    
    file >> dummy >> dummy >> algoName;

    if (algoName == "jDE")
    {
        *ea = new JDE();
    }
    else if (algoName == "MOEAD")
    {
        *ea = new MOEADDE();
    }
    else if (algoName == "NSDE")
    {
        *ea = new NSDE();
    }
    else if (algoName == "CCNSDE")
    {
        *ea = new CCNSDE();
    }
    else if (algoName == "NSGAOS")
    {
        *ea = new NSGAOS();
    }
    else if (algoName == "MOEADOS")
    {
        *ea = new MOEADOS();
    }
    else if (algoName == "MOEADGR")
    {
        *ea = new MOEADGR();
    }
    else if (algoName == "BPMOEAD")
    {
        *ea = new BPMOEAD();
    }

    (*ea)->Setup(file);

    string prob;    
    while (file >> dummy >> dummy >> prob)
    {
        probList.push_back(prob);
    }

    return;
}

void SetProblemType(BProblem** prob, const string& fname)
{
    if (fname.find("WB") != string::npos)
    {
        *prob = new WBProblem();
        (*prob)->Read(fname.substr(0, (fname.size() - 3)));
    }
    else if (fname.find("WOB") != string::npos)
    {
        *prob = new WOBProblem();
        (*prob)->Read(fname.substr(0, (fname.size() - 4)));
    }
    else if (fname.find("N") != string::npos)
    {
        *prob = new NProblem();
        (*prob)->Read(fname.substr(0, (fname.size() - 2)));
    }
    else if (fname == "MW1")
    {
        *prob = new MW1();
    }
    else if (fname == "MW2")
    {
        *prob = new MW2();
    }
    else if (fname == "MW3")
    {
        *prob = new MW3();
    }
    else if (fname == "MW5")
    {
        *prob = new MW5();
    }
    else if (fname == "MW6")
    {
        *prob = new MW6();
    }
    else if (fname == "MW7")
    {
        *prob = new MW7();
    }
    else if (fname == "MW9")
    {
        *prob = new MW9();
    }
    else if (fname == "MW10")
    {
        *prob = new MW10();
    }
    else if (fname == "MW11")
    {
        *prob = new MW11();
    }
    else if (fname == "MW12")
    {
        *prob = new MW12();
    }
    else if (fname == "MW13")
    {
        *prob = new MW13();
    }
    else if (fname == "ZDT1")
    {
        *prob = new ZDT1();
    }

    return;
}