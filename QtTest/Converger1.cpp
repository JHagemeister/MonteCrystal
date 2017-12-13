/*
* Converger1.cpp
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

#include "Converger1.h"

// forward and further includes
#include "SpinOrientation.h"
#include "Hamiltonian.h"
#include "RanGen.h"
#include "MyMath.h"

#include "SimulationProgram.h"


Converger1::Converger1(SpinOrientation* spinOrientation,
	int simulationSteps, double temperature, QSharedPointer<Hamiltonian> hamilton,
	std::shared_ptr<RanGen> ranGen, SimulationProgram* simulationProgram) :
	SimulationMethod(spinOrientation, simulationSteps, temperature, hamilton, ranGen, simulationProgram)
{
}

Converger1::~Converger1()
{
}

double Converger1::simulation_step(void)
{
	/*
	* Simulation step for the Landau Lifshitz Gilbert differential equation
	*/

	// pointer to current spin array
	Threedim* spinArray = _spinOrientation->get_spin_array();

	// effective field as derived from the energies acting on a certain spin
	Threedim effectiveFieldDir = { 0,0,0 };

	double convergenceCriterion = 0;
	int position = 0;

	for (int i = 0; i < _numberActiveSites; i++)
	{
		position = _activeSites[i];

		effectiveFieldDir = MyMath::normalize(_hamilton->effectiveField(position));

		if (_boolConvergenceCriterion == TRUE)
		{
			double norm = MyMath::norm(MyMath::vector_product(spinArray[position], effectiveFieldDir));
			if (convergenceCriterion < norm)
			{
				convergenceCriterion = norm;
			}
		}
		spinArray[position] = effectiveFieldDir;
	}

	return convergenceCriterion;
}