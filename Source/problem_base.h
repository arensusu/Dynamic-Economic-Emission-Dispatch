#ifndef PROB_BASE__
#define PROB_BASE__

#include <string>

class BProblem
{
public:
    explicit BProblem() { setName(""); }
    ~BProblem() {}

    void setName(const std::string& name) { name_ = name; }

    virtual int read(const std::string&) = 0;

protected:
    std::string name_;

};

#endif