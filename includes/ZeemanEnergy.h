/*
* ExternalMagneticField.h
*
*  Created on: 03.01.2013
*      
*/

#ifndef ZEEMANENERGY_H_
#define ZEEMANENERGY_H_

#include "Energy.h"

#include "typedefs.h"

/// Zeeman energy (external magnetic field)

class ZeemanEnergy : public Energy
{
public:
	ZeemanEnergy(Threedim* spinArray, double energyParameter, Threedim direction);
	virtual ~ZeemanEnergy();
	virtual double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

	void set_direction(Threedim direction); ///< set direction of magnetic field
	Threedim get_direction(void) const;

protected:
	Threedim _direction; ///< direction of homogeneous magnetic field
};

#endif /* ZEEMANENERGY_H_ */
