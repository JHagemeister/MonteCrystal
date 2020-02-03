/*
* SimulationProgram.cpp
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
* This is the main class of the simulation software for the modelling of magnetic crystal systems. From here,
* the course of the program is managed. This includes: The creation of lattice, spin configuration, 
* Hamiltonian, start of Monte Carlo or Spin Dynamics simulation and output of system information.
*
*/

#include "SimulationProgram.h"

// from forward declarations and further includes
#include "Configuration.h"
#include "Lattice.h"
#include "SpinOrientation.h"
#include "Setup.h"
#include "Mersenne.h"

#include <QMutex>

#include "Hamiltonian.h"
#include "Energy.h"
#include "Measurement.h"
#include "WindingNumber.h"
#include "Functions.h"
#include "Metropolis.h"
#include "LandauLifshitzGilbert.h"
#include "ExcitationModeSolver.h"
#include "Converger1.h"

#include <fstream>
#include <iostream>

SimulationProgram::SimulationProgram(QDir &workfolder, const std::shared_ptr<Configuration> &config, 
	QMutex* mutex, int* terminateThread, QSharedPointer<Lattice> lattice,
	QSharedPointer<SpinOrientation> spinOrientation)
{
	/**
	* Constructor for the main class of simulation software.
	*
	* @param[in] workfolder This is the root folder for the output of text files containing information about
	*                       the simulation and the simulation results. This folder will contain: a "README" 
	*                       textfile with the history of all simulations, and a folder called "Data" which
	*                       contains a folder with a unique ID for each simulation.
	* @param[in] config This object contains all the necessary parameters needed for a simulation.
	* @param[in] mutex This object is needed to avoid simultaneous access to data as lattice and spin 
	*                  configuration from simulation thread and GUI thread.
	* @param[in] terminateThread This is a boolean variable that is shared between simulation and GUI thread. 
	*                            A running simulation will be aborted when set to true.
	* @param[in] lattice An already existing Lattice can be passed and will be used for the simulation. Ensure 
	*                    compatibility with spinOrientation.
	* @param[in] spinOrientation An already existing SpinOrientation can be passed and will be used for the
	*                            simulation. Ensure compatibility with lattice.
	*/

	// initialization of member variables
	_workFolder = workfolder;
	_config = config;
	_lattice = lattice;
	_spinOrientation = spinOrientation;
	_mutex = mutex;
	_terminateThread = terminateThread;
}

SimulationProgram::~SimulationProgram()
{
	/**
	 *Destructor: No work needs to be done since no variables are created with "new" on the heap.
	 */
}

void SimulationProgram::main(void)
{
	/**
	* Only public method. This method manages the course of the simulation program.
	*/

	// Create pseudo random number generator
	std::shared_ptr<RanGen> ranGen = std::make_shared<Mersenne>(_config->_seed);

	// Create object that manages lattice, spin configuration and Hamiltonian with energy objects
	auto setup = std::make_shared<Setup>(_config);

	int boolNewLattice = FALSE;
	
	if (_config->_programType == readLatticeConfiguration)
	{
		_lattice.clear();
		setup->read_crystal_lattice(_config->_storageFname);
		boolNewLattice = TRUE;
	}
	else if (_config->_programType == latticeMaskRead)
	{
		_lattice.clear();
		setup->create_crystal_lattice_from_mask();
		boolNewLattice = TRUE;
	}
	// Create crystal lattice if needed
	else if (_lattice.data() != NULL)
	{
		// lattice and spin orientaiton from previous simulation will be used
		setup->_lattice = _lattice;
	}
	else
	{
		// new lattice is created 
		setup->create_crystal_lattice();
		boolNewLattice = TRUE;
	}

	int boolNewSpins = FALSE;

	if (_config->_programType == readSpinConfiguration)
	{
		int numberLines = Functions::get_num_lines(_config->_storageFname);
		if (numberLines == setup->_lattice->get_number_atoms())
		{
			setup->create_spin_orientation(ranGen);
			setup->_spinOrientation->read_spin_configuration(_config->_storageFname);
			boolNewSpins = TRUE;
		}
	}
	else if (_spinOrientation.data() != NULL &&
		_spinOrientation->get_number_atoms() == setup->_lattice->get_number_atoms())
	{
		setup->_spinOrientation = _spinOrientation;
	}
	else
	{
		_spinOrientation.clear();
		// new spin orientation is created
		setup->create_spin_orientation(ranGen);
		boolNewSpins = TRUE;
	}

	// Send current lattice structure and spin configuration to GUI thread for graphical output during 
	// simulation and as cache for output until next simulation starts.
	emit send_crystal_structure(setup->_lattice, setup->_spinOrientation, boolNewSpins+boolNewLattice);
	if (boolNewSpins + boolNewLattice)
	{
		_config->_dmDefects.clear();
		_config->_exchangeDefects.clear();
		_config->_anisotropyDefects.clear();
	}

	// Setup Hamiltonian with the energies specified in the member object _config
	setup->setup_hamiltonian();

	emit send_hamiltonian(setup->_hamilton);

	// adjust camera of openGLWidget in GUI thread to current crystal structure
	if (boolNewLattice == TRUE)
	{
		emit send_camera_adjustment_request();
	}
	emit send_repaint_request();

	// Manage output of system information and start simulation 
	switch (_config->_programType)
	{
	case saveLatticeConfiguration:
		Functions::save(setup->_lattice->get_lattice_coordinate_array(), setup->_lattice->get_number_atoms(),
			_config->_storageFname);
		break;
	case saveSpinConfiguration:
		Functions::save(setup->_spinOrientation->get_activity_list(),
			setup->_spinOrientation->get_spin_array(),
			setup->_spinOrientation->get_number_atoms(), _config->_storageFname);
		break;
	case latticeSiteEnergies: 
		// save energies resolved to lattice sites for a read in spin orienation. 
		// No Monte Carlo or Spin Dynamics simulation is done.
		save_lattice_site_energies(setup, _config->_storageFname);
		break;

	case latticeSiteWindingNumber: 
		// save topological charge resolved to lattice sites for a read in spin orienation.
		// No Monte Carlo or Spin Dynamics simulation done.
		save_local_winding_number(setup, _config->_storageFname);
		break;
	case EigenFrequency:
	case temperatureMagneticFieldLoop:
		// start simulation in temperature and magnetic field loop
		std::cout << "---------------------------------------------" << std::endl;
		std::cout << "Metropolis calculation starts." << std::endl;
		// start temperature and magnetic field loop
		temperature_magnetic_field_loop(setup, ranGen, _config->_doOutput); 
		break;

	case spinSeebeck:
		// Spin-Seebeck simulation (simulation with a temperature gradient along the crystal)
		std::cout << "---------------------------------------------" << std::endl;
		std::cout << "Metropolis calculation with temperature gradient starts." << std::endl;
		// start Spin-Seebeck simulation
		spin_seebeck(setup, ranGen, _config->_doOutput);
		break;

	case tipMovement:
		// Simulation with a magnetic tip to simulate influence of tip of scanning tunneling microscope
		std::cout << "---------------------------------------------" << std::endl;
		std::cout << "Metropolis calculations with magnetic tip starts." << std::endl;		
		// start simulation with a magnetic tip
		tip_movement(setup, ranGen);
		break;
	case Experiment01:
		experiment01(setup, ranGen, _config->_doOutput);
		break;
	}

	// Send notification about end of simulation program
	emit send_finished(); 
}

void SimulationProgram::save_lattice_site_energies(const std::shared_ptr<Setup> &setup, std::string fname)
{
	/**
	 * Save the energies resolved for each lattice site.
	 *
	 * @param[in] setup The information about lattice, spin configuration and Hamiltonian.
	 * @param[in] fname The file name for the storage of the lattice energies.
	*/

	// temporary buffer
	std::stringstream tmpstream;
	
	// obtain energies included in Hamiltonian
	std::vector<std::shared_ptr<Energy>> energies = setup->_energies; 
	
	tmpstream << "index ";
	// iterate over all energies and get string identifications/names of the respective energies
	for (int i = 0; i < energies.size(); ++i)
	{
		tmpstream << energies[i]->get_string_id(); // obtain name of respective energy
	}
	tmpstream << "\n";
	
	// iterate over all lattice sites and calculate all energy contributions
	for (int i = 0; i < setup->_lattice->get_number_atoms(); ++i)
	{
		// atom/lattice site index
		tmpstream << i << " ";
		for (int j = 0; j < energies.size(); ++j)
		{
			// energy at atom i 
			tmpstream << setup->_hamilton->single_part_energy(j, i) << " "; 
		}
		// avoid line break at end of last line
		if (i < setup->_lattice->get_number_atoms() - 1) 
		{
			tmpstream << "\n";
		}
	}

	// output into file:
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << tmpstream.str();
	filestr.close();
	std::cout << "Energies resolved to lattice sites written.";
	std::cout << "---------------------------------------------" << std::endl;
}

void SimulationProgram::save_local_winding_number(const std::shared_ptr<Setup> &setup, std::string fname)
{
	/**
	* Save the topological charge/winding number resolved for each lattice site.
	*
	* @param[in] setup The information about lattice, spin configuration and Hamiltonian. (Only lattice inf-
	*                  ormation and spin configuration needed for calculation of topological charge.)
	* @param[in] fname The file name for the storage of the topological charge.
	*/

	// temporary buffer
	std::stringstream tmpstream;
	
	// The topological charge is calculated locally for a set of three neighboring spins. cells contains 
	// 3*cellNum entries where cellNum is the number of cells consisting of three neighboring lattice sites. 
	TopologicalChargeCell* cells = setup->_lattice->get_skN_cells(); 
	int cellNum = setup->_lattice->get_skN_cell_number();

	// Calculate winding number/topological charge for the given spin configuration
	auto windNum = std::make_shared<WindingNumber>(1, setup->_spinOrientation->get_spin_array(), cells, 
		cellNum);
	windNum->evaluate_local_winding_number();

	// header for the file in which the winding number will be stored
	tmpstream << "x y WindN \n ";
	
	// pointer to lattice coordinate array
	Threedim* latticeCoordArr = setup->_lattice->get_lattice_coordinate_array(); 
	
	// the center of three neighboring lattice sites for which the topological charge is calculated
	Threedim center = { 0, 0, 0 };
	
	// iterate over all cells of three neighboring lattice sites for which´topological charge was calculated
	for (int i = 0; i < cellNum; ++i)
	{
		// average the three position vectors of a cell
		center = MyMath::add(latticeCoordArr[cells[i].i], latticeCoordArr[cells[i].j]);
		center =	MyMath::add(center, latticeCoordArr[cells[i].k]);
		center = MyMath::mult(center, 1/(double)3);

		// store center of cell and corresponding topological charge
		tmpstream << center.x << " " << center.y  << " " << windNum->get_local_winding_number()[i];
		
		// avoid line break in last line
		if (i < cellNum - 1)
		{
			tmpstream << "\n";
		}
	}

	// output into file:
	std::fstream filestr;
	filestr.open(fname, std::ios::out);
	filestr << tmpstream.str();
	filestr.close();
	std::cout << "Winding number resolved to lattice sites written. programme exit.";
	std::cout << "---------------------------------------------" << std::endl;
}


void SimulationProgram::temperature_magnetic_field_loop(const std::shared_ptr<Setup> &setup,
	std::shared_ptr<RanGen> ranGen, int boolFolderOutput)
{
	/**
	* Performs Monte Carlo or Spin Dynamics simulation within a temperature and magnetic field loop.
	*
	* @param[in] setup The information about lattice, spin configuration and Hamiltonian.
	* @param[in] ranGen Pseudo random number generator.
	*/

	// unique simulation identity number
	std::string simID = "";

	// determine unique identity number and created unique folder for simulation output
	QDir simFolder = create_unique_simulation_folder(simID, boolFolderOutput);
	// folder for simulation ouput
	std::string outputFolder = simFolder.absolutePath().toStdString() + "/SIMULATION/";

	std::string fname = "";
	std::stringstream stringStream;

	// measurement object that manages and contains all "measurement" information about simulation
	setup->setup_measurement();
	auto measurement = setup->_measurement;

	// number of "measurements" done during one step of temperature and magnetic field loop
	int numMeasurements = _config->_simulationSteps / _config->_outputWidth;

	// provide measurement object with number of measurements that shall be done during one step of 
	// temperature and magntic field loop
	measurement->set_number_measurements(numMeasurements);

	// Create simulation object
	std::shared_ptr<SimulationMethod> simulation;
	switch (_config->_simulationType)
	{
	case metropolis:
		// Metropolis type Monte Carlo simulation
		// Note that temperature is arbitrarily set to 1 since it will be set in temperature loop anyway
		simulation = std::make_shared<Metropolis>(setup->_spinOrientation.data(), _config->_simulationSteps, 
			1, setup->_hamilton, ranGen, this);
		break;
	case landauLifshitzGilbert:
		// Landau Lifshitz Gilbert type spin dynamics simulation
		// Note that temperature is arbitrarily set to 1 since it will be set in temperature loop anyway
		simulation = std::make_shared<LandauLifshitzGilbert>(setup->_spinOrientation.data(),
			_config->_simulationSteps, 1, setup->_hamilton, ranGen, _config->_LLG_timeWidth,
			_config->_LLG_dampingParameter, _config->_magneticMoment, this);
		break;
	case converger1:
		simulation = std::make_shared<Converger1>(setup->_spinOrientation.data(),
			_config->_simulationSteps, 1, setup->_hamilton, ranGen, this);
		break;
	}

	//  Temperatures for temperature loop
	std::vector<double> temperature = MyMath::linspace(_config->_temperatureStart, _config->_temperatureEnd,
		_config->_temperatureSteps);

	// Magnetic fields for magnetic field loop
	std::vector<double> magneticField = MyMath::linspace(_config->_magneticField.start,
		_config->_magneticField.end, _config->_magneticField.steps);


	// main loop of this method where the simulation takes place
	for (std::vector<double>::iterator fieldPtr = magneticField.begin(); fieldPtr != magneticField.end();
		++fieldPtr)
	{
		std::cout << std::endl << "external magnetic field: " << *fieldPtr << std::endl;
		
		// set magnetic field for simulation
		setup->set_magnetic_field(*fieldPtr);
		
		for (std::vector<double>::iterator tempPtr = temperature.begin(); tempPtr != temperature.end(); 
			++tempPtr)
		{
			std::cout << std::endl << "temperature: " << *tempPtr << std::endl;
			
			// set temperature for simulation
			simulation->set_temperature(*tempPtr);

			// send information about magnetic field and temperature to GUI thread
			emit send_simulation_info("<font size=5><i>B</i> = " + QString::number((*fieldPtr)/
				(_config->_magneticMoment*muBohr))+
				"T    <i>T</i> = " 	+ QString::number(*tempPtr)+"K");

			// basis file name for output during this step of the temperature and magnetic field loops
			fname = outputFolder;
			fname.append(simID);
			fname.append("_B_");
			fname.append(Functions::get_name((*fieldPtr)/(_config->_magneticMoment*muBohr)));
			fname.append("_T_");
			fname.append(Functions::get_name(*tempPtr));

			// run simulation
			simulation->run_simulation(_config->_uiUpdateWidth, measurement ,_config->_outputWidth, 
				_config->_movieStart, _config->_movieEnd, _config->_movieWidth, fname);

			// check for abortion of simulation
			_mutex->lock();
			if (*_terminateThread == 1)
			{
				_mutex->unlock();
				return; // abort current simulation by return
			}
			_mutex->unlock();

			// output of measurement information as a function of simulation steps
			if (_config->_doSimulationStepsOutput)
			{
				std::string header;
				switch (_config->_simulationType)
				{					
				case metropolis:
					header.append("MCStep");
					measurement->save_step_values(fname + "_observables", header, _config->_outputWidth);
					break;
				case landauLifshitzGilbert:
					header.append("t_[ps]");
					double width = _config->_outputWidth * _config->_LLG_timeWidth;
					measurement->save_step_values(fname + "_observables", header, width);
					break;
				}
			}

			// average mean values of observables over simulation steps
			stringStream.str("");
			stringStream.clear();
			// identify by temperature and magnetic field
			stringStream << *tempPtr << " " << (*fieldPtr)/ (_config->_magneticMoment*muBohr); 
			measurement->take_mean_values(stringStream.str(), *tempPtr); // mean values of observables

			// output of spin configuration at end of loop step
			if (_config->_doSpinConfigOutput)
			{ 
				// save spin configuration as text file
				Functions::save(setup->_spinOrientation->get_activity_list(), 
					setup->_spinOrientation->get_spin_array(),
					setup->_spinOrientation->get_number_atoms(), fname + "SpinConfigurationAtEnd");

				// save spin configuration as png image from GUI widget
				emit send_repaint_request();
				emit send_save_image_request(QString::fromStdString(fname + "SpinConfigurationAtEnd.png"));
			}
			// For all observables: clear storage and reset measurement index to 0.
			measurement->reset_observables_measurement_index(); 


			/////////////////////////////
			if (_config->_numEigenStates > 0)
			{
				eigen_frequency(setup, fname);
			}
			/////////////////////////////
		}
	}

	if (boolFolderOutput)
	{
		// output of mean values for T and H
		fname = outputFolder;
		fname.append(simID);
		fname.append("_MeanValues");
		measurement->save_mean_steps(fname, "T[K] B[T] ");
	}

	// Save information about the lattice used in the simulation to simulation folder
	save_lattice_information(setup->_lattice.data(), simFolder.absolutePath().toStdString() + "/SYSTEM/", simID, boolFolderOutput);
}

void SimulationProgram::spin_seebeck(const std::shared_ptr<Setup> &setup, std::shared_ptr<RanGen> ranGen, 
	int boolFolderOutput)
{
	/**
	* Simulates a magnetic system with a temperature gradient with a constant external magnetic field. 
	*
	* @param[in] setup The information about lattice, spin configuration and Hamiltonian.
	* @param[in] ranGen Pseudo random number generator.
	*/

	// unique simulation identity number
	std::string simID = "";

	// determine unique identity number and created unique folder for simulation output
	QDir simFolder = create_unique_simulation_folder(simID, boolFolderOutput);
	// folder for simulation ouput
	std::string outputFolder = simFolder.absolutePath().toStdString() + "/SIMULATION/";

	std::string fname = "";

	// measurement object that manages and contains all "measurement" information about simulation
	setup->setup_measurement();
	auto measurement = setup->_measurement;

	// number of "measurements" done during one step of temperature and magnetic field loop
	int numMeasurements = _config->_simulationSteps / _config->_outputWidth;

	// provide measurement object with number of measurements that shall be done during one step of 
	// temperature and magntic field loop
	measurement->set_number_measurements(numMeasurements);

	// Create simulation object
	std::shared_ptr<SimulationMethod> simulation;
	switch (_config->_simulationType)
	{
	case metropolis:
		// Metropolis type Monte Carlo simulation
		// Note that temperature is arbitrarily set to 1 since temperature gradient is set for simulation
		simulation = std::make_shared<Metropolis>(setup->_spinOrientation.data(), _config->_simulationSteps,
			1, setup->_hamilton, ranGen, this);
		break;
	case landauLifshitzGilbert:
		// Landau Lifshitz Gilbert type spin dynamics simulation
		// Note that temperature is arbitrarily set to 1 since temperature gradient is set for simulation
		simulation = std::make_shared<LandauLifshitzGilbert>(setup->_spinOrientation.data(),
			_config->_simulationSteps, 1, setup->_hamilton, ranGen, _config->_LLG_timeWidth,
			_config->_LLG_dampingParameter, _config->_magneticMoment, this);
		break;
	}
	
	// Spin-Seebeck effect is the behavior of magnetic systems with a temperature gradient along the system
	// minimum temperature of crystal
	double Tmin = _config->_temperatureStart;
	// maximum temperature of crystal
	double Tmax = _config->_temperatureEnd;
	simulation->set_temperature_gradient(Tmin, Tmax, _config->_temperatureGradientDirection, 
		setup->_lattice.data());

	// the magnetic field is constant over the magnetic system during the simulation
	double magneticField = _config->_magneticField.start;
	setup->set_magnetic_field(magneticField);

	// basis file name for output during simulation
	fname = outputFolder;
	fname.append(simID);
	fname.append("_B_");
	fname.append(Functions::get_name(magneticField/(_config->_magneticMoment*muBohr)));

	// run simulation
	simulation->run_simulation(_config->_uiUpdateWidth, measurement, _config->_outputWidth,
		_config->_movieStart, _config->_movieEnd, _config->_movieWidth, fname);

	// output of measurement information as a function of simulation steps
	if (_config->_doSimulationStepsOutput)
	{
		std::string header;
		switch (_config->_simulationType)
		{
		case metropolis:
			header.append("MCStep");
			measurement->save_step_values(fname + "Measurements", header, _config->_outputWidth);
			break;
		case landauLifshitzGilbert:
			header.append("t_[ps]");
			double width = _config->_outputWidth * _config->_LLG_timeWidth;
			measurement->save_step_values(fname + "Measurements", header, width);
			break;
		}
	}
	
	// Save information about the lattice used in the simulation to simulation folder
	save_lattice_information(setup->_lattice.data(), simFolder.absolutePath().toStdString() + "/SYSTEM/", simID, boolFolderOutput);
}

void SimulationProgram::tip_movement(const std::shared_ptr<Setup> &setup, std::shared_ptr<RanGen> ranGen)
{
	/**
	* Simulates a magnetic system in contact with a moving magnetic tip at a constant magnetic field and 
	* constant temperature. The magnetic tip is intended to simulate the effect of the tip of a scanning 
	* tunneling microscope.
	*
	* @param[in] setup The information about lattice, spin configuration and Hamiltonian.
	* @param[in] ranGen Pseudo random number generator.
	*/

	// unique simulation identity number
	std::string simID = "";

	// determine unique identity number and created unique folder for simulation output
	QDir simFolder = create_unique_simulation_folder(simID);
	// folder for simulation ouput
	std::string outputFolder = simFolder.absolutePath().toStdString() + "/SIMULATION/";

	std::string fname = "";
	std::stringstream stringStream;

	// measurement object that manages and contains all "measurement" information about simulation
	setup->setup_measurement();
	auto measurement = setup->_measurement;

	// number of "measurements" done during one step of temperature and magnetic field loop
	int numMeasurements = _config->_simulationSteps / _config->_outputWidth;

	// provide measurement object with number of measurements that shall be done during one step of 
	// temperature and magntic field loop
	measurement->set_number_measurements(numMeasurements);

	// constant temperature for simulation with magnetic tip
	double temperature = _config->_temperatureStart;

	// Create simulation object
	std::shared_ptr<SimulationMethod> simulation;
	switch (_config->_simulationType)
	{
	case metropolis:
		// Metropolis type Monte Carlo simulation
		simulation = std::make_shared<Metropolis>(setup->_spinOrientation.data(), _config->_simulationSteps,
			temperature, setup->_hamilton, ranGen, this);
		break;
	case landauLifshitzGilbert:
		// Landau Lifshitz Gilbert type spin dynamics simulation
		simulation = std::make_shared<LandauLifshitzGilbert>(setup->_spinOrientation.data(),
			_config->_simulationSteps, temperature, setup->_hamilton, ranGen, _config->_LLG_timeWidth,
			_config->_LLG_dampingParameter, _config->_magneticMoment, this);
		break;
	}

	setup->set_tip_strength(_config->_magneticTip.energyParameter);
	setup->set_tip_direction(_config->_magneticTip.magnetizationDirection);

	double steps = _config->_magneticTip.steps;
	Threedim tipPosition = _config->_magneticTip.start;
	Threedim tipStep = MyMath::difference(_config->_magneticTip.end, _config->_magneticTip.start);
	tipStep = MyMath::mult(tipStep, 1.0 / (steps - 1.0));

	// iterate over tip movement steps
	for (int i = 0; i <= steps; ++i)
	{
		//set new tip position
		setup->set_tip_position(tipPosition); 
		
		std::cout << "position set" << std::endl;

		// run simulation
		simulation->run_simulation(_config->_uiUpdateWidth, measurement, _config->_outputWidth, 0, 0, 0,
			"");

		// take the average of measurements
		stringStream.str("");
		stringStream << i;
		measurement->take_mean_values(stringStream.str(), temperature);
		measurement->reset_observables_measurement_index(); //set measurement index to 0 for next loop
		
		// every stepWidth tip movement steps store current spin configuration to file
		if ((i % _config->_magneticTip.stepWidth) == 0)
		{
			std::cout << tipPosition.x << " " << tipPosition.y << " " << tipPosition.z << std::endl;
			fname = outputFolder;
			fname.append(simID);
			fname.append("_TipMovement_SpinConfiguration_");
			stringStream.str("");
			stringStream.clear();
			stringStream << i;
			fname.append(stringStream.str());
			Functions::save(setup->_spinOrientation->get_activity_list(), 
				setup->_spinOrientation->get_spin_array(),
				setup->_spinOrientation->get_number_atoms(), fname);

			// save spin configuration as png image from GUI widget
			emit send_repaint_request();
			emit send_save_image_request(QString::fromStdString(fname + ".png"));
		}

		//evaluate new tip position for next step of loop
		tipPosition = MyMath::add(tipPosition, tipStep); 
	}

	// store measurements to a file; one average value for each tip step
	fname = outputFolder;
	fname.append(simID);
	fname.append("_MeanValues_TipMovement");
	measurement->save_mean_steps(fname, "TipMovementStep");

	save_lattice_information(setup->_lattice.data(), simFolder.absolutePath().toStdString() + "/SYSTEM/", simID);
}

void SimulationProgram::experiment01(const std::shared_ptr<Setup>& setup, std::shared_ptr<RanGen> ranGen, 
	int boolFolderOutput)
{
	/**
	* Experiment01: Intended for a 2D lattice within the (x,y)-plane. Constant temperature and constant 
	* magnetic field. Spins at edge of sample are rotated with given frequency.
	*
	* @param[in] setup The information about lattice, spin configuration and Hamiltonian.
	* @param[in] ranGen Pseudo random number generator.
	* @param[in] boolFolderOutput  0 not output, 1 output to simulation folder
	*/

	// unique simulation identity number
	std::string simID = "";

	// determine unique identity number and created unique folder for simulation output
	QDir simFolder = create_unique_simulation_folder(simID, boolFolderOutput);
	// folder for simulation ouput
	std::string outputFolder = simFolder.absolutePath().toStdString() + "/SIMULATION/";

	std::string fname = "";

	setup->_spinOrientation->set_all_sites_active();

	Threedim* latticeCoordArray = setup->_lattice->get_lattice_coordinate_array();
	int numberAtoms = setup->_lattice->get_number_atoms();
	Threedim min{ 0,0,0 }; // minimum x component of lattice needed
	Threedim max{ 0,0,0 }; //not needed for further calculations
	MyMath::min_max_threedim(latticeCoordArray, numberAtoms, min, max);

	std::vector<int> rotationSites; // indexes of lattice sites that shall be rotated with given frequency

	for (int i = 0; i < numberAtoms; ++i)
	{
		// rotate all spins at lattice sites with minimum x component
		if (fabs(latticeCoordArray[i].x - min.x) < PRECISION) 
		{
			rotationSites.push_back(i);
			// exclude the site from simulation-based update; will be updated "manually" 
			setup->_spinOrientation->set_inactive_site(i); 
		}
	}

	// Create simulation object
	std::shared_ptr<SimulationMethod> simulation;
	switch (_config->_simulationType)
	{
	case metropolis:
		// Metropolis type Monte Carlo simulation
		// Note that temperature is arbitrarily set to 1 since temperature gradient is set for simulation
		simulation = std::make_shared<Metropolis>(setup->_spinOrientation.data(), _config->_simulationSteps,
			1, setup->_hamilton, ranGen, this);
		break;
	case landauLifshitzGilbert:
		// Landau Lifshitz Gilbert type spin dynamics simulation
		// Note that temperature is arbitrarily set to 1 since temperature gradient is set for simulation
		simulation = std::make_shared<LandauLifshitzGilbert>(setup->_spinOrientation.data(),
			_config->_simulationSteps, 1, setup->_hamilton, ranGen, _config->_LLG_timeWidth,
			_config->_LLG_dampingParameter, _config->_magneticMoment, this);
		break;
	}

	// temperature of spin system
	double temperature = _config->_temperatureStart;
	simulation->set_temperature(temperature);

	// the magnetic field is constant over the magnetic system during the simulation
	double magneticField = _config->_magneticField.start;
	setup->set_magnetic_field(magneticField);

	// basis file name for output during simulation
	fname = outputFolder;
	fname.append(simID);
	fname.append("_B_");
	fname.append(Functions::get_name(magneticField / (_config->_magneticMoment*muBohr)));

	std::stringstream stream;

	Threedim rotSpin{ 0,0,1 }; // direction of spins that are set according to predefined rotation
	double omega = 2*Pi*_config->_experiment01.freq; // rotation frequency (2*pi*f) with f in [1/SimulationStep]

	for (int i = 1; i < _config->_simulationSteps + 1; ++i)
	{
		for (int j = 0; j < rotationSites.size(); ++j) // update rotation spins after each simulation step
		{
			setup->_spinOrientation->set_spin(rotSpin, rotationSites[j]);
		}

		_mutex->lock();
		simulation->simulation_step(); // perform a simulation step
		_mutex->unlock();

		// calculate direction of rotation spins for next simulation step (rotation in (x,z)-plane)
		rotSpin.z = cos(omega*i); 
		rotSpin.x = sin(omega*i);

		// update GUI after each _uiUpdateWidth simulation steps
		if ((i % _config->_uiUpdateWidth) == 0)
		{
			emit send_simulation_step("Step = " + QString::number(i));
			emit send_repaint_request();

			_mutex->lock();
			if (*_terminateThread == 1) // check for termination request of simulation
			{
				// terminate simulation by setting iteration index i larger than iteration range
				i = _config->_simulationSteps + 1;
			}
			_mutex->unlock();
		}

		// save spin configuration to png and corresponding text files for requested simulation steps
		if ((i >= _config->_movieStart) && (i <= _config->_movieEnd))
		{
			if (((i - _config->_movieStart) % _config->_movieWidth) == 0) 
			{
				stream.str("");
				stream.clear();
				stream << "SpinConfiguration_" << i;
				emit send_simulation_step("Step = " + QString::number(i));
				emit send_repaint_request();
				emit send_save_image_request(QString::fromStdString(fname + stream.str()
					+ ".png"));
				Functions::save(setup->_spinOrientation->get_activity_list(), 
					_spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms(),
					fname + stream.str());
			}
		}
	}

	// Save information about the lattice used in the simulation to simulation folder
	save_lattice_information(setup->_lattice.data(), simFolder.absolutePath().toStdString() + "/SYSTEM/", simID, boolFolderOutput);
}

void SimulationProgram::eigen_frequency(const std::shared_ptr<Setup>& setup, std::string fname)
{
	/**
	* Calculation of eigenmodes for current spin and lattice system configuration. The sparse matrix,
	* the eigenvalues and eigenvectors are stored in text files.
	*
	* @param[in] setup Information about the spin system, lattice system and Hamiltonian
	* @param[in] fname Base name for output files.
	*/

	std::shared_ptr<ExcitationModeSolver> excitationModeSolver = std::make_shared<ExcitationModeSolver>
		(setup->_spinOrientation.data(), setup->_hamilton);
	excitationModeSolver->setup_matrix();

	excitationModeSolver->save_sparse_matrix(fname + "sparseMatrix");

	excitationModeSolver->diagonalize(_config->_numEigenStates);

	excitationModeSolver->save_eigenmodes(fname);

	Functions::save(setup->_spinOrientation->get_activity_list(),
		setup->_spinOrientation->get_spin_array(),
		setup->_spinOrientation->get_number_atoms(), fname + "sparseMatrix_spinConfiguration");
}


QDir SimulationProgram::create_unique_simulation_folder(std::string &simID, int boolFolderOutput)
{
	/**
	* Create a simulation folder with unique ID and subfolders SYSTEM and SIMULATION for output of informaiton
	* about lattice structure and measurements during simulaiton. 
	* Copy configuration file to SYSTEM folder if simulation was setup with configuration file instead of GUI.
	* Write new entry into README file about the new simulation.
	*
	* @param[in] simulation identity number is returned by passing by reference.
	*
	* @return The simulation folder.
	*/	

	if (boolFolderOutput == FALSE)
	{
		return {};
	}

	// unique simulation folder for output during simulation
	QDir sim_folder = _workFolder;
	sim_folder.cd("Data");

	// obtain unique identity number for the new simulation from the helper file in the working directory
	simID = Functions::get_id(_workFolder);

	// unique simulation identity number ensures that simulation folder is unique.
	auto simulation_folder_name = QString::fromStdString(simID);

	// append string generated from parameters specified in _config. This is useful to quickly determine the
	// purpose of a simulation from the folder name.
	simulation_folder_name.append(QString::fromStdString(Functions::folder_name(_config.get())));

	// create simulation folder
	sim_folder.mkdir(simulation_folder_name);

	// create subfolder SYSTEM for output of system information
	sim_folder.cd(simulation_folder_name);
	sim_folder.mkdir("SYSTEM");

	// store information about simulation parameters in output folder
	QDir system_dir = sim_folder;
	system_dir.cd("SYSTEM");
	QFile sim_info{ system_dir.absoluteFilePath("SimInf") };
	sim_info.open(QIODevice::WriteOnly | QIODevice::Text);
	sim_info.write(_config->all_parameters().c_str());
	sim_info.close();

	// create subfolder SIMULATION for ouput during simulation
	QDir simulation_dir = sim_folder;
	simulation_dir.mkdir("SIMULATION");

	// write new entry with simulation information into README file
	Functions::write_README(_workFolder, sim_folder.absolutePath().toStdString(), _config.get());

	return sim_folder;
}

void SimulationProgram::save_lattice_information(Lattice* lattice, std::string path, std::string simID,
	int boolFolderOutput)
{
	/**
	* Save information about lattice.
	*
	* @param[in] lattice A lattice configuration.
	* @param[in] path The path where to store the lattice information in.
	* @param[in] simID A simulation identity number which is built in into the file names to facilitate the
	*                  identification of the simulation.
	*/

	if (boolFolderOutput == TRUE)
	{
		std::string fname = "";

		// save lattice
		fname = path;
		fname.append(simID);
		fname.append("_lattice");
		Functions::save(lattice->get_lattice_coordinate_array(), lattice->get_number_atoms(), fname);

		// save winding number cells
		if (lattice->get_skN_cells() != NULL)
		{
			fname = path;
			fname.append(simID);
			fname.append("_WindingNumberCells");
			Functions::save(lattice->get_skN_cells(), lattice->get_skN_cell_number(), fname);
		}

		// save nearest neighbor arrays (up to the fifth which is set just arbitrarily):
		for (int i = 0; i < 5; i++)
		{
			int order = i + 1; // nearest neighbor corresponds to value 1! 
			int* neighborArray = lattice->get_neighbor_array(order);
			if (neighborArray != NULL)
			{
				fname = path;
				fname.append(simID);
				fname.append("_");
				std::stringstream sstream;
				sstream << order;
				fname.append(sstream.str());
				fname.append("_nbors");

				int neighborArraySize = lattice->get_number_atoms()*lattice->get_number_nth_neighbors(order);
				Functions::save(neighborArray, neighborArraySize, fname);
			}
		}
	}
}
