/*
* WindingNumber.cpp
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

#include "WindingNumber.h"

// further includes
#include "MyMath.h"

WindingNumber::WindingNumber(int numberMeasurements, Threedim* spinArray, TopologicalChargeCell* cells, int cellNum):
	Observable(numberMeasurements), _spinArray(spinArray), _cells(cells), _cellNum(cellNum)
{	
	/**
	* @param[in] numberMeasurements Number of measurement values before memory is full
	* @param[in] spinArray Spin configuration
	* @param[in] cells Size is 3*cellNum and 3 successive entries contain spin indexes of one cell for 
	*                  calculation of topological charge contribution to skyrmion number
	* @param[in] cellNum Number of cells to calculate topological charge contribution for
	*/

	_localWindingNumber = new double[cellNum];
	_windingNumber = new double[numberMeasurements];
	_areaUnitSphere = 4 * Pi;
	evaluate_winding_number();
}

WindingNumber::~WindingNumber()
{
	delete[] _windingNumber;
	delete[] _localWindingNumber;
}

std::string WindingNumber::get_steps_header(void)
{
	return "WindingNumber ";
}

std::string WindingNumber::get_mean_header(void)
{
	return "WindingNumber WindingNumber_SUS ";
}

void WindingNumber::take_value(void)
{
	_windingNumber[_measurementIndex] = evaluate_winding_number();
	++_measurementIndex;
}

std::string WindingNumber::get_step_value(int index)
{
	std::stringstream stream;
	if (index < _numberMeasurements)
	{
		stream << _windingNumber[index] << " ";
	}
	else
	{
		stream << 0;
		std::cout << "Error in WindingNumber::get_monte_carlo_step_value."	<< std::endl;
	}
	return stream.str();
}

std::string WindingNumber::get_mean_value(double temperature)
{
	std::stringstream stream;
	if (_measurementIndex == _numberMeasurements)
	{
		stream << MyMath::mean_value(_windingNumber, _numberMeasurements) << " ";
		stream << MyMath::variance(_windingNumber, _numberMeasurements) << " ";
	}
	else
	{
		std::cout << "Error in WindingNumber::get_monte_carlo_step_averaged_value" << std::endl;
	}
	return stream.str();
}

void WindingNumber::clear_storage(void)
{
	delete[] _windingNumber;
	_windingNumber = new double[_numberMeasurements];
}

double WindingNumber::evaluate_winding_number(void)
{
	double N = 0;
	double D = 0;
	Threedim spin1 = { 0,0,0 };
	Threedim spin2 = { 0,0,0 };
	Threedim spin3 = { 0,0,0 };

	double windN = 0;
	for (int i = 0; i < _cellNum; ++i)
	{
		spin1 = _spinArray[_cells[i].i];
		spin2 = _spinArray[_cells[i].j];
		spin3 = _spinArray[_cells[i].k];
		N = MyMath::dot_product(spin1, MyMath::vector_product(spin2, spin3));
		D = 1 + MyMath::dot_product(spin1, spin2) + MyMath::dot_product(spin1, spin3)
			+ MyMath::dot_product(spin2, spin3);
		_localWindingNumber[i] = 2 * atan2(N, D);
		windN += _localWindingNumber[i];
	}
	return windN / _areaUnitSphere;
}

double* WindingNumber::get_local_winding_number(void)
{
	return _localWindingNumber;
}