/*
* Observable.cpp
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

#include "Observable.h"

//additional includes 
#include "MyMath.h"

Observable::Observable(int numberMeasurements)
{
	/**
	* @param[in] numberMeasurements Number of measurements before storage is full.
	*/

	_numberMeasurements = numberMeasurements;
	_measurementIndex = 0;
}

Observable::~Observable()
{
}

void Observable::set_measurement_index(int measurementIndex)
{
	_measurementIndex = measurementIndex;
}

void Observable::set_number_measurements(int numberMeasurements)
{
	_numberMeasurements = numberMeasurements;
	clear_storage();
	set_measurement_index(0);
}

int Observable::get_measurement_index(void)
{
	return _measurementIndex;
}

int Observable::get_num_measurements(void)
{
	return _numberMeasurements;
}