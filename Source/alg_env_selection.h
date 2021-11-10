#ifndef ENVSEL__
#define ENVSEL__

class Population;

class BasicEnvSelection
{
public:
    void operator()(Population&, Population&) const;
};

#endif