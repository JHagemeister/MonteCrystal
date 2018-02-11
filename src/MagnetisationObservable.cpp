/*
* MagnetisationObservable.cpp
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

#include "MagnetisationObservable.h"

//forward and further includes
#include "SpinOrientation.h"
#include "MyMath.h"
#include <iomanip>
#include <iostream>

MagnetisationObservable::MagnetisationObservable(int numberMeasurements, SpinOrientation* spinOrientation, 
	int boolEachSpin) :
	Observable(numberMeasurements)
{
	/**
	* @param[in] numberMeasurements Number of measurements.
	* @param[in] spinOrientation Information about spin configuration.
	* @param[in] boolEachSpin 1 to obtain spin resolved information, 0 to suppress spin resolved information
	*/

	_spinOrientation = spinOrientation;
	_boolEachSpin = boolEachSpin;
	_numberAtoms = spinOrientation->get_number_atoms();
	_valuesSpinResolved = NULL;
	_values = NULL;
	clear_storage();
}

MagnetisationObservable::~MagnetisationObservable()
{
	delete[] _values;
	delete[] _valuesSpinResolved;
}

std::string MagnetisationObservable::get_steps_header(void) const
{
	/**
	* This function returns the header for the output file.
	*
	* @return std::string "MX MY MZ ".
	*/
	std::string tmpStr = "";
	std::string header = "Mx My Mz ";
	if (_boolEachSpin == TRUE)
	{
		for (int i = 0; i < _numberAtoms; i++)
		{
			std::ostringstream convert; // stream used for the conversion
			convert << i;
			tmpStr.clear();
			tmpStr.append("Spin_");
			tmpStr.append(convert.str());
			tmpStr.append("_x ");
			tmpStr.append("Spin_");
			tmpStr.append(convert.str());
			tmpStr.append("_y ");
			tmpStr.append("Spin_");
			tmpStr.append(convert.str());
			tmpStr.append("_z ");
			header.append(tmpStr);
		}
	}
	return header;
}

std::string MagnetisationObservable::get_mean_header(void) const
{
	/**
	* This function returns the header for the output file.
	*
	* @return std::string
	*/
	std::string tmpStr = "";
	std::string header = "";
	
	header.append(get_steps_header());
	header.append("SUS ");

	return header;
}

void MagnetisationObservable::take_value(void)
{
	/**
	* Make a measurement on the system.
	*/
	_values[_measurementIndex] = _spinOrientation->magnetisation();
	if (_boolEachSpin == TRUE)
	{
		Threedim* tmpPntr = _valuesSpinResolved + _measurementIndex * _numberAtoms;
		for (int i = 0; i < _numberAtoms; i++, tmpPntr++)
		{
			*tmpPntr = _spinOrientation->get_spin(i);
		}
	}
	++_measurementIndex;
}

std::string MagnetisationObservable::get_step_value(const int &index) const
{
	/**
	* Get a measurement result that has been taken.
	*
	* @param[in] _i Index of a measurement result.
	*
	* @return The measurement result as a std::string.
	*/
	std::stringstream stream;
	stream << std::setprecision(15);
	if (index < _numberMeasurements)
	{
		stream << _values[index].x / _numberAtoms << " "  << _values[index].y / _numberAtoms << " "
		 << _values[index].z / _numberAtoms;
		if (_boolEachSpin == TRUE)
		{
			Threedim* tmpPntr = _valuesSpinResolved + index * _numberAtoms;
			for (int i = 0; i < _numberAtoms; i++, tmpPntr++)
			{
				stream << " " << (*tmpPntr).x << " " << (*tmpPntr).y << " " << (*tmpPntr).z;
			}
		}
	}
	else
	{
		std::cout << "Index out of bounds in MagnetisationObservable::get_value(int index)." << std::endl;
	}
	return stream.str();
}

std::string MagnetisationObservable::get_mean_value(const double &temperature) const
{
	/**
	* Get the mean values and susceptibilities of the measurement results.
	*
	* @param[in] _temperature system temperature
	*
	* @return The measurement results as a string.
	*/
	std::stringstream stream;
	stream << std::setprecision(15);
	if (_measurementIndex == _numberMeasurements)
	{
		Threedim meanValues = {0,0,0};
		for (int i = 0; i < _numberMeasurements; ++i)
		{
			meanValues.x += _values[i].x;
			meanValues.y += _values[i].y;
			meanValues.z += _values[i].z;
		}
		meanValues = MyMath::mult(meanValues, 1 / (double)_numberMeasurements);
		meanValues = MyMath::mult(meanValues, 1 / (double)_numberAtoms);

		stream  << meanValues.x << " "  << meanValues.y << " " << meanValues.z << " ";

		double esq = 0;
		for (int i = 0; i < _numberMeasurements; ++i)
		{
			esq += _values[i].x*_values[i].x + _values[i].y*_values[i].y + _values[i].z*_values[i].z;
		}
		meanValues = MyMath::mult(meanValues, _numberAtoms);
		esq /= _numberMeasurements;
		stream << (esq - MyMath::dot_product(meanValues,meanValues)) / (kB * temperature) << " ";

		if (_boolEachSpin == TRUE)
		{
			Threedim spinAverage = { 0,0,0 };
			Threedim* tmpPntr = NULL;
			for (int i = 0; i < _numberAtoms; i++, tmpPntr++)
			{
				spinAverage = { 0,0,0 };
				for (int j = 0; j < _numberMeasurements; j++)
				{
					tmpPntr = _valuesSpinResolved + j * _numberAtoms + i;
					spinAverage = MyMath::add(spinAverage, *tmpPntr);
				}
				spinAverage = MyMath::mult(spinAverage, 1 / (double)_numberMeasurements);
				stream << " " << spinAverage.x << " " << spinAverage.y << " " << spinAverage.z;
			}
		}
		else
		{
			std::cout << "Error in MagnetisationObservable::mean_value." << std::endl;
		}
	}
	return stream.str();
}

void MagnetisationObservable::clear_storage()
{
	delete[] _values;
	_values = new Threedim[_numberMeasurements];
	delete[] _valuesSpinResolved;
	_valuesSpinResolved = new Threedim[_numberMeasurements * _numberAtoms];
}
