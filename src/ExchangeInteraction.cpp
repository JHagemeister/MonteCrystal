/*
* ExchangeInteraction.cpp
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

#include "ExchangeInteraction.h"

// forward and further includes
#include "MyMath.h"

ExchangeInteraction::ExchangeInteraction(Threedim* spinArray, double energyParameter, int* neighborArray, 
	int nbors, std::string order):
	Energy(0.5, "E_J"+order+" ", spinArray, energyParameter), _nbors(nbors),
	_neighborArray(neighborArray)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] neighborArray Indexes of neighbor sites for interaction
	* @param[in] nbors Number of neighbors per lattice site
	*/
}

ExchangeInteraction::~ExchangeInteraction()
{
}

double ExchangeInteraction::single_energy(const int &position) const
{
	double energy = 0;
	int neighbor = 0; // index of neighbor atom
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
		{
			neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
			energy = energy + _spinArray[position].x * _spinArray[neighbor].x
				+ _spinArray[position].y * _spinArray[neighbor].y
				+ _spinArray[position].z * _spinArray[neighbor].z;
		}
	}
	energy = -_energyParameter * energy;
	return energy; // energy of single atom
}

Threedim ExchangeInteraction::effective_field(const int &position) const
{
	Threedim field = {0,0,0};
	int neighbor = 0; // index of neighbor atom
	for (int i = 0; i < _nbors; ++i)
	{
		if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
		{
			neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
			field = MyMath::add(field, _spinArray[neighbor]);
		}
	}
	return MyMath::mult(field,_energyParameter); // energy of single atom
}

int ExchangeInteraction::get_nbors(void) const
{
	return _nbors;
}

int* ExchangeInteraction::get_neighbor_array(void) const
{
	return _neighborArray;
}
