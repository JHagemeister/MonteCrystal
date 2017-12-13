/*
* UniaxialAnisotropyEnergy.cpp
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

#include "UniaxialAnisotropyEnergy.h"

// forward and further includes
#include "MyMath.h"

UniaxialAnisotropyEnergy::UniaxialAnisotropyEnergy(Threedim* spinArray, double energyParameter, 
	Threedim direction):  
	Energy(1, "E_UniaxAniso_K"+std::to_string(energyParameter)+"_dir("+ std::to_string(direction.x)+","
		+std::to_string(direction.y)+","+std::to_string(direction.z)+") ", spinArray, energyParameter)
{
	/**
	* @param[in] spinArray Spin configuration
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] direction Direction vector for spatial orientation of anisotropy axis is normalized
	*/

	_direction = MyMath::normalize(direction); // anisotropy direction
}

UniaxialAnisotropyEnergy::~UniaxialAnisotropyEnergy()
{
}

double UniaxialAnisotropyEnergy::single_energy(int position)
{
	double cos = MyMath::dot_product(_spinArray[position], _direction);
	return (_energyParameter * (1 - cos * cos));
}

double UniaxialAnisotropyEnergy::interaction_energy_between_two_spins(int position1, int position2)
{
	return 0;
}

Threedim UniaxialAnisotropyEnergy::effective_field(int position)
{
	double factor = 2*_energyParameter*MyMath::dot_product(_direction,_spinArray[position]);
	Threedim field = MyMath::mult(_direction,factor);
	return field;
}