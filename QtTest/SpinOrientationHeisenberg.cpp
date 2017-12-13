/*
* SpinOrientationHeisenberg.cpp
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

#include "SpinOrientationHeisenberg.h"

#include "Lattice.h"
#include "RanGen.h"
#include "MyMath.h"

SpinOrientationHeisenberg::SpinOrientationHeisenberg(int numberAtoms, std::shared_ptr<RanGen> ranGen) :
	SpinOrientation(numberAtoms, ranGen)
{
	/**
	* Constructor.
	*
	* @param[in] number of atoms
	* @param[in] random number generator
	*/

	delete[] _spinArray;
	_spinArray = new Threedim[_numberAtoms];
	random_orientation();
}

SpinOrientationHeisenberg::~SpinOrientationHeisenberg()
{

}

void SpinOrientationHeisenberg::random_orientation(void)
{
	/*
	* This method sets a random initial spin configuration and sets all spins active for update during 
	* simulation
	*/

	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArray[i] = generate_random_spin();
	}
	set_all_sites_active();
}

void SpinOrientationHeisenberg::set_spin_spiral(Threedim spiralR, Threedim spiralI, Threedim ek, 
	double lambda, Lattice* lattice)
{
	spiralR = MyMath::normalize(spiralR);
	spiralI = MyMath::normalize(spiralI);
	ek = MyMath::normalize(ek);
	ek = MyMath::mult(ek, 2 * Pi / lambda);
	Threedim* latticeCoordArrayPntr = lattice->get_lattice_coordinate_array();
	delete[] _spinArray;
	_spinArray = new Threedim[_numberAtoms];
	Threedim* spinArrayPntr = _spinArray;
	Threedim spin;
	double arg = 0;
	for (int i = 0; i < _numberAtoms; ++i, ++spinArrayPntr)
	{
		arg = MyMath::dot_product(ek, latticeCoordArrayPntr[i]);
		spin.x = spiralR.x * cos(arg) - spiralI.x * sin(arg);
		spin.y = spiralR.y * cos(arg) - spiralI.y * sin(arg);
		spin.z = spiralR.z * cos(arg) - spiralI.z * sin(arg);
		spinArrayPntr->x = spin.x;
		spinArrayPntr->y = spin.y;
		spinArrayPntr->z = spin.z;
	}
}

void SpinOrientationHeisenberg::set_spin_spiral(Threedim kVector, Threedim position, int helicity, 
	Lattice* lattice)
{
	double angle = 0;
	double dotProd = 0;
	double normK = MyMath::norm(kVector);
	Threedim ek = MyMath::normalize(kVector);
	Threedim helpVec = { 0, 0, 0 };
	Threedim helpVec1 = { 0, 0 ,0 };
	Threedim ez = { 0, 0, 1 };
	Threedim* latticeCoordArray = lattice->get_lattice_coordinate_array();
	for (int i = 0; i < _numberAtoms; i++)
	{
		helpVec = MyMath::difference(latticeCoordArray[i], position);
		dotProd = MyMath::dot_product(ek, helpVec);
		angle = normK * dotProd * helicity;
		helpVec = MyMath::mult(ez, cos(angle));
		helpVec1 = MyMath::mult(ek, sin(angle));
		helpVec = MyMath::add(helpVec, helpVec1);
		_spinArray[i] =helpVec;
	}
}

void SpinOrientationHeisenberg::set_ferromagnet(Threedim direction)
{
	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArray[i] = direction;
	}
}

void SpinOrientationHeisenberg::single_orientation(int position)
{
	/**
	* This method sets a new spin at lattice site given by position. The new spin has a random direction. 
	* The old spin is saved in the member variable spin and position.
	*
	* @param[in] position Index of spin site.
	*/
	
	// save position and spin in cache to allow for a possible restoration of old configuration later on
	_position = position;
	_spin = _spinArray[position];

	// set new random spin direction
	_spinArray[position] = generate_random_spin();

}

void SpinOrientationHeisenberg::restore_single_orientation(void)
{
	/**
	* This method restores the latest spin reorientation. At lattice site given by member variable position, 
	* the spin will be changed to the spin given by member variable spin.
	*/

	_spinArray[_position] = _spin;
}

Threedim SpinOrientationHeisenberg::magnetisation(void)
{
	/**
	* This method gives the total magnetisation.
	*
	* @return The magnetisation in cartesian coordinates.
	*/

	Threedim m = { 0, 0, 0 };
	for (int i = 0; i < _numberAtoms; i++)
	{
		m = MyMath::add(m, _spinArray[i]);
	}
	return m;
}

Threedim SpinOrientationHeisenberg::generate_random_spin(void)
{
	/**
	* This function generates a spin with a random direction. The
	* spin vector has length one.
	*
	* @return Normalised spin vector with random direction
	*/

	int found = 0;
	Threedim tmpSpin = { 0,0,0 };
	while (found == 0)
	{
		tmpSpin.x = (2.0 * _ranGen->Random() - 1);
		tmpSpin.y = (2.0 * _ranGen->Random() - 1);
		tmpSpin.z = (2.0 * _ranGen->Random() - 1);

		double norm = MyMath::norm(tmpSpin);
		if (norm < 1)
		{
			// this check is needed to avoid accumulations in corners of cube
			found = 1;
			tmpSpin.x = tmpSpin.x / norm;
			tmpSpin.y = tmpSpin.y / norm;
			tmpSpin.z = tmpSpin.z / norm;
		}
	}
	return tmpSpin;
}

