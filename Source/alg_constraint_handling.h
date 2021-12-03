#ifndef CH__
#define CH__

class Individual;

class DivisionCH
{
public:
    void operator()(Individual&) const;
};

class FineTuningCH
{
public:
    void operator()(Individual&) const;
};

#endif