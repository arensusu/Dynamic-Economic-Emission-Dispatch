#ifndef LS__
#define LS__

class Population;

class OneDimLS
{
public:
    bool operator()(Population& pop) const;
};

#endif