/*
* EnergyObservable.cpp
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

#include "EnergyObservable.h"

//forward includes
#include "Hamiltonian.h"

#include "MyMath.h"

#include <iostream>
#include <string>
#include <sstream> 
#include <iomanip>

EnergyObservable::EnergyObservable(int numberMeasurements, QSharedPointer<Hamiltonian> hamilton,
	int numberAtoms, bool eachSpin):
	Observable(numberMeasurements)
{
	/**
	* @param[in] numberMeasurements Number of measurement values that can be stored
	* @param[in] hamilton The Hamiltonian to obtain information about system energy
	* @param[in] numberAtoms Total number of spins
	*/

	_hamilton = hamilton;
	_numberEnergies = hamilton->get_number_energies();
	_numberAtoms = numberAtoms;
	_eachSpin = eachSpin;

	_singleEnergies = new double*[_numberEnergies];
	for (int i = 0; i < _numberEnergies; ++i)
	{
		_singleEnergies[i] = new double[numberMeasurements];
	}
	_singleEnergiesperspin = new double**[_numberEnergies + 1];
	for (int i = 0; i < _numberEnergies + 1; ++i)
	{
		_singleEnergiesperspin[i] = new double*[_numberAtoms];
		for (int j = 0; j < _numberAtoms; ++j)
		{
			_singleEnergiesperspin[i][j] = new double[numberMeasurements];
		}
	}
}

EnergyObservable::~EnergyObservable()
{
	for (int i = 0; i < _numberEnergies; ++i)
	{
		delete[] _singleEnergies[i];
		_singleEnergies[i] = NULL;
	}
	delete[] _singleEnergies;
	_singleEnergies = NULL;
	for (int i = 0; i < _numberEnergies + 1; ++i)
	{
		for (int j = 0; j < _numberAtoms; ++j)
		{
			delete[] _singleEnergiesperspin[i][j];
			_singleEnergiesperspin[i][j] = NULL;
		}
		delete[] _singleEnergiesperspin[i];
		_singleEnergiesperspin[i] = NULL;
	}
	delete[] _singleEnergiesperspin;
	_singleEnergiesperspin = NULL;
}

std::string EnergyObservable::get_steps_header(void) const
{
	/**
	* This function returns the header for the output file.
	*
	* @return The header as a string.
	*/
	std::string header;
	std::vector<std::shared_ptr<Energy>> energies = _hamilton->get_energies();
	if (_numberEnergies != energies.size())
	{
		std::cout << "Error in EnergyObservable::monte_carlo_steps_header()" << std::endl;
	}
	else
	{
		for (int i = 0; i < _numberEnergies; ++i)
		{
			header.append(energies[i]->get_string_id());
		}
		header.append("E ");
		if (_eachSpin == TRUE)
		{
			for (int i = 0; i < _numberEnergies; ++i)
			{
				for (int j = 0; j < _numberAtoms; ++j)
				{
					header.append("S_" + std::to_string(j) + "_" + energies[i]->get_string_id());
				}
			}
		for (int j = 0; j < _numberAtoms; ++j)
			{
				header.append("S_" + std::to_string(j) + "_E ");
			}
		}

	}
	return header;
}

std::string EnergyObservable::get_mean_header(void) const
{
	/**
	* This function returns the header for the output file.
	*
	* @return The header as a string.
	*/
	std::string header = get_steps_header();
	header.append("C ");

	return header;
}

void EnergyObservable::take_value(void)
{
	/**
	* Do a measurement on the system.
	*/
	double* valuesPntr = NULL;
	double* valuesPntrperspin = NULL;
	double* valuesPntrperspintotal = NULL;
	for (int i = 0; i < _numberEnergies; ++i)
	{
		valuesPntr = _singleEnergies[i];
		valuesPntr[_measurementIndex] = _hamilton->part_energy(i);
		if (_eachSpin)
		{
			for (int j = 0; j < _numberAtoms; ++j)
			{
				valuesPntrperspin = _singleEnergiesperspin[i][j];
				valuesPntrperspin[_measurementIndex] = _hamilton->single_part_energy(i, j);
				valuesPntrperspintotal = _singleEnergiesperspin[_numberEnergies][j];
				if (i == 0)
				{
					valuesPntrperspintotal[_measurementIndex] = 0.0;
				}
				valuesPntrperspintotal[_measurementIndex] += valuesPntrperspin[_measurementIndex];
			}
		}

	}
	++_measurementIndex;
}

std::string EnergyObservable::get_step_value(const int &index) const
{
	std::stringstream stream;
	stream << std::setprecision(15);
	if (index < _numberMeasurements)
	{
		double* valuesPntr = NULL;
		double* valuesPntrperspin = NULL;
		double totalEnergy = 0;
		for (int j = 0; j < _numberEnergies; ++j)
		{
			valuesPntr = _singleEnergies[j];
			stream << valuesPntr[index] / _numberAtoms << " ";
			totalEnergy += valuesPntr[index];
		}
		stream << totalEnergy / _numberAtoms << " ";
		if (_eachSpin == TRUE)
		{
			for (int j = 0; j < _numberEnergies + 1; ++j)
			{
				for (int k = 0; k < _numberAtoms; ++k)
				{
					valuesPntrperspin = _singleEnergiesperspin[j][k];
					stream << valuesPntrperspin[index] << " ";
				}
			}
		}
	}
	else
	{
		stream << 0;
		std::cout << "Index out of bounds in class EnergyObervable in function get_value(int i)." << std::endl;
	}
	return stream.str();
}

std::string EnergyObservable::get_mean_value(const double &temperature) const
{
	std::stringstream stream;
	stream << std::setprecision(15);
	if (_measurementIndex == _numberMeasurements)
	{
		double* valuesPntr = NULL;
		double value = 0;
		for (int i = 0; i < _numberEnergies; ++i)
		{
			value = 0;
			valuesPntr = _singleEnergies[i];
			for (int j = 0; j < _numberMeasurements; ++j)
			{
				value += valuesPntr[j];
			}
			value /= _numberMeasurements;
			value /= _numberAtoms;
			stream << value << " ";
		}

		double* totalEnergies = new double[_numberMeasurements];
		for (int i = 0; i < _numberMeasurements; ++i)
		{
			totalEnergies[i] = 0;
			for (int j = 0; j < _numberEnergies; ++j)
			{
				totalEnergies[i] += _singleEnergies[j][i];
			}
		}

		stream << MyMath::mean_value(totalEnergies, _numberMeasurements) / _numberAtoms << " ";

		if (_eachSpin == TRUE)
		{
			double* valuesPntrperspin = NULL;
			double valueperspin = 0.0;
			for (int i = 0; i < _numberEnergies + 1; ++i)
			{
				for (int k = 0; k < _numberAtoms; ++k)
				{
					valueperspin = 0.0;
					valuesPntrperspin = _singleEnergiesperspin[i][k];
					for (int j = 0; j < _numberMeasurements; ++j)
					{
						valueperspin += valuesPntrperspin[j];
					}
					valueperspin /= _numberMeasurements;
					stream << valueperspin << " ";
				}
			}
		}

		stream << MyMath::variance(totalEnergies, _numberMeasurements) / (kB * pow(temperature, 2)) << " ";

		delete[] totalEnergies;
	}
	else
	{
		std::cout << "Error in EnergyObservable: values are averaged before last Monte-Carlo-Step." << std::endl;
	}
	return stream.str();
}

void EnergyObservable::clear_storage()
{
	for (int i = 0; i < _numberEnergies; ++i)
	{
		delete[] _singleEnergies[i];
		_singleEnergies[i] = NULL;
	}
	delete[] _singleEnergies;
	_singleEnergies = NULL;
	for (int i = 0; i < _numberEnergies + 1; ++i)
	{
		for (int j = 0; j < _numberAtoms; ++j)
		{
			delete[] _singleEnergiesperspin[i][j];
			_singleEnergiesperspin[i][j] = NULL;
		}
		delete[] _singleEnergiesperspin[i];
		_singleEnergiesperspin[i] = NULL;
	}
	delete[] _singleEnergiesperspin;
	_singleEnergiesperspin = NULL;
	_singleEnergies = new double*[_numberEnergies];
	for (int i = 0; i < _numberEnergies; ++i)
	{
		_singleEnergies[i] = new double[_numberMeasurements];
	}
	_singleEnergiesperspin = new double** [_numberEnergies + 1];
	for (int i = 0; i < _numberEnergies + 1; ++i)
	{
		_singleEnergiesperspin[i] = new double* [_numberAtoms];
		for (int j = 0; j < _numberAtoms; ++j)
		{
			_singleEnergiesperspin[i][j] = new double[_numberMeasurements];
		}
	}
}
