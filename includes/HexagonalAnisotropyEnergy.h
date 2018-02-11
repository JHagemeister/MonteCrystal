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

	virtual double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

protected:
	double _energyParameter1;
	double _energyParameter2;
	double _energyParameter3;
};

#endif /* HEXAGONALANISOTROPYENERGY_H_ */
