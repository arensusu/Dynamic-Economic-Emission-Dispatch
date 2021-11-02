#ifndef ENVSEL__
#define ENVSEL__

class Population;

class EnvSelection
{
public:
    void operator()(Population&, Population&, unsigned int) const;
};

#endif