/*
* Mersenne.cpp
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
*/

#include "Mersenne.h"

#include <algorithm>

Mersenne::Mersenne(int seed): _mt(seed)
{
}

Mersenne::~Mersenne()
{
}

int Mersenne::IRandom(int min, int max)
{
	/**
	* Pseudo random number uniformly drawn from inerval [min,max]
	*/
	std::uniform_int_distribution<> dist(min,max);
	return dist(_mt);
}

double Mersenne::Random(void)
{
	/**
	* Pseudo random number uniformly drawn from inerval [0,1)
	*/
	std::uniform_real_distribution<double> dist(0, 1);
	return dist(_mt);
}

void Mersenne::Shuffle(std::vector<int>& vector)
{
	/**
	* Randomize positions of integer values in a vector of integer values.
	*
	* @param[in] vector Integer values
	*/
	std::shuffle(vector.begin(), vector.end(), _mt);
}
