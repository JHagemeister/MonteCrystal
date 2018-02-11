/*
* UniaxialAnisotropyEnergyDefect.h
*
*
*      This class is supposed to model the anisotropy energy.
*      It inherits from the class Energy.
*/

#ifndef UNIAXIALANISOTROPYENERGYDEFECT_H_
#define UNIAXIALANISOTROPYENERGYDEFECT_H_

#include "Energy.h"

#include <unordered_map>

#include "typedefs.h"

/// Uniaxial first order anisotropy energy

class UniaxialAnisotropyEnergyDefect : public Energy
{
public:
	UniaxialAnisotropyEnergyDefect(Threedim* spinArray, 
		std::unordered_map <int, UniaxialAnisotropyStruct> anisotropyDefects);
	virtual ~UniaxialAnisotropyEnergyDefect();

	virtual double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

protected:
	std::unordered_map <int, UniaxialAnisotropyStruct> _anisotropyDefects;
};

#endif /* UNIAXIALANISOTROPYENERGY_H_ */

