/*
* BiquadraticInteraction.cpp
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

#include "BiquadraticInteraction.h"

// forward and further includes
#include "MyMath.h"

BiquadraticInteraction::BiquadraticInteraction(Threedim* spinArray, double energyParameter,
	int* neighborArray, int nbors):
	Energy(0.5, "E_BiQ ", spinArray, energyParameter), _neighborArray(neighborArray), _nbors(nbors)
{
	/**
	* @param[in] spinArray Spin configuration
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] neighborArray Indexes of neighbor sites
	* @param[in] nbors Number of neighbors per lattice site
	*/
}

BiquadraticInteraction::~BiquadraticInteraction()
{
}

double BiquadraticInteraction::single_energy(const int &position) const
{
	double energy = 0;
	int neighbor = 0; // index of neighbor atom
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position + i] != -1)
		{
			neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
			energy += pow(MyMath::dot_product(_spinArray[position], _spinArray[neighbor]), 2);
		}
	}
	energy = -_energyParameter * energy;
	return energy; // energy of single atom
}

Threedim BiquadraticInteraction::effective_field(const int &position) const
{
	Threedim field = { 0,0,0 };
	int neighbor = 0; // index of neighbor atom
	double factor = 0;
	Threedim tempVec = { 0,0,0 };
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position + i] != -1)
		{
			neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
			factor = MyMath::dot_product(_spinArray[position],_spinArray[neighbor]);
			tempVec = MyMath::mult(_spinArray[neighbor], factor);
			field = MyMath::add(field, tempVec);
		}
	}
	return MyMath::mult(field,2*_energyParameter); // energy of single atom
}