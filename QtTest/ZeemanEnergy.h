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
	virtual double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);

	void set_direction(Threedim direction); ///< set direction of magnetic field
	Threedim get_direction(void);

protected:
	Threedim _direction; ///< direction of homogeneous magnetic field
};

#endif /* ZEEMANENERGY_H_ */
