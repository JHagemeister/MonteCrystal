/*
* SpinOrientationIsing.cpp
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

#include "SpinOrientationIsing.h"

#include "RanGen.h"
#include "MyMath.h"

SpinOrientationIsing::SpinOrientationIsing(int numberAtoms, std::shared_ptr<RanGen> ranGen) :
	SpinOrientation(numberAtoms, ranGen)
{
	/**
	* Constructor.
	*
	* @param[in] number of atoms
	* @param[in] random number generator
	*/
}

SpinOrientationIsing::~SpinOrientationIsing()
{
}

void SpinOrientationIsing::random_orientation(void)
{
	/**
	* This function creates an initial spin configuration and sets all sites active for update during
	* simulation.
	*/

	delete[] _spinArray;
	_spinArray = new Threedim[_numberAtoms];
	
	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArray[i].x = generate_random_spin();
		_spinArray[i].y = 0;
		_spinArray[i].z = 0;
	}
	set_all_sites_active();
}

void SpinOrientationIsing::set_spin_spiral(Threedim spiralR, Threedim spiralI, Threedim ek, double lambda, 
	Lattice* lattice)
{
	delete[] _spinArray;
	_spinArray = new Threedim[_numberAtoms];
	Threedim* spinArrayPntr = _spinArray;
	for (int i = 0; i < _numberAtoms; ++i, ++spinArrayPntr)
	{
		spinArrayPntr->x = 1;
		spinArrayPntr->y = 0;
		spinArrayPntr->z = 0;
	}
}

void  SpinOrientationIsing::set_spin_spiral(Threedim kVector, Threedim position, int helicity,
	Lattice* lattice)
{
	random_orientation();
}

void SpinOrientationIsing::set_ferromagnet(Threedim direction)
{
	int signum = MyMath::signum(direction.x);
	
	if (signum == 0)
	{
		// the spin length should be equal to 1!
		signum = 1;
	}
	for (int i = 0; i < _numberAtoms; i++)
	{	
		_spinArray[i].x = signum;
	}
}

void SpinOrientationIsing::single_orientation(int position)
{
	/**
	* This function flips the spin at a given position.
	*
	* @param[in] _position A lattice site.
	*/

	_spinArray[position].x = -_spinArray[position].x;
	_position = position;
}

void SpinOrientationIsing::restore_single_orientation(void)
{
	/**
	* This function undoes the spin flip done by
	* 'SingleOrientation(_position)'.
	*/
	_spinArray[_position].x = -_spinArray[_position].x;
}

Threedim SpinOrientationIsing::magnetisation(void) const
{
	/**
	* This function returns the total energy.
	*
	* @return The total magnetisation in cartesian coordinates.
	*/
	Threedim m;
	m.x = m.y = m.z = 0;
	Threedim* spinArrayPntr = _spinArray;
	for (int i = 0; i < _numberAtoms; ++i, ++spinArrayPntr)
	{
		m.x += spinArrayPntr->x; // only x-components
	}
	return m;
}

int SpinOrientationIsing::generate_random_spin(void)
{
	/**
	* This function returns a random spin direction.
	*
	* @return A random spin direction (either +1 or -1).
	*/
	if (_ranGen->Random() > 0.5)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
