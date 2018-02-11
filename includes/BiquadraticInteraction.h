/*
* BiquadratInteraction.h
*
*  Created on: 08.10.2014
*      
*/

#ifndef BIQUADRATICINTERACTION_H_
#define BIQUADRATICINTERACTION_H_

#include "Energy.h"

#include "typedefs.h"

/// Biquadratic interaction between two spins

class BiquadraticInteraction : public Energy
{
public:
	BiquadraticInteraction(Threedim* spinArray, double energyParameter, int* neighborArray, int nbors);
	virtual ~BiquadraticInteraction();

	virtual double single_energy(const int &position) const ;
	
	virtual Threedim effective_field(const int &position) const;

protected:
	int const _nbors; ///< number of neighbors per lattice site
	int const * const _neighborArray; ///< indexes of neighbor sites
};

#endif /* BIQUADRATICINTERACTION_H_ */
