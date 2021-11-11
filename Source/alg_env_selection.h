#ifndef ENVSEL__
#define ENVSEL__

class Population;

class BasicEnvSelection
{
public:
    size_t operator()(Population&, Population&) const;
};

#endif