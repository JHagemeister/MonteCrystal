/*
* HexagonalAnisotropyEnergy.h
*
*  Created on: 26.03.2012
*      
*
*      This class is supposed to model the anisotropy energy of a hexagonal system.
*      It inherits from the class Energy.
*/

#ifndef HEXAGONALANISOTROPYENERGY_H_
#define HEXAGONALANISOTROPYENERGY_H_

#include "Energy.h"

#include "typedefs.h"

/// Hexagonal anisotropy energy

class HexagonalAnisotropyEnergy : public Energy
{
public:
	HexagonalAnisotropyEnergy(Threedim* spinArray, double energyParameter1, double energyParameter2, 
		double energyParameter3);
	virtual ~HexagonalAnisotropyEnergy();

	virtual double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);

protected:
	double _energyParameter1;
	double _energyParameter2;
	double _energyParameter3;
};

#endif /* HEXAGONALANISOTROPYENERGY_H_ */
