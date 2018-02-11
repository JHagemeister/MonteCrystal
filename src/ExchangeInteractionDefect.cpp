/*
* ExchangeInteractionDefect.cpp
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

#include "ExchangeInteractionDefect.h"

// forward and further includes
#include "MyMath.h"

ExchangeInteractionDefect::ExchangeInteractionDefect(Threedim* spinArray, int* neighborArray, int nbors, 
	const std::unordered_map<int, double> &defects, std::string order) :
	Energy(0.5, "E_J" + order + "_Defect ", spinArray, 0), _nbors(nbors),
	_neighborArray(neighborArray)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] neighborArray Indexes of neighbor sites for interaction
	* @param[in] nbors Number of neighbors per lattice site
	* @param[in] defects Lattice site index, defect energy parameter
	*/
	setup_energy_bonds(defects);
}

ExchangeInteractionDefect::~ExchangeInteractionDefect()
{
}

double ExchangeInteractionDefect::single_energy(const int &position) const
{
	double energy = 0;
	if (_bondEnergies.count(position) != 0)
	{
		int neighbor = 0; // index of neighbor atom
		for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
		{
			if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
			{
				neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
				energy -= _bondEnergies.at(position)[i] * MyMath::dot_product(_spinArray[position], _spinArray[neighbor]);
			}
		}
	}
	return energy; // energy of single atom
}

Threedim ExchangeInteractionDefect::effective_field(const int &position) const
{
	Threedim field = { 0,0,0 };
	if (_bondEnergies.count(position) != 0)
	{
		int neighbor = 0; // index of neighbor atom
		for (int i = 0; i < _nbors; ++i)
		{
			if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
			{
				neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
				field = MyMath::add(field, MyMath::mult(_spinArray[neighbor], _bondEnergies.at(position)[i]));
			}
		}
	}
	return field; // energy of single atom
}

void ExchangeInteractionDefect::setup_energy_bonds(const std::unordered_map<int, double>& defects)
{
	/**
	* Setup exchange energy resolved for each relevant bond.
	*/

	// iterate over all defects to add the exchange energy parameters to each adjacent bond
	// note, that this does not register the exchange parameter correspondingly for the 
	// neighboring bond -> taken care of in the next step
	for (auto it = defects.begin(); it != defects.end(); ++it)
	{
		for (int i = 0; i < _nbors; ++i)
		{
			_bondEnergies[it->first].push_back(it->second);
		}
	}
	
	// iterate once more over all defects to register the exchange defect bonds at neighboring sites
	for (auto it = defects.begin(); it != defects.end(); ++it)
	{
		// for each defect site, iterate over all neighbors
		for (int i = 0; i < _nbors; ++i)
		{
			int neighbor = _neighborArray[it->first*_nbors + i]; // neighbor site
			// if neighbor site is not a defect site itself, add site for adding 
			// information about bonds
			if (_bondEnergies.count(neighbor) == 0)
			{
				for (int j = 0; j < _nbors; ++j)
				{
					// only energy for bond with defect site is unequal zero, set in next step
					_bondEnergies[neighbor].push_back(0);
				}
			}

			for (int j = 0; j < _nbors; ++j)
			{
				// check which bond corresponds to conntection with defect site
				if (_neighborArray[neighbor*_nbors + j] == it->first)
				{
					// register defect energy at neighbor site of defect
					_bondEnergies[neighbor][j] += it->second;
				}
			}
		}
	}
}
