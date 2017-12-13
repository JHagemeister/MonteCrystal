/*
* Energy.cpp
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

#include "Energy.h"

Energy::Energy(double factor, std::string stringID, Threedim* spinArray, double energyParameter)
{
	/**
	* @param[in] factor Correction factor for double summations
	* @param[in] stringID Name of the energy
	* @param[in] spinArray Spin configuration
	* @param[in] energyParameter Energy parameter [meV]
	*/

	_factor = factor;
	_stringID = stringID;
	_energyParameter = energyParameter;
	_spinArray = spinArray;
}

Energy::~Energy() 
{

}

double Energy::get_factor(void)  const
{
	return _factor;
}

std::string Energy::get_string_id(void) const
{
	return _stringID;
}

double Energy::get_energy_parameter(void) const
{
	return _energyParameter;
}

void Energy::set_energy_parameter(double energyParameter)
{
	/**
	* @param[in] energyParameter Energy parameter [meV]
	*/
	_energyParameter = energyParameter;
}

void Energy::set_spin_array(Threedim* spinArray)
{
	/**
	* @param[in] spinArray Spin configuration
	*/
	_spinArray = spinArray;
}

