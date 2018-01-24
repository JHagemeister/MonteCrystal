/*
* ZeemanEnergy.cpp
*
* Copyright 2017 Julian Hagemeister
*
* This file is part of MonteCrystal.
*
* MonteCrystal is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MonteCrystal is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with MonteCrystal.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "ZeemanEnergy.h"

// forward and further includes
#include "MyMath.h"

ZeemanEnergy::ZeemanEnergy(Threedim* spinArray, double energyParameter, Threedim direction) :
	Energy(1, "E_Zeeman ", spinArray, energyParameter)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] direction Direction of magnetic field.
	*/
	_direction = MyMath::normalize(direction);
}

ZeemanEnergy::~ZeemanEnergy()
{
}

double ZeemanEnergy::single_energy(int position)
{
	return -MyMath::dot_product(_spinArray[position], _direction) * _energyParameter;
}

double ZeemanEnergy::interaction_energy_between_two_spins(int position1, int position2) 
{
	return 0;
}

Threedim ZeemanEnergy::effective_field(int position)
{
	return MyMath::mult(_direction,_energyParameter);
}

void ZeemanEnergy::set_direction(Threedim direction) {
	_direction = MyMath::normalize(direction);
}

Threedim ZeemanEnergy::get_direction(void)
{
	return _direction;
}
