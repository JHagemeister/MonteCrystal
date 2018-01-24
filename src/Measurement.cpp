/*
* Measurement.cpp
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

#include "Measurement.h"

Measurement::Measurement(std::vector<std::shared_ptr<Observable>> observables):
	_meanBody("")
{
	/**
	* @param[in] observables Pointers to observables.
	*/

	_observables = observables; // pointer on array of pointers on observables
}

Measurement::~Measurement()
{
}

void Measurement::set_number_measurements(int numMeasurements)
{
	/**
	* @param[in] numberMeasurements Number of measurements before mean value will be calculated
	*/

	for (int i = 0; i < _observables.size(); ++i)
	{
		_observables[i]->set_number_measurements(numMeasurements);
	}
}

void Measurement::measure(void)
{
	/**
	* Perform measurements on the system.
	*/

	for (int i = 0; i < _observables.size(); ++i)
	{
		_observables[i]->take_value();
	}

}

void Measurement::take_mean_values(std::string variable, double temperature)
{
	/**
	* This function stores the mean values of the observables.
	*
	* @param[in] variable The value(s) of the variable(s) is/are stored in the beginning of a line. Do not
	*                     provide a blank space in the end.
	* @param[in] temperature The system temperature [K]
	*/

	_meanBody.append(variable + " ");
	for (int j = 0; j < _observables.size(); ++j)
	{
		_meanBody.append(_observables[j]->get_mean_value(temperature));
	}
	_meanBody.append("\n");
}

void Measurement::reset_observables_measurement_index(void)
{
	/**
	* This function resets the measurement index of the observables. This function is called before a new
	* simulation run (e.g. after a step of temperature and magnetic field loop).
	*/

	for (int j = 0; j < _observables.size(); ++j)
	{
		_observables[j]->set_measurement_index(0);
		_observables[j]->clear_storage();
	}
}

void Measurement::save_mean_steps(std::string fname, std::string variableName)
{
	/**
	* This function stores the mean values of the observables into a file.
	*
	* @param[in] fname The name of the output file.
	* @param[in] variableHeader The name(s) of the variable(s) for which the measurements are done. Make sure
	*                           that there is no blank space in the end.
	*/

	std::fstream filestr;
	filestr.open(fname, std::fstream::out);

	filestr << variableName << " ";
	for (int i = 0; i < _observables.size(); ++i)
	{
		filestr << _observables[i]->get_mean_header();
	}
	filestr << std::endl;
	filestr << _meanBody;
	filestr.close();
}