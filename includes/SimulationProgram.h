/*
* SimulationProgram.h
*
*      
*
* This is the main class of the simulation software for the modelling of magnetic crystal systems. From here,
* the course of the program is managed. This includes: The creation of lattice, spin configuration,
* Hamiltonian, start of Monte Carlo or Spin Dynamics simulation and output of system information.
*
*
*/

#ifndef SIMULATIONPROGRAM_H_
#define SIMULATIONPROGRAM_H_

// standard includes
#include <memory>
#include <string>

// Qt includes
#include <QDir>
#include <QObject>
#include <QSharedPointer>
class QMutex;

//own
#include "typedefs.h"
class Configuration;
class Lattice;
class Hamiltonian;
class SpinOrientation;
class Setup;
class RanGen;

/// Main class of simulation software.
/**
* This is the main class of the simulation software for the modelling of magnetic crystal systems.From here,
* the course of the program is managed. This includes : The creation of lattice, spin configuration,
* Hamiltonian, start of Monte Carlo or Spin Dynamics simulation and output of system information.
*/

class SimulationProgram: public QObject
{
Q_OBJECT
public:
	SimulationProgram(QDir &workfolder, const std::shared_ptr<Configuration> &config, QMutex* mutex,
		int* terminateThread, QSharedPointer<Lattice> lattice,
		QSharedPointer<SpinOrientation> spinOrientation);
	virtual ~SimulationProgram();

	QMutex* _mutex; ///< prevent simultaneous access to shared data between GUI and simulation thread
	int* _terminateThread; ///< shared with GUI; boolean variable to abort running simulation
	
public slots:
	void main(void); ///< starts simulation software

signals:
	/// adjust camera of OGLWidget to current lattice structure
	void send_camera_adjustment_request();
	/// send simulation progress to GUI (e.g. magnetic field, temperature...)
	void send_simulation_info(QString qString);
	/// send simulation step to GUI
	void send_simulation_step(QString qString);
	/// send convergence criterion
	void send_simulation_convergence_criterion(QString qString);
	/// send repaint request for OGLWidget
	void send_repaint_request();
	/// send request to save current OGLWidget to image file
	void send_save_image_request(QString qString);
	/// send lattice and spin objects to GUI
	/** For graphical output during simulation and as cache for output until next simulation starts */
	void send_crystal_structure(QSharedPointer<Lattice> lattice, 
		QSharedPointer<SpinOrientation> spinOrientation, int boolNew);
	void send_hamiltonian(QSharedPointer<Hamiltonian> hamiltonian);
	/// send information about termination of simulation program
	void send_finished();

protected:
	// Currently there are five different program types implemented
	
	/// Program type 1: save energies resolved to lattice sites for a read in spin configuration
	void save_lattice_site_energies(const std::shared_ptr<Setup> &setup, std::string fname);
	
	/// Program type 2: save topological charge resolved to lattice sites for a read-in spin configuration
	void save_local_winding_number(const std::shared_ptr<Setup> &setup, std::string fname);	
	
	/// Program type 3: Monte Carlo or Spin Dynamics simulation within a temperature and magnetic field loop
	void temperature_magnetic_field_loop(const std::shared_ptr<Setup> &setup, std::shared_ptr<RanGen> ranGen, 
		int boolFolderOutput);
	
	/// Program type 4: Monte Carlo or Spin Dynamics simulation with a temperature gradient over lattice
	void spin_seebeck(const std::shared_ptr<Setup> &setup, std::shared_ptr<RanGen> ranGen, 
		int boolFolderOutput);
	
	/// Program type 5: Monte Carlo or Spin Dynamics simulation with a magnetic tip
	void tip_movement(const std::shared_ptr<Setup> &setup, std::shared_ptr<RanGen> ranGen);

	/// Experiment01: Rotation of edge spins.
	void experiment01(const std::shared_ptr<Setup> &setup, std::shared_ptr<RanGen> ranGen, 
		int boolFolderOutput);

	/// Eigen frequency calculation.
	void eigen_frequency(const std::shared_ptr<Setup> &setup, std::string fname);
	
	/// create a unique simulation folder 
	QDir create_unique_simulation_folder(std::string &simID, int boolFolderOutput = 1);
	/// save lattice information to simulation folder
	void save_lattice_information(Lattice* lattice, std::string path, std::string simID, 
		int boolFolderOutput = 1);

	/// working folder 
	/** containing: README with history of all simulations run with this working folder, "Data" folder 
	containing the simulation folders */
	QDir _workFolder; 
	/// configuration parameters
	std::shared_ptr<Configuration> _config;
	/// lattice from cache of last simulation
	QSharedPointer<Lattice> _lattice;
	/// spin orientation from cache of last simulation
	QSharedPointer<SpinOrientation> _spinOrientation;
};

#endif /* SIMULATIONPROGRAM_H_ */
