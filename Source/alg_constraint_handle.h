#ifndef CH__
#define CH__

class Individual;
class BProblem;

class DivisionCH
{
public:
    void operator()(Individual&, const BProblem&) const;
};

#endif