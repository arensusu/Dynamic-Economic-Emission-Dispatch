#ifndef COMPARATOR__
#define COMPARATOR__

class Individual;

// ----------------------------------------------------------------------------------
//			BComparator : the base class of comparison operators
// ----------------------------------------------------------------------------------
class BComparator
{
public:
	virtual ~BComparator() {}

	virtual bool	operator()(const Individual &l, const Individual &r) const = 0;
};



// ----------------------------------------------------------------------------------
//			CParetoDominate
// ----------------------------------------------------------------------------------

class CParetoDominate : public BComparator 
{
public:
	virtual	bool	operator()(const Individual &l, const Individual &r) const;
};


extern CParetoDominate ParetoDominate;
#endif