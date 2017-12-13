/*
* UniaxialAnisotropy.h
*      
*
*      This class is supposed to model the anisotropy energy.
*      It inherits from the class Energy.
*/

#ifndef UNIAXIALANISOTROPYENERGY_H_
#define UNIAXIALANISOTROPYENERGY_H_

#include "Energy.h"

#include "typedefs.h"

/// Uniaxial first order anisotropy energy

class UniaxialAnisotropyEnergy : public Energy
{
public:
	UniaxialAnisotropyEnergy(Threedim* spinArray, double energyParameter, Threedim direction);
	virtual ~UniaxialAnisotropyEnergy();
	
	virtual double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);
	
	virtual Threedim effective_field(int position);

protected:
	Threedim _direction; ///< spatial orientation of anisotropy axis
};

#endif /* UNIAXIALANISOTROPYENERGY_H_ */
