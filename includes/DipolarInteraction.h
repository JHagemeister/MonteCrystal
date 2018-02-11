/*
* DipolInteraction.h
*
*  Created on: 29.07.2014
*      
*/

#ifndef DIPOLARINTERACTION_H_
#define DIPOLARINTERACTION_H_

#include "Energy.h"

#include "typedefs.h"

class Lattice;

/// Dipolar interaction between spins

class DipolarInteraction : public Energy
{
public:
	DipolarInteraction(Threedim* spinArray, double magneticMoment, double latticeConstant, Lattice* lattice);
	virtual ~DipolarInteraction();

	virtual double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

protected:
	void setup_distance_array(Lattice* lattice); ///< calculate all distances between spins once at creation

	double** _distanceArray; ///< all distances between all spins (inverse and to power 3)
	Threedim** _distanceVectors; ///< all vectors between all spins (normalized)
	int** _indexArray; ///< all neighbor indexes for all spins
	int _numberAtoms; ///< number lattice sites
	double _prefactor; ///< interaction prefactor
};

#endif /* DIPOLARINTERACTION_H_ */
