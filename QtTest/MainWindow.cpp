/*
* MainWindow.cpp
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

#include "MainWindow.h"

// forward declared
#include "AnisotropyWindow.h"
#include "ColorsWindow.h"
#include "Experiment01Window.h"
#include "ExcitationFrequencyWindow.h"

#include "MarkedSpinsHandler.h"

#include "Configuration.h"
#include "Lattice.h"
#include "SpinOrientation.h"
#include "SimulationProgram.h"
#include "GUIProgramTypeElement.h"
#include "GUIEnergyElements.h"
#include "GUILatticeElements.h"
#include "GUISpinElements.h"
#include "GUISimulationProcedureElements.h"
#include "GUIOutputElements.h"
#include "WorkfolderWindow.h"

#include "direct.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	/**
	* Constructor. Sets up GUI elements and sets default values for simulation parameters.
	*/

	_ui = new Ui::QtMainWindow();
	_ui->setupUi(this); // auto generated user interface from Qt forms

	_blockSimulationStart = FALSE;

	_colorsWindow = NULL;
	_experiment01Window = NULL;
	_excitationFreqWindow = NULL;

	_ui->textEditSimulationInfo->setFontPointSize(12);
	_ui->textEditSimulationInfo->setDisabled(1);
	_ui->textEditSimulationStep->setFontPointSize(12);
	_ui->textEditSimulationStep->setDisabled(1);
	_ui->textEditConvergence->setFontPointSize(12);
	_ui->textEditConvergence->setDisabled(1);
	_ui->textEditColorMap->setDisabled(1);

	_ui->pushButtonStartStop->setStyleSheet("QPushButton { background-color: green; }");
	
	_guiProgramTypeElement = new GUIProgramTypeElement(_ui);
	_guiEnergyElements = new GUIEnergyElements(_ui);
	_guiLatticeElements = new GUILatticeElements(_ui);
	_guiSpinElements = new GUISpinElements(_ui);
	_guiOutputElements = new GUIOutputElements(_ui);
	_guiSimulationProcedureElements = new GUISimulationProcedureElements(_ui);

	_markedSpinsHandler = new MarkedSpinsHandler(this,_ui->openGLWidget);
	_ui->openGLWidget->set_marked_spins_handler(_markedSpinsHandler);

	_simulationThread = NULL; // thread in which simulation will run
	_terminateThread = new int(0); // variable to abort simulation

	_workfolder = ""; // folder to store simulation results

	// output of color map information to the GUI
	_ui->textEditColorMap->setFontPointSize(12);
	connect(_ui->openGLWidget, SIGNAL(color_map_text(QString)), _ui->textEditColorMap, 
		SLOT(setText(QString)), Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
	delete _terminateThread;
	delete _guiProgramTypeElement;
	delete _guiEnergyElements;
	delete _guiLatticeElements;
	delete _guiSpinElements;
	delete _guiSimulationProcedureElements;
	delete _guiOutputElements;
	delete _ui;
	delete _markedSpinsHandler;
}

void MainWindow::start_stop_simulation(void)
{
	/**
	* Starts simulation program with current GUI parameters if no simulation is currently running.
	* Otherwise current simulation is stopped. In this case the current spin and lattice configuration are 
	* saved as possible start configuration for next simulation.
	*/

	// simulation not running, new simulation will be started.
	if (_simulationThread == NULL && _colorsWindow == NULL && _blockSimulationStart == FALSE) 
	{
		_ui->pushButtonStartStop->setStyleSheet("QPushButton { background-color: red; }");
		_ui->pushButtonStartStop->setText("Stop");

		// Configuration parameters.
		QSharedPointer<Configuration> config = QSharedPointer<Configuration>(new Configuration); 
		read_parameters_from_ui(config);

		config->determine_outputfolder_needed();
		config->_storageFname = _storageFname;
		
		auto simulationProgram = new SimulationProgram(_workfolder, config, _ui->openGLWidget->_mutex,
			_terminateThread, _lattice, _spinOrientation);

		_simulationThread = new QThread(); // Thread to run simulation in.

		// Connect start of thread with the start of simulation.
		connect(_simulationThread, SIGNAL(started()), simulationProgram, SLOT(main()));

		// obtain lattice and spin configuration in GUI thread for graphical output during simulation and as
		// cache for graphical output until next simulation starts.
		connect(simulationProgram, 
			SIGNAL(send_crystal_structure(QSharedPointer<Lattice>, QSharedPointer<SpinOrientation>, int)), this,
			SLOT(receive_system_cache(QSharedPointer<Lattice>, QSharedPointer<SpinOrientation>, int)),
			Qt::DirectConnection);

		connect(simulationProgram, SIGNAL(send_hamiltonian(QSharedPointer<Hamiltonian>)), this,
			SLOT(receive_hamiltonian(QSharedPointer<Hamiltonian>)), Qt::DirectConnection);
		
		// Adjustment of camera postion in openGLWidget to lattice dimensions
		connect(simulationProgram, SIGNAL(send_camera_adjustment_request()), _ui->openGLWidget, 
			SLOT(adjust_camera_to_lattice()), Qt::DirectConnection);
		
		// Trigger repaint of openGLWidget
		connect(simulationProgram, SIGNAL(send_repaint_request()), _ui->openGLWidget, SLOT(repaint()));
		
		// Update Status information of simulation in GUI
		connect(simulationProgram, SIGNAL(send_simulation_info(QString)), _ui->textEditSimulationInfo,
			SLOT(setText(QString)));

		// Update Status of simulation number in GUI
		connect(simulationProgram, SIGNAL(send_simulation_step(QString)), _ui->textEditSimulationStep, 
			SLOT(setText(QString)));

		// Update value of convergence criterion in GUI
		connect(simulationProgram, SIGNAL(send_simulation_convergence_criterion(QString)), _ui->textEditConvergence,
			SLOT(setText(QString)));

		// Save QWidget to image
		connect(simulationProgram, SIGNAL(send_save_image_request(QString)), _ui->openGLWidget, 
			SLOT(receive_save_request(QString)), Qt::BlockingQueuedConnection);

		// Quit Thread when simulation is done.
		connect(simulationProgram, SIGNAL(send_finished()), _simulationThread, SLOT(quit()));

		connect(simulationProgram, SIGNAL(send_finished()), simulationProgram, SLOT(deleteLater()));

		connect(_simulationThread, SIGNAL(finished()), _simulationThread, SLOT(deleteLater()));
		
		connect(_simulationThread, SIGNAL(destroyed()), this, SLOT(on_program_done()));
		
		// Move simulation to separate thread
		simulationProgram->moveToThread(_simulationThread);

		// start thread and alongside with it the Monte Carlo calculation
		_simulationThread->start();
	}
	else if ( _simulationThread != NULL) // simulation running and will be terminated
	{
		_ui->openGLWidget->_mutex->lock();
		*_terminateThread = 1; // variable checked regularly in the simulation loop
		_ui->openGLWidget->_mutex->unlock();
	}
	else if (_colorsWindow != NULL) // program start not allowed while color window open
	{
		_colorsWindow->error_box();
	}
}

void MainWindow::receive_system_cache(QSharedPointer<Lattice> lattice,
	QSharedPointer<SpinOrientation> spinOrientation, int boolNew)
{
	/**
	Save lattice and spins of last simulation for next simulation.
	*/

	_lattice = lattice;
	_spinOrientation = spinOrientation;

	_ui->openGLWidget->set_lattice(lattice);
	_ui->openGLWidget->set_spins(spinOrientation->get_spin_array(), lattice->get_number_atoms(), boolNew);

	_markedSpinsHandler->set_spin_orientation(spinOrientation, boolNew);
}

void MainWindow::receive_hamiltonian(QSharedPointer<Hamiltonian> hamiltonian)
{
	/**
	Receive Hamiltonian in order to provide energy color maps on the GUI.
	*/

	_ui->openGLWidget->set_hamiltonian(hamiltonian);
}

void MainWindow::on_program_done(void)
{
	/**
	Clean-up work after Thread with simulation is done.
	*/

	_simulationThread = NULL;
	*_terminateThread = 0;
	_ui->pushButtonStartStop->setText("Start");
	_ui->pushButtonStartStop->setStyleSheet("QPushButton { background-color: green; }");
	if (_ui->comboBoxProgramType->currentText().contains("save")
		|| _ui->comboBoxProgramType->currentText().contains("read"))
	{
		_ui->comboBoxProgramType->setCurrentIndex(0);
	}
}

void MainWindow::on_program_type_selected(QString qString)
{
	/**
	* enable/disable GUI elements according to selected program type.
	*
	* @param[in] qString Specifier of program type.
	*/

	if (qString.compare("experiment01") == 0)
	{
		if (_experiment01Window == NULL && _simulationThread == NULL)
		{
			_experiment01Window = new Experiment01Window(this);
			_experiment01Window->setAttribute(Qt::WA_DeleteOnClose);
			connect(_experiment01Window, SIGNAL(destroyed()), this, SLOT(on_experiment01_window_destroyed()));
			_experiment01Window->show();
		}
	}
	else if (qString.compare("eigenFreq") == 0)
	{
		if (_excitationFreqWindow == NULL && _simulationThread == NULL)
		{
			_excitationFreqWindow = new ExcitationFrequencyWindow(_ui->openGLWidget, _workfolder, this);
			_excitationFreqWindow->setAttribute(Qt::WA_DeleteOnClose);
			
			if (_spinOrientation)
			{
				_excitationFreqWindow->set_spin_array(_spinOrientation->get_spin_array(),
								_spinOrientation->get_number_atoms());
			}
			connect(_excitationFreqWindow, SIGNAL(destroyed()), this, SLOT(on_excitation_freq_window_destroyed()));
			_excitationFreqWindow->show();
		}
	}
	else if (qString.compare("read bitmap lattice mask") == 0)
	{
		QString fname("");
		fname = QFileDialog::getOpenFileName(this, tr(qPrintable("Filename for " + qString)),
				QString::fromStdString(_workfolder), "Bitmaps (*.bmp)");
		_storageFname = fname.toStdString();
		if (fname.contains(".bmp"))
		{
			_guiLatticeElements->_latticeMaskParameters.fname = fname.toStdString();
			_guiLatticeElements->show_lattice_mask_window();
			start_stop_simulation();
		}
		else
		{
			_ui->comboBoxProgramType->setCurrentIndex(0);
		}
	}
	else 
	{
		QString fname("");
		if (qString.contains("save"))
		{
			fname = QFileDialog::getSaveFileName(this, tr(qPrintable("Filename for " + qString)), 
				QString::fromStdString(_workfolder), "ALL files (*)");
			_storageFname = fname.toStdString();
			if (fname.size() != 0)
			{
				start_stop_simulation();
			}
			else
			{
				_ui->comboBoxProgramType->setCurrentIndex(0);
			}
		}
		else if (qString.contains("read"))
		{
			fname = QFileDialog::getOpenFileName(this, tr(qPrintable("Filename for " + qString)),
				QString::fromStdString(_workfolder), "ALL files (*)");
			_storageFname = fname.toStdString();
			if (fname.size() != 0)
			{
				start_stop_simulation();
			}
			else
			{
				_ui->comboBoxProgramType->setCurrentIndex(0);
			}
		}
	}
}

void MainWindow::on_push_button_colors(void)
{
	/**
	Open window to specify graphical output in the GUI (Spins, color maps).
	*/

	if (_simulationThread == NULL && _colorsWindow == NULL)
	{
		_colorsWindow = new ColorsWindow(_ui->openGLWidget, this);
		_colorsWindow->setAttribute(Qt::WA_DeleteOnClose, true);

		connect(_colorsWindow, SIGNAL(send_repaint_request()), _ui->openGLWidget, SLOT(repaint()));

		connect(_colorsWindow, SIGNAL(destroyed()), this, SLOT(on_colors_window_destroyed()));
		_colorsWindow->open();
	}
}

void MainWindow::on_colors_window_destroyed()
{
	/**
	Reset member pointer to zero after window deletion.
	*/

	_colorsWindow = NULL;
}

void MainWindow::on_experiment01_window_destroyed()
{
	_experiment01Window = NULL;
	_ui->comboBoxProgramType->setCurrentIndex(0);
}

void MainWindow::on_excitation_freq_window_destroyed()
{
	_excitationFreqWindow = NULL;
	_ui->comboBoxProgramType->setCurrentIndex(0);
}

void MainWindow::on_anisotropy_button(void)
{
	/**
	Open window for specification of anisotropy energies.
	*/

	_guiEnergyElements->open_anisotropy_window();
}

void MainWindow::on_push_button_workfolder(void)
{
	/**
	Open dialog to specify a workfolder (used for text output).
	*/

	read_workfolder();
}

void MainWindow::on_push_button_random_spin(void)
{
	/**
	When a spin configuration has been displayed to the GUI and a simulation is stopped, this button can be
	used to create a random spin configuration.
	*/

	if (_simulationThread == NULL && _spinOrientation != NULL && _lattice != NULL)
	{
		_guiSpinElements->random_spin_configuration(_spinOrientation.data());
	}
}

void MainWindow::on_push_button_ferromagnet(void)
{
	/**
	When a spin configuration has been displayed to the GUI and a simulation is stopped, this button can be
	used to create a ferromagnetic spin configuration.
	*/

	if (_simulationThread == NULL && _spinOrientation != NULL && _lattice != NULL)
	{
		_guiSpinElements->open_ferromagnet_window(_lattice->get_lattice_coordinate_array(),
			_spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms());
	}
}

void MainWindow::on_push_button_spin_spiral(void)
{
	/**
	When a spin configuration has been displayed to the GUI and a simulation is stopped, this button can be
	used to create a spin-spiral spin configuration.
	*/

	if (_simulationThread == NULL && _spinOrientation != NULL && _lattice != NULL)
	{
		_guiSpinElements->open_spin_spiral_window(_ui->openGLWidget, _lattice->get_lattice_coordinate_array(),
			_spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms());
	}
}

void MainWindow::on_push_button_skyrmions(void)
{
	/**
	When a spin configuration has been displayed to the GUI and a simulation is stopped, this button can be
	used to add a skyrmion to the current spin configuration.
	*/

	if (_simulationThread == NULL && _spinOrientation != NULL && _lattice != NULL)
	{
		_guiSpinElements->open_skyrmion_window(_ui->openGLWidget,_lattice->get_lattice_coordinate_array(),
			_spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms());
	}
}

void MainWindow::on_lattice_type_selected(QString qString)
{
	_guiLatticeElements->update_to_lattice_type(qString);
}

void MainWindow::open_workfolder_dialog()
{
	/**
	Open dialog to select a workfolder (used for text file output)
	*/

	QFileDialog dialog(this, "Workfolder", "");
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	int result = dialog.exec();
	QString directory;
	if (result)
	{
		directory = dialog.selectedFiles()[0];
		_workfolder = (directory.toStdString()).append("/");
		write_workfolder();
	}
	else
	{
		open_workfolder_dialog();
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "MonteCrystal",
		tr("Are you sure about closing this program?\n"),
		 QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	}
	else {
		event->accept();
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		emit close();
	}
	else
	{
		QWidget::keyReleaseEvent(event);
	}
}

void MainWindow::block_simulation_start(void)
{
	_blockSimulationStart += 1;
}

void MainWindow::unblock_simulation_start(void)
{
	if (_blockSimulationStart > 0)
	{
		_blockSimulationStart -= 1;
	}
}

int MainWindow::simulation_running(void)
{
	if (_simulationThread == NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


void MainWindow::read_workfolder(void)
{
	/**
	Read workfolder (used for text file output) from textfile which was automatically written during last run
	of this program. If no file with this information exists, a window to select a workfolder will be opened.
	*/

	std::string fname = "workfolder";
	std::fstream filestr;
	filestr.open(fname, std::fstream::in);
	if (!filestr.good())
	{
		filestr.close();
		filestr.open(fname, std::fstream::out);
		filestr << "";
		filestr.close();
		open_workfolder_dialog();
	}
	else 
	{
		filestr >> _workfolder;
		filestr.close();
		if (_workfolder.compare("") == 0)
		{
			open_workfolder_dialog();
		}
		WorkfolderWindow* workfolder = new WorkfolderWindow();
		workfolder->setAttribute(Qt::WA_DeleteOnClose, true);
		connect(workfolder, SIGNAL(send_select_other()), this,
			SLOT(open_workfolder_dialog()));
		workfolder->set_text_edit(QString::fromStdString(_workfolder));
		workfolder->exec();
	}
}

void MainWindow::write_workfolder(void)
{
	/**
	Save currently selected workfolder (used for text file output) in a text file.
	*/

	std::string fname = "workfolder";
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << _workfolder;
	filestr.close();
	std::string tempString = _workfolder + "Data";
	_mkdir(tempString.c_str());
}

void MainWindow::read_parameters_from_ui(QSharedPointer<Configuration> &config)
{
	/**
	Read the parameters for the next simulation from the GUI.

	@param[in] config Object to store parameters in.
	*/

	_guiProgramTypeElement->read_parameter(config);

	_guiLatticeElements->read_parameters(config);
	check_lattice_cache(config);
	config->_latticeMaskParameters = _guiLatticeElements->_latticeMaskParameters;

	_guiSpinElements->read_parameters(config);

	_guiEnergyElements->read_parameters(config);

	config->_exchangeDefects = _markedSpinsHandler->get_exchange_defects();
	config->_dmDefects = _markedSpinsHandler->get_dm_defects();
	config->_anisotropyDefects = _markedSpinsHandler->get_anisotropy_defects();

	_guiSimulationProcedureElements->read_parameters(config);

	_guiOutputElements->read_parameters(config);

	read_experiment_windows(config);
}

void MainWindow::read_experiment_windows(QSharedPointer<Configuration> &config)
{
	if (_experiment01Window != NULL)
	{
		config->_experiment01 = _experiment01Window->read_parameters();
	}

	if (_excitationFreqWindow != NULL)
	{
		_excitationFreqWindow->read_parameters(config);
	}
}

void MainWindow::check_lattice_cache(QSharedPointer<Configuration> &config)
{
	/**
	Checks if lattice stored in cache is equivalent to the parameters specified in config. If not, cache is
	deleted.

	@param[in] config Lattice of last simulation is compared to parameters specified in config.
	*/

	int latticeIdentical = 1;

	if (_lattice == NULL)
	{
		//if no lattice available from previous simulaiton, new one has to be created
		latticeIdentical = 0;
	}
	else if (config->_latticeType != _lattice->_latticeType)
	{
		latticeIdentical = 0;
	}
	else if (config->_boundaryConditions != _lattice->_boundaryConditions)
	{
		latticeIdentical = 0;
	}
	else if (config->_latticeDimensions.size() != _lattice->_latticeDimensions.size())
	{
		latticeIdentical = 0;
	}
	else
	{
		for (int i = 0; i < config->_latticeDimensions.size(); i++)
		{
			if (config->_latticeDimensions[i] != _lattice->_latticeDimensions[i])
			{
				latticeIdentical = 0;
			}
		}
	}

	if (latticeIdentical == 0)
	{
		_lattice.clear();
	}
}