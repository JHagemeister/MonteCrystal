/*
* SpinOrientationIsing.h
*
*  Created on: 13.03.2012
*      
*/

#ifndef SPINORIENTATIONISING_H_
#define SPINORIENTATIONISING_H_

#include "SpinOrientation.h"

/// Ising spins
/**
* Spins can take two directions only during Metropolis Monte Carlo. Spin dynamics does not make sense to use
* with this spin model? 
*/
class SpinOrientationIsing : public SpinOrientation 
{
public:
	SpinOrientationIsing(int numberAtoms, std::shared_ptr<RanGen> ranGen);
	virtual ~SpinOrientationIsing();
	virtual void random_orientation(void);
	virtual void set_spin_spiral(Threedim spiralR, Threedim spiralI, Threedim ek, double lambda, 
		Lattice* lattice);
	virtual void set_spin_spiral(Threedim kVector, Threedim position, int helicity, Lattice* lattice);
	virtual void set_ferromagnet(Threedim direction);
	virtual Threedim magnetisation(void);

	virtual void single_orientation(int position);
	virtual void restore_single_orientation(void);;

private:
	int generate_random_spin(void);
};

#endif /* SPINORIENTATIONISING_H_ */
