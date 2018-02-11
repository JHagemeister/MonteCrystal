/*
* DipolarInteraction.cpp
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

#include "DipolarInteraction.h"

// forward and further includes
#include "Lattice.h"
#include "MyMath.h"
#include <omp.h>

DipolarInteraction::DipolarInteraction(Threedim* spinArray, double magneticMoment, double latticeConstant, 
	Lattice* lattice):
	Energy(0.5, "E_Dipol ", spinArray, 1)
{
	/**
	* @param[in] spinArray Pointer to spin configuration.
	* @param[in] magneticMoment Magnetic moment of single spin as multiples of Bohr magneton
	* @param[in] latticeConstant Lattice constant of the lattice in [Angstrom]
	* @param[in] lattice Lattice object containing all information about lattice configuration
	*/

	// array containing all distances in the form 1/distance**3.
	_distanceArray = NULL;
	// array containing vectors between all atoms, also calculated at object creation
	_distanceVectors = NULL;
	// contain for each spin all indexes of all other spins
	_indexArray = NULL;
	_numberAtoms = lattice->get_number_atoms();
	// calculate prefactor of Dipol energy
	_prefactor = 9.27401 * 9.27401 * pow(magneticMoment,2) / (1000 * 1.602 * pow(latticeConstant, 3));
	// setup all parameters as promised above
	setup_distance_array(lattice);
}

DipolarInteraction::~DipolarInteraction()
{
	for (int i = 0; i < _numberAtoms; ++i)
	{
		delete _distanceArray[i];
		_distanceArray[i] = NULL;
		delete _indexArray[i];
		_indexArray[i] = NULL;
		delete _distanceVectors[i];
		_distanceVectors[i] = NULL;
	}
	delete[] _distanceArray;
	_distanceArray = NULL;
	delete[] _indexArray;
	_indexArray = NULL;
	delete[] _distanceVectors;
	_distanceVectors = NULL;
}

double DipolarInteraction::single_energy(const int &position) const
{
	double energy = 0;
	omp_set_num_threads(2);
#pragma omp parallel for reduction(+:energy)
	for (int i = 0; i < _numberAtoms - 1; ++i)
	{
		energy += (MyMath::dot_product(_spinArray[position], _spinArray[_indexArray[position][i]])
			- 3 * MyMath::dot_product(_spinArray[position], _distanceVectors[position][i])
			* MyMath::dot_product(_spinArray[_indexArray[position][i]], _distanceVectors[position][i]))
			*_distanceArray[position][i];
	}
	return _prefactor * energy;
}


Threedim DipolarInteraction::effective_field(const int &position) const
{
	Threedim field = { 0,0,0 };
	Threedim tmpField = {0,0,0};
	double value = 0;
	for (int i = 0; i < _numberAtoms - 1; i++)
	{
		value = -3 * MyMath::dot_product(_spinArray[_indexArray[position][i]], _distanceVectors[position][i]);
		tmpField = MyMath::mult(_distanceVectors[position][i], value);
		tmpField = MyMath::add(tmpField, _spinArray[_indexArray[position][i]]);
		tmpField = MyMath::mult(tmpField, _distanceArray[position][i]);
		field = MyMath::add(field, tmpField);
	}
	field = MyMath::mult(field, -1*_prefactor);
	return field;
}

void DipolarInteraction::setup_distance_array(Lattice* lattice)
{
	_distanceArray = new double*[_numberAtoms];
	_indexArray = new int*[_numberAtoms];
	_distanceVectors = new Threedim*[_numberAtoms];
	
	double* doublePntr = NULL;
	int* intPntr = NULL;
	Threedim* ThreedimPntr = NULL;
	Threedim diffVector = { 0,0,0 };

	Threedim* latticeCoordArray = lattice->get_lattice_coordinate_array();
	int count = 0;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		_distanceArray[i] = new double[_numberAtoms - 1];
		_indexArray[i] = new int[_numberAtoms - 1];
		_distanceVectors[i] = new Threedim[_numberAtoms - 1];

		doublePntr = _distanceArray[i];
		intPntr = _indexArray[i];
		ThreedimPntr = _distanceVectors[i];

		count = 0;
		for (int j = 0; j < _numberAtoms; ++j)
		{
			if (i != j)
			{
				intPntr[count] = j;
				diffVector = MyMath::difference(latticeCoordArray[i], latticeCoordArray[j]);
				doublePntr[count] = 1. / pow(MyMath::norm(diffVector), 3);
				ThreedimPntr[count] = MyMath::normalize(diffVector);
				count += 1;
			}
		}
	}
}
