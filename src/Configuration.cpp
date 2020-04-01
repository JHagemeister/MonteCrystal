/*
* Configuration.cpp
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

#include "Configuration.h"

#include <iostream>

Configuration::Configuration()
{
	/**
	*  @param[in] confile name of configuration file
	*/
	_simulationType = metropolis;
	_programType = temperatureMagneticFieldLoop;
	_experiment01 = { 0 };
	
	// lattice parameters:
	_latticeType = simpleCubic;
	_latticeDimensions.assign(3,10);
	_latticeDimensions[2] = 1;
	_millerIndexes[0] = 0;
	_millerIndexes[1] = 0;
	_millerIndexes[2] = 0;
	_boundaryConditions = openBound;
	_magneticMoment = 1;
	_latticeConstant = 1; // lattice constant of respective material in Angstrom
	_gaussianSpinSamplingSigma = 0;
	
	// spin parameters
	_spinSystem = Heisenberg;
	_initialSpiralR = { 1, 0, 0 };
	_initialSpiralI = { 1, 0, 0 };
	_initialSpiralek = { 1, 0, 0 };
	_initialSpiralLambda = -1;
	
	// energy parameters
	_dmType = Neel;
	_hexagonalAnisotropyEnergies.push_back(0);
	_hexagonalAnisotropyEnergies.push_back(0);
	_hexagonalAnisotropyEnergies.push_back(0);
	_pseudoDipolarEnergy = 0;
	_biQuadraticEnergy = 0;
	_fourSpinEnergy = 0;
	_dipolEnergy = FALSE;
	_magneticField = { 0, 0, 1, { 0,0,1 } };
	_magneticTip = { { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0, 0, 0,};

	// parameters for Landau-Lifshitz-Gilbert simulation
	_LLG_dampingParameter = 0.1;
	_LLG_timeWidth = 0.05; // [ps]

	// LLG, Monte Carlo mutual parameters 
	_seed = 10;
	_temperatureStart = 1;
	_temperatureEnd = 1;
	_temperatureSteps = 1;
	_temperatureGradientDirection = { 0,0,0 };
	_outputWidth = 1000;
	_simulationSteps = 100000;

	_numEigenStates = 0;
	
	// parameters for output configuration
	_movieStart = -1;
	_movieEnd = -1;
	_movieWidth = 0;

	// parameters UI output
	_uiUpdateWidth = 10;
}

std::string Configuration::all_parameters(void) const
{
	/**
	* Create a single string _allParameters from all parameters to e.g. store in README later on
	*/
	std::string _allParameters;

	switch (_simulationType)
	{
	case metropolis:
		_allParameters.append("Simulation type: Metropolis");
		break;
	case landauLifshitzGilbert:
		_allParameters.append(" Simulation type: Landau-Lifshitz-Gilbert");
		break;
	}

	switch (_programType)
	{
	case temperatureMagneticFieldLoop:
		_allParameters.append(" Program type: T-H-Loop");
		break;
	case spinSeebeck:
		_allParameters.append(" Program type: Metropolis Seebeck");
		break;
	case tipMovement:
		_allParameters.append(" Program type: Metropolis tip movement");
		break;
	case latticeSiteEnergies:
		_allParameters.append(" Program type: lattice site energies");
		break;
	case latticeSiteWindingNumber:
		_allParameters.append(" Program type: lattice site winding number");
		break;
	}

	_allParameters.append("   Lattice type:");
	switch (_latticeType)
	{
	case simpleCubic:
		_allParameters.append(" simple cubic");
		break;
	case bodyCenteredCubic:
		_allParameters.append(" body centered cubic");
		break;
	case faceCenteredCubic:
		_allParameters.append(" face centered cubic");
		break;
	case triangularHexagonal:
		_allParameters.append(" 2D triangular hexagonal boundary");
		break;
	case triangularTriangular:
		_allParameters.append(" 2D triangular triangular boundary");
		break;
	case triangularHalfDisk:
		_allParameters.append(" 2D triangular half disk shape");
		break;
	case triangularDisk:
		_allParameters.append(" 2D triangular disk");
		break;
	case triangularArrowHead:
		_allParameters.append(" 2D triangular arrow head shape");
		break;
	case triangularStripe:
		_allParameters.append(" 2D triangular rectangular shape");
		break;
	default: _allParameters.append(" not recognized"); break;
	}

	for (int i = 0; i < _latticeDimensions.size(); ++i)
	{
		_allParameters.append("   DIM_" + std::to_string(i) + " " + std::to_string(_latticeDimensions[i]));
	}

	if (abs(_millerIndexes[0]) + abs(_millerIndexes[1]) + abs(_millerIndexes[2]) > PRECISION)
	{
		_allParameters.append("Miller indexes: h " + std::to_string(_millerIndexes[0]) +
			"k " + std::to_string(_millerIndexes[1]) + "l " + std::to_string(_millerIndexes[2]));
	}

	_allParameters.append("   Boundary conditions:");
	switch (_boundaryConditions)
	{
	case openBound:
		_allParameters.append(" open");
		break;
	case periodic:
		_allParameters.append(" periodic");
		break;
	case periodicX:
		_allParameters.append(" periodic_x");
		break;
	case periodicY:
		_allParameters.append(" periodic_y");
		break;
	}

	_allParameters.append("   Lattice constant: " + std::to_string(_latticeConstant));
	_allParameters.append("\n");
	_allParameters.append("Magnetic moment: " + std::to_string(_magneticMoment));

	_allParameters.append("Spin system: ");
	switch (_spinSystem)
	{
	case Heisenberg:
		_allParameters.append(" Heisenberg");
		break;
	case Ising:
		_allParameters.append(" Ising");
		break;
	default:
		_allParameters.append(" not detected");
	}

	_allParameters.append("   Gaussian spin sampling: ");
	if (_gaussianSpinSamplingSigma < PRECISION)
	{
		_allParameters.append("disabled");
	}
	else
	{
		_allParameters.append("enabled, sigma: " + std::to_string(_gaussianSpinSamplingSigma));
	}
	_allParameters.append("\n");

	_allParameters.append("Energies:");
	for (int i = 0; i < _exchangeEnergies.size(); ++i)
	{
		_allParameters.append(" J" + std::to_string(_exchangeEnergies[i].order) + ": " +
			std::to_string(_exchangeEnergies[i].energyParameter));
	}

	for (int i = 0; i < _DMEnergies.size(); ++i)
	{
		_allParameters.append(" D" + std::to_string(_DMEnergies[i].order) + ": " +
			std::to_string(_DMEnergies[i].energyParameter));
	}
	switch (_dmType)
	{
	case Neel:
		_allParameters.append(" Neel-type");
		break;
	case Chiral:
		_allParameters.append(" chiral");
		break;
	}

	if (abs(_pseudoDipolarEnergy) > PRECISION)
	{
		_allParameters.append("   Pseudo-Dipolar energy: " + std::to_string(_pseudoDipolarEnergy));
	}	

	if (abs(_biQuadraticEnergy) > PRECISION)
	{
		_allParameters.append("   Biquadratic energy: " + std::to_string(_biQuadraticEnergy));
	}
	
	if (abs(_fourSpinEnergy) > PRECISION)
	{
		_allParameters.append("   4-spin energy: " + std::to_string(_fourSpinEnergy));
	}

	if (abs(_threeSiteEnergy) > PRECISION)
	{
		_allParameters.append("   Three-site energy: " + std::to_string(_threeSiteEnergy));
	}

	for (int i = 0; i < _uniaxialAnisotropyEnergies.size(); ++i)
	{
		_allParameters.append("   Anisotorpy energy: "+std::to_string(
			_uniaxialAnisotropyEnergies[i].energyParameter));
		_allParameters.append(" , direction: " + std::to_string(
			_uniaxialAnisotropyEnergies[i].direction.x) + " "
			+ std::to_string(_uniaxialAnisotropyEnergies[i].direction.y) + " "
			+ std::to_string(_uniaxialAnisotropyEnergies[i].direction.z));
	}

	for (int i = 0; i < _hexagonalAnisotropyEnergies.size(); i++)
	{
		if (fabs(_hexagonalAnisotropyEnergies[i]) > PRECISION)
		{
			_allParameters.append(" K_" + std::to_string(i) + ": " + 
				std::to_string(_hexagonalAnisotropyEnergies[i]));
		}
	}

	if (_dipolEnergy == TRUE)
	{
		_allParameters.append("   Dipol energy");
	}

	for (int i = 0; i < _modulatedExchangeEnergies.size(); ++i)
	{
		_allParameters.append("   Modulated exchange energy: Jx "
			+ std::to_string(_modulatedExchangeEnergies[i].Jx) + " Jy "
			+ std::to_string(_modulatedExchangeEnergies[i].Jy) + " lambda "
			+ std::to_string(_modulatedExchangeEnergies[i].lambda) + " alpha "
			+ std::to_string(_modulatedExchangeEnergies[i].alpha) + " modulation number "
			+ std::to_string(_modulatedExchangeEnergies[i].modulationNumber));
	}

	for (int i = 0; i < _modulatedAnisotropyEnergies.size(); ++i)
	{
		_allParameters.append("   Modulated anisotropy energy: energy parameter "
			+ std::to_string(_modulatedAnisotropyEnergies[i].energyParameter) + " lambda "
			+ std::to_string(_modulatedAnisotropyEnergies[i].lambda) + " width "
			+ std::to_string(_modulatedAnisotropyEnergies[i].width) + " direction "
			+ std::to_string(_modulatedAnisotropyEnergies[i].direction.x) + " "
			+ std::to_string(_modulatedAnisotropyEnergies[i].direction.y) + " " 
			+ std::to_string(_modulatedAnisotropyEnergies[i].direction.z) + " modulation number "
			+ std::to_string(_modulatedAnisotropyEnergies[i].modulationNumber));
	}

	for (auto it = _exchangeDefects.begin(); it != _exchangeDefects.end(); ++it)
	{
		_allParameters.append("J");
		_allParameters.append(std::to_string(it->first));
		_allParameters.append("_Defects (site,energy): ");
		
		for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
		{
			_allParameters.append("(");
			_allParameters.append(std::to_string(it1->first));
			_allParameters.append(",");
			_allParameters.append(std::to_string(it1->second));
			_allParameters.append(")");
		}
	}

	for (auto it = _dmDefects.begin(); it != _dmDefects.end(); ++it)
	{
		_allParameters.append("D");
		_allParameters.append(std::to_string(it->first));
		_allParameters.append("_Defects (site,energy): ");

		for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
		{
			_allParameters.append("(");
			_allParameters.append(std::to_string(it1->first));
			_allParameters.append(",");
			_allParameters.append(std::to_string(it1->second));
			_allParameters.append(")");
		}
	}
	_allParameters.append(" ");

	if (!_anisotropyDefects.empty()) {
		_allParameters.append("K");
		_allParameters.append("_Defects (site,energy,direction): ");
		for (auto it = _anisotropyDefects.begin(); it != _anisotropyDefects.end(); ++it)
		{
			_allParameters.append("(");
			_allParameters.append(std::to_string(it->first));
			_allParameters.append(",");
			_allParameters.append(std::to_string(it->second.energyParameter));
			_allParameters.append(",");
			_allParameters.append("(");
			_allParameters.append(std::to_string(it->second.direction.x));
			_allParameters.append(",");
			_allParameters.append(std::to_string(it->second.direction.y));
			_allParameters.append(",");
			_allParameters.append(std::to_string(it->second.direction.z));
			_allParameters.append(")");
			_allParameters.append(")");
		}
		_allParameters.append(" ");
	}

	_allParameters.append("\n");
	
	_allParameters.append("Magnetic field: start " + std::to_string(_magneticField.start) + " end "
		+ std::to_string(_magneticField.end) + " steps " + std::to_string(_magneticField.steps) + " direction "
		+ std::to_string(_magneticField.direction.x) + " " + std::to_string(_magneticField.direction.y) + " "
		+ std::to_string(_magneticField.direction.z));

	if (_programType == tipMovement)
	{
		_allParameters.append("   Magnetic tip: energy parameter " + std::to_string(_magneticTip.energyParameter)
			+ " tip orientation " + std::to_string(_magneticTip.magnetizationDirection.x) + " "
			+ std::to_string(_magneticTip.magnetizationDirection.y) + " "
			+ std::to_string(_magneticTip.magnetizationDirection.z) + " "
			+ " start position " + std::to_string(_magneticTip.start.x) + " "
			+ std::to_string(_magneticTip.start.y) + " " + std::to_string(_magneticTip.start.z) + " "
			+ " end position " + std::to_string(_magneticTip.end.x) + " "
			+ std::to_string(_magneticTip.end.y) + " " + std::to_string(_magneticTip.end.z) + " steps "
			+ std::to_string(_magneticTip.steps) + " stepWidth " + std::to_string(_magneticTip.stepWidth));
	}
	_allParameters.append("\n");

	if (_simulationType == landauLifshitzGilbert)
	{
		_allParameters.append("damping parameter: " + std::to_string(_LLG_dampingParameter));
		_allParameters.append("time step width: " + std::to_string(_LLG_timeWidth));
	}

	_allParameters.append("seed: " + std::to_string(_seed));
	_allParameters.append("   Simulation steps: " + std::to_string(_simulationSteps));
	_allParameters.append(" Output width: " + std::to_string(_outputWidth));

	_allParameters.append("   temperature start: " + std::to_string(_temperatureStart));
	_allParameters.append(" temperature end: " + std::to_string(_temperatureEnd));
	_allParameters.append(" temperature steps: " + std::to_string(_temperatureSteps));

	if (_programType == spinSeebeck)
	{
		_allParameters.append("   Temperature gradient: temperature min = temperature start, temperature max = "
			"temperature end, direction: " + std::to_string(_temperatureGradientDirection.x) + " "
			+ std::to_string(_temperatureGradientDirection.y) + " "
			+ std::to_string(_temperatureGradientDirection.z));
	}
	_allParameters.append("\n");

	_allParameters.append("Output settings: ");
	if (_doSpinConfigOutput)
	{
		_allParameters.append("   SpinConfig ");
	}

	_allParameters.append("   Movie: start " + std::to_string(_movieStart) + " end " + std::to_string(_movieEnd)
		+ " width " + std::to_string(_movieWidth));
	
	if (_doSimulationStepsOutput)
	{
		_allParameters.append("   Monte_Carlo_steps ");
	}

	if (_doEnergyOutput)
	{
		_allParameters.append("   energy ");
	}

	if (_doMagnetisationOutput)
	{
		_allParameters.append("   magnetization ");
	}
	
	if (_doAbsoluteMagnetisationOutput)
	{
		_allParameters.append("   absolute_magnetization ");
	}

	if (_doSpinResolvedOutput)
	{
		_allParameters.append("   each_spin ");
	}

	if (_doWindingNumberOutput)
	{
		_allParameters.append("   winding_number ");
	}

	return _allParameters;
}

void Configuration::show_parameters(void)
{
	std::cout << "The following parameters have been defined " << std::endl;
	std::cout << all_parameters() << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}

void Configuration::determine_outputfolder_needed(void)
{
	_doOutput = _doSpinConfigOutput || _doEnergyOutput || _doMagnetisationOutput
		|| _doAbsoluteMagnetisationOutput || _doWindingNumberOutput || _doNCMROutput;
	
	if (_movieStart > -1 && _movieEnd > -1 && _movieEnd > _movieStart && _movieWidth > 0)
	{
		_doOutput = true;
	}

	if (_numEigenStates > 0)
	{
		_doOutput = true;
	}
}
