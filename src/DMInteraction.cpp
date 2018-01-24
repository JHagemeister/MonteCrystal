/*
* DMInteraction.cpp
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

#include "DMInteraction.h"

// forward includes
#include "Lattice.h"

// forward and further includes
#include "MyMath.h"

#include <omp.h>

DMInteraction::DMInteraction(Threedim* spinArray, double energyParameter, Threedim direction, DMType dmType,
	Lattice* lattice, int order):
	Energy(0.5, "E_D"+std::to_string(order)+" ", spinArray, energyParameter)
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
	
	_DMVectors = NULL;
	set_DM_vectors(lattice);
}

DMInteraction::~DMInteraction()
{
	delete[] _DMVectors;
	_DMVectors = NULL;
}

double DMInteraction::single_energy(int position)
{
	Threedim spinProd = {0,0,0};
	double energy = 0;
	int neighbor = 0; // index of neighbor atom
	int index = 0;
	for (int i = 0; i < _nbors; ++i)
	{
		index = _nbors * position;
		if (_neighborArray[index + i] != -1) // -1 refers to empty entry
		{
			neighbor = _neighborArray[index + i]; // index of neighbor atom
			spinProd = MyMath::vector_product(_spinArray[position], _spinArray[neighbor]);
			energy = energy + MyMath::dot_product(_DMVectors[index + i], spinProd);
		}
	}
	energy = -_energyParameter * energy;
	return energy; // energy of single atom

}

double DMInteraction::interaction_energy_between_two_spins(int position1, int position2) 
{
	return 0;
}

Threedim DMInteraction::effective_field(int position)
{
	Threedim field = { 0,0,0 };
	Threedim tmpVec = { 0,0,0 };
	int neighbor = 0; // index of neighbor atom
	int index = 0;
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		index = _nbors * position;
		if (_neighborArray[index + i] != -1) // -1 refers to empty entry
		{
			neighbor = _neighborArray[index + i]; // index of neighbor atom
			tmpVec = MyMath::vector_product(_DMVectors[index + i], _spinArray[neighbor]);
			field = MyMath::add(field, tmpVec);
		}
	}
	return MyMath::mult(field,-1*_energyParameter);
}

int DMInteraction::get_nbors(void)
{
	return _nbors;
}

int * DMInteraction::get_neighbor_array(void)
{
	return _neighborArray;
}

Threedim * DMInteraction::get_dm_vectors(void)
{
	return _DMVectors;
}

void DMInteraction::set_DM_vectors(Lattice* lattice)
{
	/**
	* Calculates DM vectors as cross products of _direction and nearest neighbor vectors 
	*/

	_DMVectors = new Threedim[lattice->get_number_atoms() * _nbors];
	Threedim* neighborVectorArray = lattice->get_neighbor_vector_array(_order);
	if (neighborVectorArray == NULL)
	{
		std::cout << "Error in DMInteraction::setDMVectors: neighborVectorArray"
			" is NULL for order " << _order << " Program exit." << std::endl;
		exit(0);
	}

	Threedim diff;
	int neighbor = 0;
	int currentatom = 0;
	int count = 0;
	double precision = 0.01;
	for (int i = 0; i < lattice->get_number_atoms() * _nbors; ++i)
	{
		if (count == _nbors)
		{
			currentatom += 1;
			count = 0;
		}
		count += 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			diff = neighborVectorArray[i];
			if (MyMath::norm(diff) > PRECISION)
			{
				if (_dmType == Neel)
				{
					_DMVectors[i] = MyMath::vector_product(_direction, diff);
					_DMVectors[i] = MyMath::normalize(_DMVectors[i]);
				}
				else if (_dmType == Chiral)
				{
					_DMVectors[i] = MyMath::normalize(diff);
				}
			}
			else
			{
				_DMVectors[i] = { 0, 0, 0 };
			}
		}
	}
}
