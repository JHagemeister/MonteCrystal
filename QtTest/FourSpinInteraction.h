/*
* FourSpinInteraction.h
*
*  Created on: 29.07.2014
*      
*/

#ifndef FOURSPININTERACTION_H_
#define FOURSPININTERACTION_H_

#include "Energy.h"

#include "typedefs.h"

/// Higher order exchange interaction between four neighboring spins

class FourSpinInteraction : public Energy
{
public:
	FourSpinInteraction(Threedim* spinArray, double energyParameter, Fourdim* cells, int nCellsPerAtom);
	virtual ~FourSpinInteraction();
	virtual double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);

protected:
	int const _nCellsPerAtom; ///< number of four spin cells per atom
	Fourdim const * const _cells; ///< four spin cells consisting of 4 lattice sites indexes
};

#endif /* FOURSPININTERACTION_H_ */
