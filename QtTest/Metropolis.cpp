/*
* Metropolis.cpp
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

#include "Metropolis.h"

//forward and further includes
#include "SpinOrientation.h"
#include "RanGen.h"
#include "Hamiltonian.h"


Metropolis::Metropolis(SpinOrientation* spinOrientation, int simulationSteps, double temperature, 
	QSharedPointer<Hamiltonian> hamilton, std::shared_ptr<RanGen> ranGen, 
	SimulationProgram* simulationProgram):
	SimulationMethod(spinOrientation, simulationSteps, temperature, hamilton, ranGen, simulationProgram)
{
	/**
	* @param[in] spinOrientation Spin information
	* @param[in] simulationSteps Number of simulation steps
	* @param[in] temperature Initial temperature [K]
	* @param[in] hamilton To calculate system energy
	* @param[in] ranGen Pseudo random number generator
	*/

	for (int i = 0; i < _numberActiveSites; ++i)
	{
		_randomizedSiteList.push_back(_activeSites[i]);
	}
}

Metropolis::~Metropolis()
{
}

double Metropolis::simulation_step(void)
{
	/**
	* This method performs a Monte-Carlo step according to the Metropolis algorithm. One simulation step
	* consists of N trial steps where N is the number of lattice sites included in the simulation process.
	* (Some lattice sites can be excluded from the simulation. See class SpinOrientation for more information)
	*/

	// helper value. lattice index determined randomly from array containing indexes of active lattice sites
	int position = 0;
	// energy difference between before and after random trial change
	double deltaEnergy = 0;
	// random Number between (0,1) to decline trial state with higher energy by certain probability
	double randomNumber = 0;
	// probability by which trial state is accepted
	double boltzmann = 0;

	int numberRejectedStates = 0;

	_ranGen->Shuffle(_randomizedSiteList);

	// one Monte Carlo steps consists of as many trial steps as there are active lattice sites.
	for (int i = 0; i < _numberActiveSites; ++i)
	{ 
		// choose a random active lattice site for trial change
		position = _randomizedSiteList[i];
		
		// energy before random spin reorientation
		deltaEnergy = _hamilton->single_energy(position);

		// randomly reorientate the spin at the previously determined lattice site
		this->_spinOrientation->single_orientation(position); 

		// energy difference before and after reorientation. negative sign -> energy increased
		deltaEnergy = deltaEnergy - _hamilton->single_energy(position);

		// if energy difference is positive, the energy decreased and the new state will be accepted. Nothing
		// needs to be done in this case. If energy increased, the new configuration is accepted with a 
		// probability according to a Boltzman factor.
		if (deltaEnergy < 0)
		{
			// random number between (0,1)
			randomNumber = _ranGen->Random();
			// boltzmann factor
			boltzmann = exp(deltaEnergy / (_temperature[position] * kB));
			
			// reject trial state if random number between (0,1) is larger than boltzmann factor
			if (randomNumber > boltzmann)
			{
				// restore original orientation
				_spinOrientation->restore_single_orientation(); 

				numberRejectedStates += 1;
			}
		}
	}
	return (double)(_numberActiveSites-numberRejectedStates)/_numberActiveSites;
}