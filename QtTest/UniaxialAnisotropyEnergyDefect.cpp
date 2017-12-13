/*
* UniaxialAnisotropyEnergyDefect.cpp
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

#include "UniaxialAnisotropyEnergyDefect.h"

// forward and further includes
#include "MyMath.h"

UniaxialAnisotropyEnergyDefect::UniaxialAnisotropyEnergyDefect(Threedim* spinArray, 
	std::unordered_map <int, UniaxialAnisotropyStruct> anisotropyDefects) :
	Energy(1, "E_UniaxAniso_K_Defect ", spinArray, 0)
{
	/**
	* @param[in] spinArray Spin configuration
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] direction Direction vector for spatial orientation of anisotropy axis is normalized
	*/

	_anisotropyDefects = anisotropyDefects;

	for (auto it = _anisotropyDefects.begin(); it != _anisotropyDefects.end(); ++it)
	{
		it->second.direction = MyMath::normalize(it->second.direction);
	}

}

UniaxialAnisotropyEnergyDefect::~UniaxialAnisotropyEnergyDefect()
{
}

double UniaxialAnisotropyEnergyDefect::single_energy(int position)
{
	double energy = 0;
	if (_anisotropyDefects.count(position) > 0)
	{
		double cos = MyMath::dot_product(_spinArray[position], _anisotropyDefects[position].direction);
		energy = _anisotropyDefects[position].energyParameter * (1 - cos * cos);
	}
	return energy;
}

double UniaxialAnisotropyEnergyDefect::interaction_energy_between_two_spins(int position1, int position2)
{
	return 0;
}

Threedim UniaxialAnisotropyEnergyDefect::effective_field(int position)
{
	Threedim field{0,0,0};
	if (_anisotropyDefects.count(position) > 0)
	{
		double factor = 2 * _anisotropyDefects[position].energyParameter*
			MyMath::dot_product(_anisotropyDefects[position].direction, _spinArray[position]);
		field = MyMath::mult(_anisotropyDefects[position].direction, factor);
	}
	return field;
}