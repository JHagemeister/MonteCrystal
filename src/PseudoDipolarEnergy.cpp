/*
* PseudoDipolarEnergy.cpp
*
* Copyright 2020 Julian Hagemeister
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

#include "PseudoDipolarEnergy.h"

// forward and further includes
#include "MyMath.h"

PseudoDipolarEnergy::PseudoDipolarEnergy(Threedim* spinArray, double energyParameter, int* neighborArray, 
	int nbors, Threedim* neighborVectorArray):
	Energy(0.5, "E_PD ", spinArray, energyParameter/MyMath::dot_product(*neighborVectorArray, *neighborVectorArray)), _nbors(nbors),
	_neighborArray(neighborArray),
	_neighborVectorArray(neighborVectorArray)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] neighborArray Indexes of neighbor sites for interaction
	* @param[in] nbors Number of neighbors per lattice site
	*/
}

PseudoDipolarEnergy::~PseudoDipolarEnergy()
{
}

double PseudoDipolarEnergy::single_energy(const int &position) const
{
	double energy = 0;
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
		{
			energy = energy +
			MyMath::dot_product(_spinArray[position], _neighborVectorArray[_nbors * position + i]) *
			MyMath::dot_product(_spinArray[_neighborArray[_nbors * position + i]], _neighborVectorArray[_nbors * position + i]);
		}
	}
	energy = _energyParameter * energy;
	return energy; // energy of single atom
}

Threedim PseudoDipolarEnergy::effective_field(const int &position) const
{
	Threedim field = {0,0,0};
	int neighbor = 0; // index of neighbor atom
	for (int i = 0; i < _nbors; ++i)
	{
		if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
		{
			field = MyMath::add(field, MyMath::mult(_neighborVectorArray[_nbors * position + i],
			MyMath::dot_product(_spinArray[_neighborArray[_nbors * position + i]], _neighborVectorArray[_nbors * position + i])));
		}
	}
	return MyMath::mult(field, -_energyParameter); // energy of single atom
}
