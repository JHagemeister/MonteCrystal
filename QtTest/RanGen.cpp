/*
* RanGen.cpp
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

#include "RanGen.h"

RanGen::RanGen() {
}

RanGen::~RanGen() {
}

void RanGen::polar(double &x1, double &x2)
{
	/**
	* @param[out] x1 normal distributed number by reference
	* @param[out] x2 normal distributed number by reference
	*/

	double u = 0;
	double v = 0;
	double q = -1;
	double p = 0;

	while (q <= 0.0 || q >= 1.0)
	{
		u = 2.0 * (( (double) rand() ) / RAND_MAX) - 1;
		v = 2.0 * (((double)rand()) / RAND_MAX) - 1;
		q = u * u + v * v;
	};

	p = sqrt(-2 * log(q) / q);
	x1 = u * p;
	x2 = v * p;
}