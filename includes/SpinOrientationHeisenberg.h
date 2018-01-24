/*
* SpinOrientationHeisenberg.h
*
*  Created on: 26.03.2012
*      
*/

#ifndef SPINORIENTATIONHEISENBERG_H_
#define SPINORIENTATIONHEISENBERG_H_

#include "SpinOrientation.h"

/// Heisenberg spins
/**
* Spins can move freely on surface of unit sphere
*/
class SpinOrientationHeisenberg : public SpinOrientation 
{
public:
	SpinOrientationHeisenberg(int numberAtoms, std::shared_ptr<RanGen> ranGen);
	virtual ~SpinOrientationHeisenberg();

	virtual void random_orientation(void);
	virtual void set_spin_spiral(Threedim spiralR, Threedim spiralI, Threedim ek, double lambda, 
		Lattice* lattice);
	virtual void set_spin_spiral(Threedim kVector, Threedim position, int helicity, Lattice* lattice);
	virtual void set_ferromagnet(Threedim direction);
	virtual Threedim magnetisation(void);

	virtual void single_orientation(int position);
	virtual void restore_single_orientation();


protected:
	Threedim generate_random_spin(void);
};

#endif /* SPINORIENTATIONHEISENBERG_H_ */
