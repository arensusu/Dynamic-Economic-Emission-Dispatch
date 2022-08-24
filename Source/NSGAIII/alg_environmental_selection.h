#ifndef ENVIRONMENTAL_SELECTION__
#define ENVIRONMENTAL_SELECTION__
#include <vector>


// ----------------------------------------------------------------------
//	The environmental selection mechanism is the key innovation of 
//  the NSGA-III algorithm.
//
//  Check Algorithm I in the original paper of NSGA-III.
// ----------------------------------------------------------------------

class Population;
class CReferencePoint;

void EnvironmentalSelection(Population *pnext, // population in the next generation
							Population *pcur,  // population in the current generation
							std::vector<CReferencePoint> rps, // the set of reference points
							std::size_t PopSize);

#endif
