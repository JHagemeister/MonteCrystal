/*
* Tip.h
*
*  Created on: 08.01.2013
*      
*/

#ifndef TIP_H_
#define TIP_H_

#include "Energy.h"

#include "typedefs.h"

class Lattice;

/// Magnetic tip (e.g. scanning tunneling microscope)

class Tip : public Energy
{
public:
	Tip(Threedim* spinArray, double energyParameter, Threedim tipPosition, Threedim tipDirection, 
		Lattice* lattice);
	virtual ~Tip();
	virtual double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);

	void set_position(Threedim position); ///< set tip position
	void set_direction(Threedim tipDirection); ///< set magnetization direction

protected:
	void set_distance_array(void); ///< setup distances between tip and atoms

	double _k; ///< decay length [multiples of lattice constants or same as lattice coordinates]
	Threedim _tipPosition; ///< tip position
	Threedim _tipDirection; ///< normalized direction vector of tip magnetization
	Threedim* _latticeCoordArray; ///< lattice coordinates
	double* _distanceArray; ///< distance between tip and lattice sites
	int _numberAtoms; ///< number of lattice sites
	
};

#endif /* TIP_H_ */
