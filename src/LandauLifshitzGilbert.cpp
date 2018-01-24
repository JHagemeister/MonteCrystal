/*
* LandauLifshitzGilbert.cpp
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

#include "LandauLifshitzGilbert.h"

// forward and further includes
#include "SpinOrientation.h"
#include "Hamiltonian.h"
#include "RanGen.h"
#include "MyMath.h"

#include "SimulationProgram.h"


LandauLifshitzGilbert::LandauLifshitzGilbert(SpinOrientation* spinOrientation, 
	int simulationSteps, double temperature, QSharedPointer<Hamiltonian> hamilton, 
	std::shared_ptr<RanGen> ranGen, double timeWidth, double dampingParameter, double magneticMoment,
	SimulationProgram* simulationProgram):
	SimulationMethod(spinOrientation, simulationSteps, temperature, hamilton, ranGen, simulationProgram)
{
	/**
	* @param[in] spinOrientation Spin information
	* @param[in] simulationSteps Number of simulation steps
	* @param[in] temperature Initial temperature [K]
	* @param[in] hamilton To calculate system energy
	* @param[in] ranGen Pseudo random number generator
	* @param[in] timeWidth Time width of a single simulation step [ps]
	* @param[in] dampingParameter Gilbert damping parameter; 1 for fastest relaxation; typical value 0.1 []
	* @param[in] magneticMoment Multiples of muBohr
	* @param[in] simulationProgram Needed to trigger updates in graphical user interface
	*/

	_timeWidth = timeWidth * pow(10,-12); // [ps] to [s]
	_sqrtTimeWidth = sqrt(_timeWidth); // needed in each step solving LLG diff equation
	_dampingParameter = dampingParameter;
	_magneticMoment = magneticMoment * muBohr; //[meV/T]
	_inverseMagneticMoment = 1./_magneticMoment; //[T/meV]
	
	// additional space; even number of thermal field vectors will be created and number should be larger than
	// number of spins included in simulation
	_thermalFieldVariance = new double[_numberActiveSites + 2];

	set_reduced_gyromagnetic_ratio();
	set_thermal_field_variance();
}

LandauLifshitzGilbert::~LandauLifshitzGilbert()
{
	delete _thermalFieldVariance;
}

double LandauLifshitzGilbert::simulation_step(void)
{
	/*
	* Simulation step for the Landau Lifshitz Gilbert differential equation
	*/

	// pointer to current spin array
	Threedim* spinArray = _spinOrientation->get_spin_array();

	// prefactors
	double bPrimeValue = -_reducedGyromagneticRatio / 4.;
	double bValue = -_reducedGyromagneticRatio / 2.;
	
	// helper vectors
	Threedim* bVector = new Threedim[_numberActiveSites];
	Threedim* aVector = new Threedim[_numberActiveSites];

	// effective field as derived from the energies acting on a certain spin
	Threedim effectiveField = { 0,0,0 };
	
	// random field that arises due to temperature fluctuations
	Threedim* randomField = new Threedim[_numberActiveSites+2];

	// temporary spin array at intermediate time step
	Threedim* spinArrayTemp = new Threedim[_spinOrientation->get_number_atoms()];

	// lattice index as derived from array containing the indexes of the active lattice sites
	int position = 0;

	// loop to determine random fluctuation field at the active lattice sites
	for (int i = 0; i < _numberActiveSites/2 + 1; i++)
	{
		// get two random fields with 3 components each at the same time
		standard_normal_variables(randomField[2*i], randomField[2*i + 1]);

		// random field
		randomField[2*i] = MyMath::mult(randomField[2*i], _sqrtTimeWidth*_thermalFieldVariance[2*i]);

		// random field
		randomField[2*i+1] = MyMath::mult(randomField[2*i+1], _sqrtTimeWidth*_thermalFieldVariance[2*i+1]);
	}

	// helper vector and value
	Threedim tmpVector = { 0,0,0 };
	double tmpValue = 0;

	double convergenceCriterion = 0;

	for (int i = 0; i < _numberActiveSites; i++)
	{
		position = _activeSites[i];

		effectiveField = _hamilton->effectiveField(position);

		if (_boolConvergenceCriterion == TRUE)
		{
			double norm = MyMath::norm(MyMath::vector_product(spinArray[position], effectiveField));
			if (convergenceCriterion < norm)
			{
				convergenceCriterion = norm;
			}
		}
		
		effectiveField = MyMath::mult(effectiveField, _inverseMagneticMoment*_timeWidth);
		
		bVector[i] = MyMath::add(effectiveField, randomField[i]);
		tmpVector = MyMath::vector_product(spinArray[position],bVector[i]);

		tmpVector = MyMath::mult(tmpVector, _dampingParameter);
		bVector[i] = MyMath::add(bVector[i], tmpVector);
		bVector[i] = MyMath::mult(bVector[i], bPrimeValue);

		aVector[i] = MyMath::vector_product(spinArray[position], bVector[i]);
		aVector[i] = MyMath::add(spinArray[position], aVector[i]);

		tmpVector = MyMath::vector_product(aVector[i], bVector[i]);
		spinArrayTemp[position] = MyMath::add(aVector[i], tmpVector);
		tmpVector = MyMath::mult(bVector[i], MyMath::dot_product(aVector[i], bVector[i]));
		spinArrayTemp[position] = MyMath::add(spinArrayTemp[position], tmpVector);
		tmpValue = 1./(1 + MyMath::dot_product(bVector[i], bVector[i]));
		spinArrayTemp[position] = MyMath::mult(spinArrayTemp[position], tmpValue);
	}

	for (int i = 0; i < _numberInactiveSites; i++)
	{
		position = _inactiveSites[i];
		spinArrayTemp[position] = spinArray[position];
	}

	_hamilton->set_spin_array(spinArrayTemp);

	for (int i = 0; i < _numberActiveSites; i++)
	{
		position = _activeSites[i];

		effectiveField = MyMath::mult(_hamilton->effectiveField(position), _inverseMagneticMoment*_timeWidth);

		bVector[i] = MyMath::add(effectiveField, randomField[i]);
		tmpVector = MyMath::vector_product(spinArray[position], bVector[i]);
		tmpVector = MyMath::mult(tmpVector, _dampingParameter);
		bVector[i] = MyMath::add(bVector[i], tmpVector);
		bVector[i] = MyMath::mult(bVector[i], bValue);

		aVector[i] = MyMath::vector_product(spinArray[position], bVector[i]);
		aVector[i] = MyMath::add(spinArray[position], aVector[i]);
	}

	for (int i = 0; i < _numberActiveSites; i++)
	{
		position = _activeSites[i];

		tmpVector = MyMath::vector_product(aVector[i], bVector[i]);
		spinArray[position] = MyMath::add(aVector[i], tmpVector);
		tmpVector = MyMath::mult(bVector[i], MyMath::dot_product(aVector[i], bVector[i]));
		spinArray[position] = MyMath::add(spinArray[position], tmpVector);
		tmpValue = 1./(1 + MyMath::dot_product(bVector[i], bVector[i]));
		spinArray[position] = MyMath::mult(spinArray[position], tmpValue);
	}
	_hamilton->set_spin_array(spinArray);

	delete[] bVector;
	delete[] aVector;
	delete[] randomField;
	delete[] spinArrayTemp;

	return convergenceCriterion;
}

void LandauLifshitzGilbert::standard_normal_variables(Threedim &vec1, Threedim &vec2)
{
	/**
	In the methods for the creation of normal distributed random numbers there are always two random numbers 
	needed that yield two normal distributed random numbers. In order to not waste any created normal 
	distributed random numbers the values of two thermal field vectors are set within this function. (6 values
	needed which is multiple of two.)

	@param[out] vec1 first vector which will contain three normal distributed random numbers
	@param[out] vec2 second vector which will contain three normal distributed random numbers
	*/
	_ranGen->polar(vec1.x, vec1.y);
	_ranGen->polar(vec1.z, vec2.x);
	_ranGen->polar(vec2.y, vec2.z);
}

void LandauLifshitzGilbert::set_temperature(double temperature)
{
	/**
	Reimplemantion of parent method because not only temperature at spin sites needs to be set but also the
	variance for the random thermal field has to be updated for each spin site.
	*/
	SimulationMethod::set_temperature(temperature);
	set_thermal_field_variance();
}

void LandauLifshitzGilbert::set_temperature_gradient(double temperatureMin, double temperatureMax, 
	Threedim direction,	Lattice* lattice)
{
	/**
	Reimplemantion of parent method because not only temperature at spin sites needs to be set but also the 
	variance for the random thermal field has to be updated for each spin site.
	*/
	SimulationMethod::set_temperature_gradient(temperatureMin, temperatureMax, direction, lattice);
	set_thermal_field_variance();
}

void LandauLifshitzGilbert::set_thermal_field_variance(void)
{
	/**
	Set variance of random thermal field for each spin site. This is necessary since the temperature may
	vary along system
	*/
	int position = 0;
	for (int i = 0; i < _numberActiveSites; i++)
	{
		position = _activeSites[i];
		_thermalFieldVariance[i] = sqrt(2*_dampingParameter*kB*_temperature[position]/
			                       (gammaElectron * _magneticMoment));
	}
}

void LandauLifshitzGilbert::set_reduced_gyromagnetic_ratio(void)
{
	/**
	Set value of reduced gyromagnetic ratio.
	*/
	_reducedGyromagneticRatio = gammaElectron / (1 + pow(_dampingParameter,2));
}