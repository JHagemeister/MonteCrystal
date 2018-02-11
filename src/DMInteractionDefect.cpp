/*
* DMInteractionDefect.cpp
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

#include "DMInteractionDefect.h"

// forward includes
#include "Lattice.h"

// forward and further includes
#include "MyMath.h"

#include <omp.h>

DMInteractionDefect::DMInteractionDefect(Threedim* spinArray, Threedim direction,
	const std::unordered_map<int, double> &defects, DMType dmType,
	Lattice* lattice, int order) :
	Energy(0.5, "E_D" + std::to_string(order) + "_Defect ", spinArray, 0)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] direction DM vectors are cross products of direction and nearest neighbor vectors
	* @param[in] chiral 0 for Neel type 1 for chiral interaction
	* @param[in] lattice Lattice information needed to calculate DM vectors
	*/

	_direction = MyMath::normalize(direction);
	_dmType = dmType;
	_order = order;

	_nbors = lattice->get_number_nth_neighbors(order);
	_neighborArray = lattice->get_neighbor_array(order);

	set_DM_vectors(defects,lattice);
}

DMInteractionDefect::~DMInteractionDefect()
{
}

double DMInteractionDefect::single_energy(const int &position) const
{
	double energy = 0;
	if (_dmVectors.count(position) != 0)
	{
		Threedim spinProd = { 0,0,0 };
		int neighbor = 0; // index of neighbor atom
		int index = 0;
		for (int i = 0; i < _nbors; ++i)
		{
			index = _nbors * position;
			if (_neighborArray[index + i] != -1) // -1 refers to empty entry
			{
				neighbor = _neighborArray[index + i]; // index of neighbor atom
				spinProd = MyMath::vector_product(_spinArray[position], _spinArray[neighbor]);
				energy -= MyMath::dot_product(_dmVectors.at(position)[i], spinProd);
			}
		}
	}
	return energy; // energy of single atom

}

Threedim DMInteractionDefect::effective_field(const int &position) const
{
	Threedim field = { 0,0,0 };
	if (_dmVectors.count(position) != 0)
	{
		Threedim tmpVec = { 0,0,0 };
		int neighbor = 0; // index of neighbor atom
		int index = 0;
		for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
		{
			index = _nbors * position;
			if (_neighborArray[index + i] != -1) // -1 refers to empty entry
			{
				neighbor = _neighborArray[index + i]; // index of neighbor atom
				tmpVec = MyMath::vector_product(_dmVectors.at(position)[i], _spinArray[neighbor]);
				field = MyMath::add(field, tmpVec);
			}
		}
	}
	return MyMath::mult(field, -1);
}

void DMInteractionDefect::set_DM_vectors(const std::unordered_map<int, double> &defects, Lattice* lattice)
{
	/**
	* Calculates DM vectors as cross products of _direction and nearest neighbor vectors
	*
	* For details see comments in ExchangeInteractionDefect::setup_energy_bonds()
	*/

	Threedim* neighborVectorArray = lattice->get_neighbor_vector_array(_order);
	if (neighborVectorArray == NULL)
	{
		std::cout << "Error in DMInteraction::setDMVectors: neighborVectorArray"
			" is NULL for order " << _order << " Program exit." << std::endl;
		exit(0);
	}

	int currentatom = 0;
	Threedim dmVector{0,0,0};
	int neighbor = 0;
	for (auto it = defects.begin(); it != defects.end(); ++it)
	{
		currentatom = it->first;
		for (int i = 0; i < _nbors; ++i)
		{
			neighbor = _neighborArray[currentatom*_nbors+i];
			if (neighbor != -1)
			{
				dmVector = neighborVectorArray[currentatom*_nbors + i];
				if (MyMath::norm(dmVector) > PRECISION)
				{
					if (_dmType == Neel)
					{
						dmVector = MyMath::vector_product(_direction, dmVector);
						dmVector = MyMath::normalize(dmVector);
					}
					else if (_dmType == Chiral)
					{
						dmVector = MyMath::normalize(dmVector);
					}
				}
				else
				{
					dmVector = { 0, 0, 0 };
				}
			}
			else
			{
				dmVector = { 0,0,0 };
			}
			_dmVectors[currentatom].push_back(MyMath::mult(dmVector, it->second));
		}
	}

	for (auto it = defects.begin(); it != defects.end(); ++it)
	{
		for (int i = 0; i < _nbors; ++i)
		{
			neighbor = _neighborArray[it->first*_nbors + i];
			if (_dmVectors.count(neighbor) == 0)
			{
				for (int j = 0; j < _nbors; ++j)
				{
					_dmVectors[neighbor].push_back({ 0, 0, 0});
				}
			}

			for (int j = 0; j < _nbors; ++j)
			{
				if (_neighborArray[neighbor*_nbors + j] == it->first)
				{
					dmVector = MyMath::mult(_dmVectors[it->first][i],-1);
					_dmVectors[neighbor][j] = MyMath::add(_dmVectors[neighbor][j],dmVector);
				}
			}
		}
	}
}
