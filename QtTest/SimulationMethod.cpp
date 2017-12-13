/*
* SimulationMethod.cpp
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

#include "SimulationMethod.h"

// forward includes
#include "SpinOrientation.h"
#include "Hamiltonian.h"
#include "RanGen.h"
#include "Measurement.h"
#include "SimulationProgram.h"
#include "Lattice.h"


SimulationMethod::SimulationMethod(SpinOrientation* spinOrientation, int simulationSteps, double temperature, 
	QSharedPointer<Hamiltonian> hamilton, std::shared_ptr<RanGen> ranGen, SimulationProgram* simulationProgram)
{
	/**
	* @param[in] spinOrientation Spin information
	* @param[in] simulationSteps Number of simulation steps per run
	* @param[in] temperature Initial temperature for each spin site [K]
	* @param[in] hamilton Hamiltonian to calculate system energy
	* @param[in] ranGen Pseudo random number generator
	* @param[in] simulationProgram Needed to trigger updates in graphical user interface
	*/

	_spinOrientation = spinOrientation;
	_simulationSteps = simulationSteps;
	
	_temperature = new double[_spinOrientation->get_number_atoms()];
	set_temperature(temperature);

	_hamilton = hamilton;
	_ranGen = ranGen;
	_activeSites = spinOrientation->get_active_sites();
	_numberActiveSites = spinOrientation->get_number_active_sites();
	_inactiveSites = spinOrientation->get_inactive_sites();
	_numberInactiveSites = spinOrientation->get_number_inactive_sites();

	_boolConvergenceCriterion = FALSE;

	_simulationProgram = simulationProgram;
}

SimulationMethod::~SimulationMethod()
{
	delete[] _temperature;
}

void SimulationMethod::run_simulation(int uiUpdateWidth, std::shared_ptr<Measurement> measurement, 
	int outputWidth, int movieStart, int movieEnd, int movieWidth, std::string fname)
{
	/**
	* This function performs one simulation run with _simulationSteps number of simulation steps.
	*
	* @param[in] uiUpdateWidth Update GUI every uiUpdateWidth simulation steps
	* @param[in] measurement Take measurement values for energy, magnetization...
	* @param[in] outputWidth Take measurement value every outputWidth simulation steps
	* @param[in] movieStart Simulation step to start output of spin configuration
	* @param[in] movieEnd Simulation step to end output of spin configuration
	* @param[in] movieWidth Between movieStart and movieEnd save spin configuration every movieWidth steps
	* @param[in] fname Basis name to save spin configuration between movieStart and movieEnd
	*/

	std::stringstream stream;

	double convergenceCriterion = 1;
	
	for (int i = 1; i < _simulationSteps + 1; i++)
	{

		_simulationProgram->_mutex->lock();
		if ((i % uiUpdateWidth) == 0)
		{
			_boolConvergenceCriterion = TRUE;
		}
		convergenceCriterion = simulation_step();
		_simulationProgram->_mutex->unlock();

		if ((i % uiUpdateWidth) == 0)
		{
			_simulationProgram->send_simulation_step("Step = " + QString::number(i));
			_simulationProgram->send_simulation_convergence_criterion("conv. = " 
				+ QString::number(convergenceCriterion));
			_simulationProgram->send_repaint_request();

			_simulationProgram->_mutex->lock();
			if (*(_simulationProgram->_terminateThread) == 1)
			{
				_simulationProgram->_mutex->unlock();
				return;
			}
			_simulationProgram->_mutex->unlock();
			_boolConvergenceCriterion = FALSE;
		}

		if ((i % outputWidth) == 0)
		{
			measurement->measure();
		}

		if ((i >= movieStart) && (i <= movieEnd))
		{
			if (((i - movieStart) % movieWidth) == 0) {
				stream.str("");
				stream.clear();
				stream << "SpinConfiguration_" << i;
				_simulationProgram->send_simulation_step("Step = " + QString::number(i));
				_simulationProgram->send_repaint_request();
				_simulationProgram->send_save_image_request(QString::fromStdString(fname + stream.str()
					+  ".png"));
				Functions::save(_spinOrientation->get_activity_list(), _spinOrientation->get_spin_array(), 
					_spinOrientation->get_number_atoms(), fname + stream.str());
			}
		}
	}
}

void SimulationMethod::relaxate(int simulationSteps)
{
	/**
	* Relaxate system.
	*
	* @param[in] simulationSteps Number of simulation steps to be performed
	*/

	for (int i = 0; i < simulationSteps; ++i)
	{
		simulation_step();
	}
}

void SimulationMethod::set_temperature(double temperature)
{
	/**
	* Set a uniform temperature
	*
	* @param[in] temperature in [K]
 	*/
	for (int i = 0; i < _spinOrientation->get_number_atoms(); i++)
	{
		_temperature[i] = temperature;
	}
}

void SimulationMethod::set_temperature_gradient(double temperatureMin, double temperatureMax, 
	Threedim direction, Lattice* lattice)
{
	/**
	* Set a linear temperature gradient across lattice structure. 
	*
	* @param[in] temperatureMin Minimum temperature
	* @param[in] temperatureMax Maximum temperature
	* @param[in] direction Direction along which the temperature shall change
	* @param[in] lattice Lattice information
	*/
	Threedim* latticeCoordArray = lattice->get_lattice_coordinate_array();
	_temperature[0] = MyMath::dot_product(latticeCoordArray[0], direction);
	double minDotProd = _temperature[0];
	double maxDotProd = _temperature[0];
	for (int i = 1; i < lattice->get_number_atoms(); ++i)
	{
		_temperature[i] = MyMath::dot_product(latticeCoordArray[i], direction);
		if (_temperature[i] < minDotProd)
		{
			minDotProd = _temperature[i];
		}
		if (_temperature[i] > maxDotProd)
		{
			maxDotProd = _temperature[i];
		}
	}
	if ((maxDotProd - minDotProd) > PRECISION)
	{
		double slope = (temperatureMax - temperatureMin) / (maxDotProd - minDotProd);
		double offSet = temperatureMin - slope * minDotProd;
		for (int i = 0; i < lattice->get_number_atoms(); ++i)
		{
			_temperature[i] = slope*_temperature[i] + offSet;
		}
	}
	else
	{
		for (int i = 0; i < lattice->get_number_atoms(); ++i)
		{
			_temperature[i] = (temperatureMin + temperatureMax) / (double) 2;
		}
	}
}