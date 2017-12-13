/*
* AbsoluteMagnetisationObservable.cpp
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

#include "AbsoluteMagnetisationObservable.h"

//forward includes
#include "SpinOrientation.h"

#include <sstream> 
#include "MyMath.h"

AbsoluteMagnetisationObservable::AbsoluteMagnetisationObservable(int numberMeasurements, 
								 SpinOrientation* spinOrientation):
	Observable(numberMeasurements)
{
	/**
	* @param[in] numberMeasurements Number of measurement values that can be stored
	* @param[in] spinOrientation Information about spin  system	
	*/

	_spinOrientation = spinOrientation;
	_absoluteMagnetisation = new Threedim[numberMeasurements]; // for storage of measurement values
	_numberAtoms = spinOrientation->get_number_atoms();
}

AbsoluteMagnetisationObservable::~AbsoluteMagnetisationObservable()
{
	delete[] _absoluteMagnetisation;
}

std::string AbsoluteMagnetisationObservable::get_steps_header(void)
{
	return ""; // no output during Monte-Carlo-Steps.
}

std::string AbsoluteMagnetisationObservable::get_mean_header(void)
{
	std::string header = "<|SiX|> <|SiY|> <|SiZ|> ";
	return header;
}

void AbsoluteMagnetisationObservable::take_value(void)
{
	Threedim* spinArray = _spinOrientation->get_spin_array();
	Threedim tmpValues = { 0,0,0 };
	for (int i = 0; i < _numberAtoms; ++i) // average measurement values
	{
		tmpValues.x += fabs(spinArray[i].x);
		tmpValues.y += fabs(spinArray[i].y);
		tmpValues.z += fabs(spinArray[i].z);
	}
	_absoluteMagnetisation[_measurementIndex] = tmpValues;
	++_measurementIndex;
}

std::string AbsoluteMagnetisationObservable::get_step_value(int index)
{
	/**
	* @return empty std::string
	*/
	return ""; // no output during Monte-Carlo-Steps
}

std::string AbsoluteMagnetisationObservable::get_mean_value(double temperature)
{
	std::stringstream stream;
	if (_measurementIndex == _numberMeasurements)
	{

		Threedim meanValues = {0,0,0};
		for (int i = 0; i < _numberMeasurements; ++i) // average measurement values
		{
			meanValues = MyMath::add(meanValues,_absoluteMagnetisation[i]);
		}
		meanValues = MyMath::mult(meanValues, 1 / (double)_numberMeasurements);
		meanValues = MyMath::mult(meanValues, 1/(double)_numberAtoms);
		stream << meanValues.x << " " << meanValues.y << " " << meanValues.z << " ";
	}
	else
	{
		std::cout << "Error in AbsoluteMagnetisationObservable::mean_value" << std::endl;
	}
	return stream.str();
}

void AbsoluteMagnetisationObservable::clear_storage()
{
	delete[] _absoluteMagnetisation;
	_absoluteMagnetisation = new Threedim[_numberMeasurements];
}
