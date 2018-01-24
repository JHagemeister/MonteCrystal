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

Configuration::Configuration(std::string confile)
{
	/**
	*  @param[in] confile name of configuration file
	*/

	_confile = confile;
	_allParameters = "";

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
	_boolOutput = FALSE;
	_boolSpinConfig = FALSE;
	_movieStart = -1;
	_movieEnd = -1;
	_movieWidth = 0;
	_boolOutSimulationSteps = FALSE;
	_boolE = FALSE;
	_boolM = FALSE;
	_boolMABS = FALSE;
	_boolNCMR = FALSE;
	_boolEachSpin = FALSE;
	_boolWindingNumber = FALSE;

	// parameters UI output
	_uiUpdateWidth = 10;
}

Configuration::~Configuration()
{
}

int Configuration::read_parameters(void)
{
	/**
	* Read configuration parameters from configuration file.
	*/

	std::ifstream infile(_confile);
	std::string line;
	std::string identifier;

	if (!infile)
	{
		std::cout << "The configuration file does not exist!" << std::endl;
		return 0;
	}
	while (std::getline(infile, line))
	{
		
		// look for comments and empty rows
		if (int(line.find("\n")) == 0 || line.find(" ") == std::string::npos)
		{ 
		}
		else
		{
			std::istringstream isstream(line);
			identifier.clear();
			isstream >> identifier;

			if (identifier.compare("SIMULATIONTYPE") == 0)
			{ //Find the filename of an existing lattice
				std::string parameter = next(isstream);
				if (parameter.compare("METROPOLIS") == 0) 
				{
					_simulationType = metropolis;
				}
				if (parameter.compare("LLG") == 0) 
				{
					_simulationType = landauLifshitzGilbert;
				}
			}

			if (identifier.compare("PROGRAMTYPE") == 0)
			{ //Find the filename of an existing lattice
				std::string parameter = next(isstream);
				if (parameter.compare("STANDARD") == 0) {
					_programType = temperatureMagneticFieldLoop;
				}
				if (parameter.compare("SEEBECK") == 0) {
					_programType = spinSeebeck;
				}
				if (parameter.compare("TIPMOVEMENT") == 0) {
					_programType = tipMovement;
				}
				if (parameter.compare("LATTICESITEENERGIES") == 0) {
					_programType = latticeSiteEnergies;
				}
				if (parameter.compare("LATTICESITEWINDINGNUMBER") == 0) {
					_programType = latticeSiteWindingNumber;
				}
			}

			lattice_parameters(identifier, isstream);

			spin_system_paremeters(identifier, isstream);

			energy_parameters(identifier, isstream);

			if (identifier.compare("T") == 0) //Find the temperature loop
			{
				_temperatureStart = std::stof(next(isstream));
				_temperatureEnd = std::stof(next(isstream));
				_temperatureSteps = std::stoi(next(isstream));
			}

			if (identifier.compare("DT") == 0) //Find the temperature loop
			{
				_temperatureGradientDirection.x = std::stof(next(isstream));
				_temperatureGradientDirection.y = std::stof(next(isstream));
				_temperatureGradientDirection.z = std::stof(next(isstream));
			}

			if (identifier.compare("SIMULATIONSTEPS") == 0) //Find the number of Monte-Carlo steps
			{
				_simulationSteps = std::stoi(next(isstream));
			}
			
			if (identifier.compare("OUTPUTWIDTH") == 0) //Find the Monte-Carlo step width for taking measurements
			{
				_outputWidth = std::stoi(next(isstream));
			}

			if (identifier.compare("MOVIE") == 0) //Find MOVIE settings
			{
				_movieStart = std::stoi(next(isstream));
				_movieEnd = std::stoi(next(isstream));
				_movieWidth = std::stoi(next(isstream));
			}

			if (identifier.compare("SEED") == 0) //Find the random initial number
			{
				_seed = std::stoi(next(isstream));
			}

			if (identifier.compare("OUTMCSTEP") == 0) //boolean for output of system values as function of Monte-Carlo-Step
			{
				_boolOutSimulationSteps = TRUE;
			}
			
			if (identifier.compare("SPINCONFIG") == 0)
			{
				_boolSpinConfig = TRUE;
			}

			if (identifier.compare("ENERGY") == 0) //boolean for output of total energy
			{
				_boolE = TRUE;
			}

			if (identifier.compare("MAG") == 0) //boolean for output of magnetisation
			{
				_boolM = TRUE;
			}

			if (identifier.compare("MAGABS") == 0) //boolean for output of magnetisation
			{
				_boolMABS = TRUE;
			}

			if (identifier.compare("EACHSPIN") == 0) //boolean for output of heat capacity
			{
				_boolEachSpin = TRUE;
			}
		}
	}

	infile.close();
	set_all_parameters();

	return 1;
}

void Configuration::lattice_parameters(const std::string &identifier, std::istringstream &isstream)
{
	/**
	* Check identifier for lattice parameter and read if found.
	*
	* @param[in] identifier String to be compared to trigger read in.
	* @param[in] isstream Stream containing the rest of a line and with this possible parameters
	*/

	if (identifier.compare("LATTICE") == 0)
	{ //Find the lattice type
		std::string parameter = next(isstream);
		if (parameter.compare("sc") == TRUE) {
			_latticeType = simpleCubic;
		}
		if (parameter.compare("bcc") == TRUE) {
			_latticeType = bodyCenteredCubic;
		}
		if (parameter.compare("fcc") == TRUE) {
			_latticeType = faceCenteredCubic;
		}
	}

	if (identifier.compare("2DFCC") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularHexagonal;
		_boundaryConditions = helical;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
	}

	if (identifier.compare("FCCSTRIPES") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularStripe;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
		_latticeDimensions.push_back(std::stoi(next(isstream)));
		
		int boolFCCStripesPeriodicX = std::stoi(next(isstream));
		int boolFCCStripesPeriodicY = std::stoi(next(isstream));
		_boundaryConditions = openBound;
		
		if (boolFCCStripesPeriodicX == TRUE) {
			_boundaryConditions = periodicX;
		}
		if (boolFCCStripesPeriodicY == TRUE) {
			if (boolFCCStripesPeriodicX == TRUE) {
				_boundaryConditions = periodic;
			}
			else {
				_boundaryConditions = periodicY;
			}
		}
	}

	if (identifier.compare("2DFCCOB") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularHexagonal;
		_boundaryConditions = openBound;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
	}

	if (identifier.compare("2DFCCTRIANGLE") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularTriangular;
		_boundaryConditions = openBound;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
	}

	if (identifier.compare("2DHALFDISK") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularHalfDisk;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
	}

	if (identifier.compare("2DDISK") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularDisk;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
	}

	if (identifier.compare("ARROWHEAD") == 0)
	{
		_latticeDimensions.clear();
		_latticeType = triangularArrowHead;
		_latticeDimensions.push_back(std::stoi(next(isstream)));
		_latticeDimensions.push_back(std::stoi(next(isstream)));
		_latticeDimensions.push_back(std::stoi(next(isstream)));
	}

	if (identifier.compare("PERIODIC") == 0)
	{
		_boundaryConditions = periodic;
	}

	if (identifier.compare("h") == 0) //Find the Miller index h
	{
		_millerIndexes[0] = std::stoi(next(isstream));
	}

	if (identifier.compare("k") == 0) //Find the Miller index k
	{
		_millerIndexes[1] = std::stoi(next(isstream));
	}

	if (identifier.compare("l") == 0) //Find the Miller index l
	{
		_millerIndexes[2] = std::stoi(next(isstream));
	}

	if (identifier.compare("DIMX") == 0) //Find the lattice dimension DIMX
	{
		_latticeDimensions[0] = std::stoi(next(isstream));
	}

	if (identifier.compare("DIMY") == 0) //Find the lattice dimension DIMY
	{
		_latticeDimensions[1] = std::stoi(next(isstream));
	}

	if (identifier.compare("DIMZ") == 0) //Find the lattice dimension DIMZ
	{
		_latticeDimensions[2] = std::stoi(next(isstream));
	}

	if (identifier.compare("GYROMAGNETICFACTOR") == 0) //Find the magnetic moment
	{
		_magneticMoment = std::stof(next(isstream));
	}

	if (identifier.compare("a") == 0) //Find the magnetic moment
	{
		_latticeConstant = std::stof(next(isstream));
	}
}

void Configuration::spin_system_paremeters(const std::string &identifier, std::istringstream &isstream)
{
	/**
	* Check identifier for spin system parameter and read if found.
	*
	* @param[in] identifier String to be compared to trigger read in.
	* @param[in] isstream Stream containing the rest of a line and with this possible parameters
	*/

	if (identifier.compare("SYSTEM") == 0) //Find the system type e.g.: "Ising"
	{
		std::string parameter = next(isstream);
		if (parameter.compare("Heisenberg") == TRUE) {
			_spinSystem = Heisenberg;
		}
		if (parameter.compare("Ising") == TRUE) {
			_spinSystem = Ising;
		}
	}

	if (identifier.compare("INITIALSPIRAL") == 0) //Find the field loop
	{
		_initialSpiralek = { 1, 0, 0 };
		_initialSpiralR.x = std::stof(next(isstream));
		_initialSpiralR.y = std::stof(next(isstream));
		_initialSpiralR.z = std::stof(next(isstream));
		_initialSpiralI.x = std::stof(next(isstream));
		_initialSpiralI.y = std::stof(next(isstream));
		_initialSpiralI.z = std::stof(next(isstream));
		_initialSpiralek.x = std::stof(next(isstream));
		_initialSpiralek.y = std::stof(next(isstream));
		_initialSpiralek.z = std::stof(next(isstream));
		_initialSpiralLambda = std::stof(next(isstream));
	}

	if (identifier.compare("GAUSSIANSPINSAMPLING") == 0) //Find the magnetic moment
	{
		_gaussianSpinSamplingSigma = std::stof(next(isstream));
	}
}

void Configuration::energy_parameters(const std::string &identifier, std::istringstream &isstream)
{
	/**
	* Check identifier for energy parameter and read if found.
	*
	* @param[in] identifier String to be compared to trigger read in.
	* @param[in] isstream Stream containing the rest of a line and with this possible parameters
	*/

	if (identifier.compare("J1") == 0) // first neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 1 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}

	if (identifier.compare("J2") == 0) // second neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 2 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}

	if (identifier.compare("J3") == 0) // third neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 3 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}
	if (identifier.compare("J4") == 0) // fourth neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 4 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}
	if (identifier.compare("J5") == 0) // fifth neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 5 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}
	if (identifier.compare("J6") == 0) // sixth neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 6 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}
	if (identifier.compare("J7") == 0) // seventh neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 7 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}
	if (identifier.compare("J8") == 0) // eigth neighbour exchange energy
	{
		ExchangeEnergyStruct exchEnergy = { 0, 8 };
		exchEnergy.energyParameter = std::stof(next(isstream));
		_exchangeEnergies.push_back(exchEnergy);
	}

	if (identifier.compare("DM") == 0) //Find the Dzyaloshinskii-Moriya-Coupling
	{
		ExchangeEnergyStruct dmEnergy = {0, 1};
		dmEnergy.energyParameter = std::stof(next(isstream));
		_DMEnergies.push_back(dmEnergy);
	}

	if (identifier.compare("BIQ") == 0) //Find the first neighbour exchange coupling constant
	{
		_biQuadraticEnergy = std::stof(next(isstream));
	}

	if (identifier.compare("4SPIN") == 0) //Find the fifth neighbour exchange coupling constant
	{
		_fourSpinEnergy = std::stof(next(isstream));
	}

	if (identifier.compare("ANISOTROPY") == 0) //Find the anisotropies
	{
		UniaxialAnisotropyStruct anisotropy;
		anisotropy.energyParameter = std::stof(next(isstream));
		anisotropy.direction.x = std::stof(next(isstream));
		anisotropy.direction.y = std::stof(next(isstream));
		anisotropy.direction.z = std::stof(next(isstream));
		_uniaxialAnisotropyEnergies.push_back(anisotropy);
	}

	if (identifier.compare("DIPOL") == 0) //Find DipolDipol
	{
		_dipolEnergy = TRUE;
	}

	if (identifier.compare("JXY") == 0)
	{
		ModulatedExchangeEnergyStruct modulatedExchangeEnergy;
		modulatedExchangeEnergy.Jx = std::stof(next(isstream));
		modulatedExchangeEnergy.Jy = std::stof(next(isstream));
		modulatedExchangeEnergy.lambda = std::stof(next(isstream));
		modulatedExchangeEnergy.alpha = std::stof(next(isstream));
		modulatedExchangeEnergy.modulationNumber = std::stoi(next(isstream));
		_modulatedExchangeEnergies.push_back(modulatedExchangeEnergy);
	}

	if (identifier.compare("ANISOTROPYMOD") == 0) //Find the anisotropies
	{
		ModulatedAnisotropyEnergyStruct modAnisotropy;
		modAnisotropy.energyParameter = std::stof(next(isstream));
		modAnisotropy.lambda = std::stof(next(isstream));
		modAnisotropy.width = std::stof(next(isstream));
		modAnisotropy.direction.x = std::stof(next(isstream));
		modAnisotropy.direction.y = std::stof(next(isstream));
		modAnisotropy.direction.z = std::stof(next(isstream));
		modAnisotropy.modulationNumber = std::stoi(next(isstream));
		_modulatedAnisotropyEnergies.push_back(modAnisotropy);
	}

	if (identifier.compare("H") == 0) //Find the field loop
	{
		_magneticField.start = std::stof(next(isstream));
		_magneticField.end = std::stof(next(isstream));
		_magneticField.steps = std::stoi(next(isstream));
		_magneticField.direction.x = std::stof(next(isstream));
		_magneticField.direction.y = std::stof(next(isstream));
		_magneticField.direction.z = std::stof(next(isstream));
	}

	if (identifier.compare("TIP_START") == 0) //Find tip movement
	{
		_magneticTip.start.x = std::stof(next(isstream));
		_magneticTip.start.y = std::stof(next(isstream));
		_magneticTip.start.z = std::stof(next(isstream));
	}

	if (identifier.compare("TIP_END") == 0) //Find tip movement
	{
		_magneticTip.end.x = std::stof(next(isstream));
		_magneticTip.end.y = std::stof(next(isstream));
		_magneticTip.end.z = std::stof(next(isstream));
	}

	if (identifier.compare("TIP_DIR") == 0) //Find tip movement
	{
		_magneticTip.magnetizationDirection.x = std::stof(next(isstream));
		_magneticTip.magnetizationDirection.y = std::stof(next(isstream));
		_magneticTip.magnetizationDirection.z = std::stof(next(isstream));
	}

	if (identifier.compare("TIP_J") == 0)
	{
		_magneticTip.energyParameter = std::stof(next(isstream));
	}

	if (identifier.compare("TIP_MCSTEP") == 0)
	{
		_magneticTip.steps = std::stoi(next(isstream));
	}

	if (identifier.compare("TIP_MCWIDTH") == 0) //Find the random initial number
	{
		_magneticTip.stepWidth = std::stoi(next(isstream));
	}
}

void Configuration::set_all_parameters(void)
{
	/**
	* Create a single string _allParameters from all parameters to e.g. store in README later on
	*/

	_allParameters.clear();

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

	if (abs(_biQuadraticEnergy) > PRECISION)
	{
		_allParameters.append("   Biquadratic energy: " + std::to_string(_biQuadraticEnergy));
	}
	
	if (abs(_fourSpinEnergy) > PRECISION)
	{
		_allParameters.append("   4-spin energy: " + std::to_string(_fourSpinEnergy));
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
	if (_boolSpinConfig == TRUE)
	{
		_allParameters.append("   SpinConfig ");
	}

	_allParameters.append("   Movie: start " + std::to_string(_movieStart) + " end " + std::to_string(_movieEnd)
		+ " width " + std::to_string(_movieWidth));
	
	if (_boolOutSimulationSteps == TRUE)
	{
		_allParameters.append("   Monte_Carlo_steps ");
	}

	if (_boolE== TRUE)
	{
		_allParameters.append("   energy ");
	}

	if (_boolM == TRUE)
	{
		_allParameters.append("   magnetization ");
	}
	
	if (_boolMABS)
	{
		_allParameters.append("   absolute_magnetization ");
	}

	if (_boolEachSpin)
	{
		_allParameters.append("   each_spin ");
	}

	if (_boolWindingNumber)
	{
		_allParameters.append("   winding_number ");
	}
}

void Configuration::copy_configuration_file(std::string fname)
{
	if (!_confile.empty())
	{
		std::fstream stream1;
		stream1.open(fname, std::fstream::out);
		std::fstream stream2;
		std::string _parameter;
		char _cstring[256];
		stream2.open(_confile, std::fstream::in);
		if (!stream2.good())
		{
			std::cout << "The configuration file does not exist!" << std::endl;
		}
		while (!stream2.eof())
		{
			stream2.getline(_cstring, sizeof(_cstring));
			_parameter = _cstring;
			stream1 << _parameter << std::endl;
		}
		stream2.close();
		stream1.close();
	}
}

void Configuration::show_parameters(void)
{
	std::cout << "The following parameters have been defined in " << _confile << ": " << std::endl;
	std::cout << _allParameters << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}

void Configuration::determine_outputfolder_needed(void)
{
	_boolOutput = FALSE;

	if ( (_boolSpinConfig == TRUE) || (_boolE == TRUE) || (_boolM == TRUE) || (_boolMABS == TRUE)
		|| (_boolWindingNumber == TRUE) || (_boolNCMR == TRUE) )
	{
		_boolOutput = TRUE;
	}
	
	if (_movieStart > -1 && _movieEnd > -1 && _movieEnd > _movieStart && _movieWidth > 0)
	{
		_boolOutput = TRUE;
	}

	if (_numEigenStates > 0)
	{
		_boolOutput = TRUE;
	}
}

std::string Configuration::next(std::istringstream &isstream)
{
	std::string parameter = "";
	isstream >> parameter;
	return parameter;
}