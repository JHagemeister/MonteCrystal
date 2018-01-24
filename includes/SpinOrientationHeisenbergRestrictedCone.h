/*
* SpinOrientationHeisenbergRestrictedCone.h
*
*  Created on: 28.05.2015
*      
*/

#ifndef SPINORIENTATIONHEISENBERGRESTRICTEDCONE_H_
#define SPINORIENTATIONHEISENBERGRESTRICTEDCONE_H_

#include "SpinOrientationHeisenberg.h"

/// Heisenberg spins with restricted update freedom
/**
* Spins can move freely on surface of unit sphere but their direction change during Metropolis Monte Carlo is
* restricted to a cone
*/
class SpinOrientationHeisenbergRestrictedCone : public SpinOrientationHeisenberg
{
public:
	SpinOrientationHeisenbergRestrictedCone(int numberAtoms, std::shared_ptr<RanGen> ranGen, double sigma);
	virtual ~SpinOrientationHeisenbergRestrictedCone();

	virtual void single_orientation(int position);	

private:
	void make_random_change_within_cone(int position);
	
	double _sigma;

};

#endif /* SPINORIENTATIONHEISENBERGRESTRICTEDCONE_H_ */
