/*
* SpinOrientationHeisenbergRestrictedCone.cpp
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

#include "SpinOrientationHeisenbergRestrictedCone.h"

#include "Lattice.h"
#include "RanGen.h"
#include "MyMath.h"

SpinOrientationHeisenbergRestrictedCone::SpinOrientationHeisenbergRestrictedCone(int numberAtoms,
	std::shared_ptr<RanGen> ranGen, double sigma) :
	SpinOrientationHeisenberg(numberAtoms, ranGen)
{
	_sigma = sigma;
}

SpinOrientationHeisenbergRestrictedCone::~SpinOrientationHeisenbergRestrictedCone()
{
	
}

void SpinOrientationHeisenbergRestrictedCone::single_orientation(int position)
{
	/**
	* The spin is updated within a cone around its old direction
	*/

	_spin = _spinArray[position];
	_position = position;

	make_random_change_within_cone(position);
}

void SpinOrientationHeisenbergRestrictedCone::make_random_change_within_cone(int position)
{
	/*
	* Look at Boris Wolter PhD thesis for further information.
	*/

	double value1 = 0;
	double value2 = 0;
	_ranGen->polar(value1, value2);
	_spinArray[position].x += value1;
	_spinArray[position].y += value2;
	_ranGen->polar(value1, value2);
	_spinArray[position].z += value1;

	_spinArray[position] = MyMath::normalize(_spinArray[position]);
}
