/*
* Setup.cpp
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
*
* This class is intended to set up lattice, spin configuration and Hamiltonian object and energy objects.
* Furthermore it can be used to manipulate energy parameters during a simulation like for example the tip
* position or strength and direction of the external magnetic field.
*/

#include "Setup.h"

// forward and additional includes
#include "Configuration.h"

#include "Lattice.h"
#include "SpinOrientationHeisenberg.h"
#include "SpinOrientationHeisenbergRestrictedCone.h"
#include "SpinOrientationIsing.h"

#include "Energy.h"
#include "Hamiltonian.h"
#include "ExchangeInteraction.h"
#include "HexagonalAnisotropyEnergy.h"
#include "UniaxialAnisotropyEnergy.h"
#include "ModulatedAnisotropyEnergy.h"
#include "DMInteraction.h"
#include "FourSpinInteraction.h"
#include "BiquadraticInteraction.h"
#include "DipolarInteraction.h"
#include "ZeemanEnergy.h"
#include "Tip.h"
#include "ModulatedExchangeInteraction.h"
#include "ExchangeInteractionDefect.h"
#include "DMInteractionDefect.h"
#include "UniaxialAnisotropyEnergyDefect.h"
#include "EnergyObservable.h"
#include "MagnetisationObservable.h"
#include "AbsoluteMagnetisationObservable.h"
#include "NCMRContrastObservable.h"
#include "WindingNumber.h"
#include "Measurement.h"

Setup::Setup(QSharedPointer<Configuration> config)
{
	_config = config;
}

Setup::~Setup()
{
}

void Setup::create_crystal_lattice(void)
{
	/**
	* Creation of crystal lattice. Lattice configuration is read in if file was specified in _config.
	* Otherwise lattice is set up according to lattice parameters specified in _config
	*/

	_lattice = QSharedPointer<Lattice>(new Lattice(_config->_latticeType, _config->_latticeDimensions, 
		_config->_millerIndexes, _config->_boundaryConditions));

	_lattice->create_lattice();

	std::cout << "Lattice was created." << std::endl
		 << "---------------------------------------------" << std::endl << std::endl;
}

void Setup::read_crystal_lattice(std::string fname)
{
	/**
	* Read existing crystal lattice from text file. The text file should contain four columns.
	* The first column is just an index and irrelevant (could be set with abitrary numbers).
	* Second, third and fourth column should provide x,y,z coordinates of lattice sites.
	*
	* @param[in] fname The name of lattice text file.
	*/

	_lattice = QSharedPointer<Lattice>(new Lattice(_config->_latticeType, _config->_latticeDimensions,
		_config->_millerIndexes, _config->_boundaryConditions));

	_lattice->read_lattice(fname);
	_lattice->neighbor_distances();
	_lattice->assign_neighbors();
}

void Setup::create_crystal_lattice_from_mask(void)
{
	/**
	* Create a lattice with a boundary shape which is specified by a bitmap mask.
	*/

	_lattice = QSharedPointer<Lattice>(new Lattice(_config->_latticeType, _config->_latticeDimensions,
		_config->_millerIndexes, _config->_boundaryConditions));
	_lattice->create_mask_read_in_shape(_config->_latticeMaskParameters);
}

void Setup::create_spin_orientation(std::shared_ptr<RanGen> ranGen)
{
	/**
	* Creation of the spin orientation. Spin configuration is read in if file is specified in _config.
	* Otherwise spin system and initial spin configuration is set up according to parameters specified in
	* _config.
	* Note that this function should only be called AFTER create_crystal_lattice()
	*
	* @param[in] ranGen Pseudo random number generator
	*/

	if (!_lattice)
	{
		std::cout<<"Error in Setup::create_spin_orientation. Spin orientation is initialized before creation"
			"of crystal lattice. Program exit" << std::endl;
		exit(0);
	}
	else
	{
		// get number of lattice sites to ensure the same number of spins
		int numberAtoms = _lattice->get_number_atoms();
		
		switch (_config->_spinSystem) 
		{
		case Heisenberg:
		{
			// Create Heisenberg type spin configuration
			if (abs(_config->_gaussianSpinSamplingSigma) > PRECISION)
			{
				_spinOrientation = QSharedPointer<SpinOrientation>( 
					new SpinOrientationHeisenbergRestrictedCone(numberAtoms, ranGen, 
					_config->_gaussianSpinSamplingSigma));
			}
			else
			{
				_spinOrientation = QSharedPointer<SpinOrientation>( 
					new SpinOrientationHeisenberg(numberAtoms, ranGen));
			}
		} break;
		case Ising:
		{
			// Create Ising type spin configuration
			_spinOrientation = QSharedPointer<SpinOrientation>( 
				new SpinOrientationIsing(numberAtoms, ranGen));
		} break;
		}

		if (_config->_initialSpiralLambda > 0)
		{
			// set a spin spiral configuration
			_spinOrientation->set_spin_spiral(_config->_initialSpiralR, _config->_initialSpiralI,
				_config->_initialSpiralek, _config->_initialSpiralLambda, _lattice.data());
			std::cout << "Spin-Spiral state is set as initial configuration." << std::endl;
		}
		else
		{
			// random initial spin configuration
			_spinOrientation->random_orientation();
			std::cout << "Random spin state is created as initial configuration." << std::endl;
		}
		

		std::cout << "Spins were created." << std::endl
			 << "---------------------------------------------" << std::endl;
	}
}

void Setup::setup_measurement(void)
{
	/**
	* Setup observable objects and a measurement object.
	*/

	int numberMeasurements = 1; // arbitrary choice, changed later anyway

	if (_config->_doEnergyOutput)
	{
		observables.push_back(std::make_shared<EnergyObservable>(numberMeasurements, _hamilton,
			_spinOrientation->get_number_atoms(), _config->_doSpinResolvedOutput));
	}

	if (_config->_doMagnetisationOutput)
	{
		observables.push_back(std::make_shared<MagnetisationObservable>(numberMeasurements, 
			_spinOrientation.data(), _config->_doSpinResolvedOutput));
	}

	if (_config->_doAbsoluteMagnetisationOutput)
	{
		observables.push_back(std::make_shared<AbsoluteMagnetisationObservable>(numberMeasurements, 
			                  _spinOrientation.data()));
	}

	if (_config->_doNCMROutput)
	{
		observables.push_back(std::make_shared<NCMRContrastObservable>(numberMeasurements,
			_spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms(), 
			_lattice->get_neighbor_array(1), _lattice->get_number_nth_neighbors(1)));
	}

	if (_config->_doWindingNumberOutput == TRUE)
	{
		observables.push_back(std::make_shared<WindingNumber>(numberMeasurements, 
			_spinOrientation->get_spin_array(),	_lattice->get_skN_cells(), _lattice->get_skN_cell_number()));
	}

	// create measurement object. this will be used for all measurements later on in the programme.
	_measurement = std::make_shared<Measurement>(observables);
}

void Setup::setup_hamiltonian(void)
{
	/**
	* Setup Hamiltonian object and all included energy objects.
	*/

	// set up the energies included in the Hamiltonian
	setup_energies(); 

	// setup Hamilton object
	_hamilton = QSharedPointer<Hamiltonian>(new Hamiltonian(_energies, _lattice->get_number_atoms()));

	std::cout << "Hamiltonian was created." << std::endl;
}

void Setup::setup_energies(void)
{
	/**
	* Setup energy objects according to parameters given in Config* object.
	*/

	setup_exchange_interaction();

	setup_DM_interaction();

	setup_biquadratic_interaction();

	setup_four_spin_interaction();

	setup_anisotropy_energy();

	setup_dipol_interaction();

	setup_modulated_exchange_interaction();

	setup_modulated_anisotropy_energy();

	setup_zeeman_energy();

	setup_magnetic_tip();

	setup_exchange_defects();

	setup_dm_defects();

	setup_anisotropy_defects();
}

void Setup::set_magnetic_field(Threedim direction)
{
	/**
	* Set direction of magnetic field.
	*
	* @param[in] direction Direction of magnetic field.
	*/

	if (_zeemanEnergy)
	{
		_zeemanEnergy->set_direction(direction);
	}
}

void Setup::set_magnetic_field(double H)
{
	/**
	* Set magnetic field strength.
	*
	* @param[in] H Strength of magnetic field.
	*/

	if (_zeemanEnergy)
	{
		_zeemanEnergy->set_energy_parameter(H);
	}
}

void Setup::set_tip_position(Threedim position)
{
	/**
	* Set a tip position.
	*
	* @param[in] position Position of tip
	*/

	if (_tipEnergy)
	{
		_tipEnergy->set_position(position);
	}
}

void Setup::set_tip_strength(double energyParam)
{
	/**
	* Set strength of magnetic tip.
	*
	* @param[in] energyParam Strength of tip
	*/

	if (_tipEnergy)
	{
		_tipEnergy->set_energy_parameter(energyParam);
	}
}

void Setup::set_tip_direction(Threedim direction)
{
	/**
	* Set tip magnetic field direction.
	*
	* @param[in] direction Direction of tip field.
	*/

	if (_tipEnergy != NULL)
	{
		_tipEnergy->set_direction(direction);
	}
}

void Setup::setup_exchange_interaction(void)
{
	/**
	Setup of exchange energies.
	*/

	for (std::vector<ExchangeEnergyStruct>::iterator it = _config->_exchangeEnergies.begin();
		it != _config->_exchangeEnergies.end(); ++it)
	{
		if (fabs(it->energyParameter) > PRECISION)
		{
			int numberNeighbors = _lattice->get_number_nth_neighbors(it->order);
			int* neighborPtr = _lattice->get_neighbor_array(it->order);
			std::stringstream stream;
			stream << it->order;
			_energies.push_back(std::make_shared<ExchangeInteraction>(_spinOrientation->get_spin_array(),
				it->energyParameter, neighborPtr, numberNeighbors, stream.str()));
		}
	}
}

void Setup::setup_DM_interaction(void)
{
	/*
	Setup of Dzyaloshinskii-Moriya energy.
	*/
	for (auto it = _config->_DMEnergies.begin(); it != _config->_DMEnergies.end(); ++it)
	{
		if (fabs(it->energyParameter) > PRECISION)
		{
			_energies.push_back(std::make_shared<DMInteraction>(_spinOrientation->get_spin_array(),
				it->energyParameter, Threedim{0, 0, 1}, _config->_dmType, _lattice.data(), it->order));
		}
	}
}

void Setup::setup_biquadratic_interaction(void)
{
	/**
	Setup of biquadratic energy.
	*/

	if (fabs(_config->_biQuadraticEnergy) > PRECISION)
	{
		int numberNeighbors = _lattice->get_number_nth_neighbors(1);
		int* neighborPtr = _lattice->get_neighbor_array(1);
		_energies.push_back(std::make_shared<BiquadraticInteraction>(_spinOrientation->get_spin_array(),
			_config->_biQuadraticEnergy, neighborPtr, numberNeighbors));
	}
}

void Setup::setup_four_spin_interaction(void)
{
	/**
	Setup of 4-spin energy.
	*/

	if (fabs(_config->_fourSpinEnergy) > PRECISION) // 4-spin interaction energy setup
	{
		if (_lattice->get_four_spin_cells() == NULL)
		{
			std::cout << "error. fourSpincells not set. end program!" << std::endl;
		}
		_energies.push_back(std::make_shared<FourSpinInteraction>(_spinOrientation->get_spin_array(),
							_config->_fourSpinEnergy, _lattice->get_four_spin_cells(), 
							_lattice->get_number_four_spin_cells_per_atom()));
	}
}

void Setup::setup_anisotropy_energy(void)
{
	/**
	Setup of anisotropy energies.
	*/

	for (std::vector<UniaxialAnisotropyStruct>::iterator it = _config->_uniaxialAnisotropyEnergies.begin();
		it != _config->_uniaxialAnisotropyEnergies.end(); ++it)
	{
		if (abs(it->energyParameter) > PRECISION)
		{
			_energies.push_back(std::make_shared<UniaxialAnisotropyEnergy>(_spinOrientation->get_spin_array(),
				it->energyParameter, it->direction));
		}
	}

	double sum = 0.;
	for (int i = 0; i < 3; i++)
	{
		sum += _config->_hexagonalAnisotropyEnergies[i];
	}
	if (fabs(sum) > PRECISION)
	{
		_energies.push_back(std::make_shared<HexagonalAnisotropyEnergy>(_spinOrientation->get_spin_array(),
			_config->_hexagonalAnisotropyEnergies[0], _config->_hexagonalAnisotropyEnergies[1],
			_config->_hexagonalAnisotropyEnergies[2]));
	}
}

void Setup::setup_dipol_interaction(void)
{
	/**
	Setup of dipolar interaction.
	*/

	if (_config->_dipolEnergy == TRUE) // dipol-dipol energy setup
	{
		_energies.push_back(std::make_shared<DipolarInteraction>(_spinOrientation->get_spin_array(), 
			_config->_magneticMoment, _config->_latticeConstant, _lattice.data()));
	}
}

void Setup::setup_modulated_exchange_interaction(void)
{
	/**
	Setup of spatially modulated exchange energy.
	*/

	for (std::vector<ModulatedExchangeEnergyStruct>::iterator it = _config->_modulatedExchangeEnergies.begin();
		it != _config->_modulatedExchangeEnergies.end(); ++it)
	{
		_energies.push_back(std::make_shared<ModulatedExchangeInteraction>(_spinOrientation->get_spin_array(),
								_lattice.data(), it->Jx, it->Jy, it->lambda, it->alpha, it->modulationNumber));
	}
}

void Setup::setup_modulated_anisotropy_energy(void)
{
	/**
	Setup of spatially modulated anisotropy energy.
	*/

	for (std::vector<ModulatedAnisotropyEnergyStruct>::iterator it = _config->_modulatedAnisotropyEnergies.begin();
		it != _config->_modulatedAnisotropyEnergies.end(); ++it)
	{
		_energies.push_back(std::make_shared<ModulatedAnisotropyEnergy>(_spinOrientation->get_spin_array(),
			it->energyParameter, it->direction, _lattice.data(), it->lambda, it->width, it->modulationNumber));
	}
}

void Setup::setup_zeeman_energy(void)
{
	/**
	Setup of Zeeman energy.
	*/

	if (!((fabs(_config->_magneticField.start) < PRECISION) &&
		(fabs(_config->_magneticField.end) < PRECISION)))
	{
		_zeemanEnergy = std::make_shared<ZeemanEnergy>(_spinOrientation->get_spin_array(),
			_config->_magneticField.start, _config->_magneticField.direction);
		_energies.push_back(_zeemanEnergy);
	}
}

void Setup::setup_magnetic_tip(void) 
{
	/**
	Setup of magnetic tip.
	*/

	double tipDistance = MyMath::norm(MyMath::difference(_config->_magneticTip.start, 
		_config->_magneticTip.end));
	if (tipDistance > PRECISION)
	{
		_tipEnergy = std::make_shared<Tip>(_spinOrientation->get_spin_array(),
			_config->_magneticTip.energyParameter, _config->_magneticTip.start,
			_config->_magneticTip.magnetizationDirection, _lattice.data());
		_energies.push_back(_tipEnergy);	
	}
}

void Setup::setup_exchange_defects(void)
{
	/**
	* Setup exchange energy defects.
	*/

	for (auto it = _config->_exchangeDefects.begin(); it != _config->_exchangeDefects.end(); ++it)
	{
		int numberNeighbors = _lattice->get_number_nth_neighbors(it->first);
		int* neighborPtr = _lattice->get_neighbor_array(it->first);
		std::stringstream stream;
		stream << it->first;
		_energies.push_back(std::make_shared<ExchangeInteractionDefect>(_spinOrientation->get_spin_array(),
			neighborPtr, numberNeighbors, it->second, stream.str()));
	}
}

void Setup::setup_dm_defects(void)
{
	/**
	* Setup DM-Interaction defects.
	*/

	for (auto it = _config->_dmDefects.begin(); it != _config->_dmDefects.end(); ++it)
	{
		int numberNeighbors = _lattice->get_number_nth_neighbors(it->first);
		int* neighborPtr = _lattice->get_neighbor_array(it->first);
		std::stringstream stream;
		stream << it->first;
		_energies.push_back(std::make_shared<DMInteractionDefect>(_spinOrientation->get_spin_array(),
			Threedim{ 0, 0, 1 }, it->second, _config->_dmType, _lattice.data(), it->first));
	}
}

void Setup::setup_anisotropy_defects(void)
{
	/**
	* Setup uniaxial anisotropy energy defects.
	*/

	if (_config->_anisotropyDefects.empty()) {
		return;
	}

	_energies.push_back(std::make_shared<UniaxialAnisotropyEnergyDefect>(_spinOrientation->get_spin_array(),
			_config->_anisotropyDefects));
}
