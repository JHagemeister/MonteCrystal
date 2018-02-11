/*
* NCMRContrast.cpp
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

#include "NCMRContrastObservable.h"

#include "MyMath.h"

#include <sstream> 
#include <iomanip>
#include <iostream>

NCMRContrastObservable::NCMRContrastObservable(int numberMeasurements, Threedim* spinArray, int numberAtoms, 
	int* neighborArray, int nbors): Observable(numberMeasurements)
{
	_spinArray = spinArray;
	_numberAtoms = numberAtoms;
	_neighborArray = neighborArray;
	_nbors = nbors;

	_ncmrValues = new double*[_numberAtoms];
	for (int i = 0; i < _numberAtoms; ++i)
	{
		_ncmrValues[i] = new double[numberMeasurements];
	}
}

NCMRContrastObservable::~NCMRContrastObservable()
{
	for (int i = 0; i < _numberAtoms; ++i)
	{
		delete[] _ncmrValues[i];
		_ncmrValues[i] = NULL;
	}
	delete[] _ncmrValues;
	_ncmrValues = NULL;
}

std::string NCMRContrastObservable::get_steps_header(void) const
{
	std::string header;
	for (int i = 0; i < _numberAtoms; i++)
	{
		header.append("NCMR_" + std::to_string(i) + " ");
	}
	return header;
}

std::string NCMRContrastObservable::get_mean_header(void) const
{
	return get_steps_header();
}

void NCMRContrastObservable::take_value(void)
{
	double* valuesPntr = NULL;
	for (int i = 0; i < _numberAtoms; i++)
	{
		valuesPntr = _ncmrValues[i];
		valuesPntr[_measurementIndex] = ncmr_contrast_value(i);
	}
	++_measurementIndex;
}

std::string NCMRContrastObservable::get_step_value(const int &index) const
{
	std::stringstream stream;
	stream << std::setprecision(15);
	if (index < _numberMeasurements)
	{
		double* valuesPntr = NULL;
		for (int j = 0; j < _numberAtoms; ++j)
		{
			valuesPntr = _ncmrValues[j];
			stream << valuesPntr[index] << " ";
		}
	}
	else
	{
		stream << 0;
		std::cout << "Index out of bounds in class NCMR contrast obervable in function get_value(int i)." 
			<< std::endl;
	}
	return stream.str();
}

std::string NCMRContrastObservable::get_mean_value(const double &temperature) const
{
	std::stringstream stream;
	stream << std::setprecision(15);
	if (_measurementIndex == _numberMeasurements)
	{
		double* valuesPntr = NULL;
		double value = 0;
		for (int i = 0; i < _numberAtoms; ++i)
		{
			value = 0;
			valuesPntr = _ncmrValues[i];
			for (int j = 0; j < _numberMeasurements; ++j)
			{
				value += valuesPntr[j];
			}
			value /= _numberMeasurements;
			stream << value << " ";
		}
	}
	return stream.str();
}

void NCMRContrastObservable::clear_storage()
{
	for (int i = 0; i < _numberAtoms; ++i)
	{
		delete[] _ncmrValues[i];
		_ncmrValues[i] = NULL;
	}
	delete[] _ncmrValues;
	_ncmrValues = NULL;
	_ncmrValues = new double*[_numberAtoms];
	for (int i = 0; i < _numberAtoms; ++i)
	{
		_ncmrValues[i] = new double[_numberMeasurements];
	}
}

double NCMRContrastObservable::ncmr_contrast_value(int position)
{
	double ncmrValue = 0;
	int neighbor = 0; // index of neighbor atom
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
		{
			neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
			ncmrValue += MyMath::dot_product(_spinArray[position], _spinArray[neighbor]);
		}
	}
	return ncmrValue; // energy of single atom
}
