/*
* MainWindow.h
*
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// standard includes
#include <memory>
#include <sstream>

// Qt includes
#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include <qthread.h>
#include <qsharedpointer.h>
#include <qfiledialog.h>
#include <qtreeview.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qboxlayout.h>
#include <qstring.h>
#include <qdebug.h>
#include <QCloseEvent>
#include <qkeyevent>

// Form include
#include "ui_mainWindow.h"

// own
#include "typedefs.h"

// forward declarations
class AnisotropyWindow;
class ColorsWindow;
class Experiment01Window;
class ExcitationFrequencyWindow;
class MarkedSpinsHandler;

class Configuration;
class Lattice;
class SpinOrientation;
class Hamiltonian;
class SimulationProgram;
class GUIProgramTypeElement;
class GUIEnergyElements;
class GUILatticeElements;
class GUISpinElements;
class GUISimulationProcedureElements;
class GUIOutputElements;

/// Main GUI window. Specification of simulation parameters and graphical display of simulation.
/**
* Main window of GUI. Offers the possibility to specify simulation parameters and to start a new simulation
* or to stop a currently running simulation. Also, output configurations for the simulation can be set. 
* Output is done into text files into a unique simulation folder. Also, the main window contains an OpenGl
* widget that can be used for on the fly graphical output during a simulation.
*/

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void read_workfolder(void); ///< check whether a worfolder was specified by a previous programme run

	void closeEvent(QCloseEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	void block_simulation_start(void);
	void unblock_simulation_start(void);

	int simulation_running(void);

public slots:
	void start_stop_simulation(void); ///< start new simulation or stop currently running simulation 

	void on_push_button_colors(void);
	void on_colors_window_destroyed();

	void on_experiment01_window_destroyed();
	void on_excitation_freq_window_destroyed();
	
	void on_anisotropy_button(void); ///< open window for specification of anisotropy energies
	
	void on_push_button_workfolder(void); ///< open file dialogue to specify workfolder for ouput
	
	void on_push_button_random_spin(void); ///< generate random spin configuration
	void on_push_button_ferromagnet(void); ///< set ferromagnetic state
	void on_push_button_spin_spiral(void); ///< open dialogue to set spin spiral
	void on_push_button_skyrmions(void); ///< open dialogue to add skymrion
	
	void on_lattice_type_selected(QString qString); ///< update lattice parameter to selected lattice type
	void on_program_type_selected(QString qString); ///< update parameters to selected program type
	void receive_system_cache(QSharedPointer<Lattice> lattice,
		QSharedPointer<SpinOrientation> spinOrientation, int boolNew); ///< keep lattice and spins of last simulation
	void receive_hamiltonian(QSharedPointer<Hamiltonian> hamiltonian);
	void on_program_done(void); ///< do clean-up work when simulation program is done or aborted

	void open_workfolder_dialog(); ///< open directory dialog to specify working directory

protected:
	Ui::QtMainWindow* _ui; ///< auto generated setup of GUI elements from Form file

	ColorsWindow* _colorsWindow; ///< window to specify graphical output to GUI
	Experiment01Window* _experiment01Window;
	ExcitationFrequencyWindow* _excitationFreqWindow;

	QThread* _simulationThread; ///< run simulation in this separate thread
	int* _terminateThread; ///< boolean variable to abort current simulation, shared between both threads

	int _blockSimulationStart;

	GUIProgramTypeElement* _guiProgramTypeElement;
	GUIEnergyElements* _guiEnergyElements;
	GUILatticeElements* _guiLatticeElements;
	GUISpinElements* _guiSpinElements;
	GUISimulationProcedureElements* _guiSimulationProcedureElements;
	GUIOutputElements* _guiOutputElements;

	MarkedSpinsHandler* _markedSpinsHandler;

	// Program parameters
	QSharedPointer<Lattice> _lattice; ///< store lattice between two succeeding simulation runs
	QSharedPointer<SpinOrientation> _spinOrientation; ///< store spins between two succeedingsimulation runs
	std::string _storageFname; ///< multi purpose file name 

	std::string _workfolder;  ///< working folder with "DATA" folder and README file

	void write_workfolder(void);

	// read parameters from gui right before start of a new simulation
	void read_parameters_from_ui(QSharedPointer<Configuration> &config); ///< GUI parameters into config
	
	void read_experiment_windows(QSharedPointer<Configuration> &config);

	/// compare parameters read from GUI for next simulation to lattice of previous simulation
	/** If there has been no changes regarding the crystal structure, the lattice of last simulation will be
	    used */
	void check_lattice_cache(QSharedPointer<Configuration> &config); 
};

#endif // MAINWINDOW_H