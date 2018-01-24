/*
* Tip.cpp
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

#include "Tip.h"

// forward and further includes
#include "Lattice.h"
#include "MyMath.h"
#include <omp.h>

Tip::Tip(Threedim* spinArray, double energyParameter, Threedim tipPosition, Threedim tipDirection, 
	Lattice* lattice):
	Energy(1, "E_Tip ", spinArray, energyParameter)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] tipPosition Position of tip
	* @param[in] tipDirection Magnetization direction of tip; is normalized in constructor
	* @param[in] lattice Lattice information
	*/

	_tipPosition = tipPosition;
	_tipDirection = MyMath::normalize(tipDirection);
	_numberAtoms = lattice->get_number_atoms();
	_latticeCoordArray = lattice->get_lattice_coordinate_array();
	_k = 3.0;
	_distanceArray = NULL;
	set_distance_array();
}

Tip::~Tip()
{
	delete[] _distanceArray;
	_distanceArray = NULL;
}

double Tip::single_energy(int position)
{
	double energy = 0;
	energy = -MyMath::dot_product(_tipDirection, _spinArray[position]);
	return energy * _distanceArray[position];
}

double Tip::interaction_energy_between_two_spins(int position1, int position2)
{
	return 0;
}

Threedim Tip::effective_field(int position)
{
	return MyMath::mult(_tipDirection, _distanceArray[position]);
}

void Tip::set_position(Threedim position)
{
	_tipPosition = position;
	set_distance_array();
}

void Tip::set_direction(Threedim tipDirection)
{
	_tipDirection = MyMath::normalize(tipDirection);
}

void Tip::set_distance_array(void)
{
	delete[] _distanceArray;
	_distanceArray = NULL;
	_distanceArray = new double[_numberAtoms];
	double d = 0;
	omp_set_num_threads(4);
#pragma omp parallel for
	for (int i = 0; i < _numberAtoms; ++i)
	{
		d = MyMath::norm(MyMath::difference(_tipPosition, _latticeCoordArray[i]));
		_distanceArray[i] = exp(-2 * _k * d) * _energyParameter;
	}
}
