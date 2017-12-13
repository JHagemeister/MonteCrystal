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

	virtual double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);	

	virtual Threedim effective_field(int position);

protected:
	int const _nbors; ///< number of neighbors per lattice site
	int const * const _neighborArray; ///< indexes of neighbor sites
};

#endif /* BIQUADRATICINTERACTION_H_ */
