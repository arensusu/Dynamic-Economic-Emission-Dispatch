#ifndef MUT__
#define MUT__

class Population;

class RandOneMutation
{
public:
    void operator()(Population&) const;
};

#endif