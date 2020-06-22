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
#include "AtomsWindow.h"
#include "CameraWindow.h"
#include "Experiment01Window.h"
#include "ExcitationFrequencyWindow.h"
#include "StmWindow.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMutex>
#include <QSplitter>

#include "MarkedSpinsHandler.h"

#include "Configuration.h"
#include "Lattice.h"
#include "SpinOrientation.h"
#include "SimulationProgram.h"
#include "Hamiltonian.h"
#include "GUIProgramTypeElement.h"
#include "GUIEnergyElements.h"
#include "GUILatticeElements.h"
#include "GUISpinElements.h"
#include "GUISimulationProcedureElements.h"
#include "GUIOutputElements.h"
#include "WorkfolderWindow.h"

#include "ToolBarWidget.h"
#include "OpenGLWidget.h"

#include "git.h"

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /**
    * Constructor. Sets up GUI elements and sets default values for simulation parameters.
    */

    _ui = new Ui::QtMainWindow();
    _ui->setupUi(this); // auto generated user interface from Qt forms
    const auto splitter = new QSplitter(this);
    _toolbar = new ToolBarWidget(this);
    _opengl_widget = new OpenGLWidget(this);
    splitter->addWidget(_toolbar);
    splitter->addWidget(_opengl_widget);
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1, 4);
    this->setCentralWidget(splitter);

    _blockSimulationStart = FALSE;

    _colorsWindow = NULL;
    _atomsWindow =NULL;
    _cameraWindow =NULL;
    _experiment01Window = NULL;
    _excitationFreqWindow = NULL;
    _stmWindow =NULL;
    _opengl_widget->textEditSimulationInfo->setFontPointSize(12);
    _opengl_widget->textEditSimulationInfo->setDisabled(1);
    _opengl_widget->textEditSimulationStep->setFontPointSize(12);
    _opengl_widget->textEditSimulationStep->setDisabled(1);
    _opengl_widget->textEditConvergence->setFontPointSize(12);
    _opengl_widget->textEditConvergence->setDisabled(1);
    _opengl_widget->textEditColorMap->setDisabled(1);

    _toolbar->pushButtonStartStop->setStyleSheet("QPushButton { background-color: green; }");

    _guiProgramTypeElement = new GUIProgramTypeElement(this);
    _guiEnergyElements = new GUIEnergyElements(this);
    _guiLatticeElements = new GUILatticeElements(this);
    _guiSpinElements = new GUISpinElements(this);
    _guiOutputElements = new GUIOutputElements(this);
    _guiSimulationProcedureElements = new GUISimulationProcedureElements(this);

    _markedSpinsHandler = new MarkedSpinsHandler(this, _opengl_widget->openGLWidget);
    _opengl_widget->openGLWidget->set_marked_spins_handler(_markedSpinsHandler);

    _simulationThread = NULL; // thread in which simulation will run
    _terminateThread = new int(0); // variable to abort simulation

    _workfolder.setPath(""); // folder to store simulation results

    // output of color map information to the GUI
    _opengl_widget->textEditColorMap->setFontPointSize(12);
    connect(_opengl_widget->openGLWidget, &OGLWidget::color_map_text, _opengl_widget->textEditColorMap,
        &QTextEdit::setText, Qt::DirectConnection);
    connect(_opengl_widget->pushButtonColor, &QAbstractButton::released, this, &MainWindow::push_button_colors);
    connect(_opengl_widget->pushButtonCamera, &QAbstractButton::released, this, &MainWindow::push_button_camera);
    connect(_opengl_widget->pushButtonAtoms, &QAbstractButton::released, this, &MainWindow::push_button_atoms);
    connect(_opengl_widget->pushButtonSTM, &QAbstractButton::released, this, &MainWindow::push_button_stm);
    connect(_toolbar->pushButtonWorkfolder, &QAbstractButton::released, this, &MainWindow::push_button_workfolder);
    connect(_toolbar->about_pushButton, &QAbstractButton::released, this, &MainWindow::show_about_box);
    connect(_toolbar->pushButtonStartStop, &QAbstractButton::released, this, &MainWindow::start_stop_simulation);
    connect(_toolbar->pushButtonSpinSpiral, &QAbstractButton::released, this, &MainWindow::push_button_spin_spiral);
    connect(_toolbar->pushButtonSkyrmions, &QAbstractButton::released, this, &MainWindow::push_button_skyrmions);
    connect(_toolbar->pushButtonFerromagnet, &QAbstractButton::released, this, &MainWindow::push_button_ferromagnet);
    connect(_toolbar->pushButtonRandomSpin, &QAbstractButton::released, this, &MainWindow::push_button_random_spin);
    connect(_toolbar->pushButtonAnisotropy, &QAbstractButton::released, this, &MainWindow::push_button_anisotropy);
    connect(_toolbar->comboBoxProgramType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),
        this, &MainWindow::program_type_selected);
    connect(_toolbar->comboBoxLatticeType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),
        this, &MainWindow::lattice_type_selected);
    connect(_ui->actionClose, &QAction::triggered, this, &QMainWindow::close);
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
        _toolbar->pushButtonStartStop->setStyleSheet("QPushButton { background-color: red; }");
        _toolbar->pushButtonStartStop->setText("Stop");

        // Configuration parameters.
        const auto config = std::make_shared<Configuration>();
        read_parameters_from_ui(config);

        config->determine_outputfolder_needed();
        config->_storageFname = _storageFname;

        auto simulationProgram = new SimulationProgram(_workfolder, config, _opengl_widget->openGLWidget->_mutex,
            _terminateThread, _lattice, _spinOrientation);

        _simulationThread = new QThread(); // Thread to run simulation in.

        // Connect start of thread with the start of simulation.
        connect(_simulationThread, &QThread::started, simulationProgram, &SimulationProgram::main);

        // obtain lattice and spin configuration in GUI thread for graphical output during simulation and as
        // cache for graphical output until next simulation starts.
        connect(simulationProgram, &SimulationProgram::send_crystal_structure, this,
            &MainWindow::receive_system_cache, Qt::DirectConnection);

        connect(simulationProgram, &SimulationProgram::send_hamiltonian, this,
            &MainWindow::receive_hamiltonian, Qt::DirectConnection);

        // Adjustment of camera postion in openGLWidget to lattice dimensions
        connect(simulationProgram, &SimulationProgram::send_camera_adjustment_request,
            _opengl_widget->openGLWidget, &OGLWidget::adjust_camera_to_lattice, Qt::DirectConnection);

        // Trigger repaint of openGLWidget
        connect(simulationProgram, &SimulationProgram::send_repaint_request,
            _opengl_widget->openGLWidget, static_cast<void (QWidget::*)()>(&QWidget::repaint));

        // Update Status information of simulation in GUI
        connect(simulationProgram, &SimulationProgram::send_simulation_info,
            _opengl_widget->textEditSimulationInfo, &QTextEdit::setText);

        // Update Status of simulation number in GUI
        connect(simulationProgram, &SimulationProgram::send_simulation_step,
            _opengl_widget->textEditSimulationStep, &QTextEdit::setText);

        // Update value of convergence criterion in GUI
        connect(simulationProgram, &SimulationProgram::send_simulation_convergence_criterion,
            _opengl_widget->textEditConvergence, &QTextEdit::setText);

        // Save QWidget to image
        connect(simulationProgram, &SimulationProgram::send_save_image_request,
            _opengl_widget->openGLWidget, &OGLWidget::receive_save_request, Qt::BlockingQueuedConnection);

        // Quit Thread when simulation is done.
        connect(simulationProgram, &SimulationProgram::send_finished, _simulationThread, &QThread::quit);

        connect(simulationProgram, &SimulationProgram::send_finished, simulationProgram, &QObject::deleteLater);

        connect(_simulationThread, &QThread::finished, _simulationThread, &QThread::deleteLater);

        connect(_simulationThread, &QThread::destroyed, this, &MainWindow::program_done);

        // Move simulation to separate thread
        simulationProgram->moveToThread(_simulationThread);

        // start thread and alongside with it the Monte Carlo calculation
        _simulationThread->start();
    }
    else if ( _simulationThread != NULL) // simulation running and will be terminated
    {
        _opengl_widget->openGLWidget->_mutex->lock();
        *_terminateThread = 1; // variable checked regularly in the simulation loop
        _opengl_widget->openGLWidget->_mutex->unlock();
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

    _opengl_widget->openGLWidget->set_lattice(lattice);
    _opengl_widget->openGLWidget->set_spins(spinOrientation->get_spin_array(), lattice->get_number_atoms(), boolNew);

    _markedSpinsHandler->set_spin_orientation(spinOrientation, boolNew);
}

void MainWindow::receive_hamiltonian(QSharedPointer<Hamiltonian> hamiltonian)
{
    /**
    Receive Hamiltonian in order to provide energy color maps on the GUI.
    */

    _opengl_widget->openGLWidget->set_hamiltonian(hamiltonian);
}

void MainWindow::program_done(void)
{
    /**
    Clean-up work after Thread with simulation is done.
    */

    _simulationThread = NULL;
    *_terminateThread = 0;
    _toolbar->pushButtonStartStop->setText("Start");
    _toolbar->pushButtonStartStop->setStyleSheet("QPushButton { background-color: green; }");
    if (_toolbar->comboBoxProgramType->currentText().contains("save")
        || _toolbar->comboBoxProgramType->currentText().contains("read"))
    {
        _toolbar->comboBoxProgramType->setCurrentIndex(0);
    }
}

void MainWindow::program_type_selected(const QString &qString)
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
            connect(_experiment01Window, &Experiment01Window::destroyed,
                this, &MainWindow::experiment01_window_destroyed);
            _experiment01Window->show();
        }
    }
    else if (qString.compare("eigenFreq") == 0)
    {
        if (_excitationFreqWindow == NULL && _simulationThread == NULL)
        {
            _excitationFreqWindow = new ExcitationFrequencyWindow(_opengl_widget->openGLWidget, _workfolder, this);
            _excitationFreqWindow->setAttribute(Qt::WA_DeleteOnClose);

            if (_spinOrientation)
            {
                _excitationFreqWindow->set_spin_array(_spinOrientation->get_spin_array(),
                                _spinOrientation->get_number_atoms());
            }
            connect(_excitationFreqWindow, &ExcitationFrequencyWindow::destroyed,
                this, &MainWindow::excitation_freq_window_destroyed);
            _excitationFreqWindow->show();
        }
    }
    else if (qString.compare("read bitmap lattice mask") == 0)
    {
        QString fname("");
        fname = QFileDialog::getOpenFileName(this, tr(qPrintable("Filename for " + qString)),
                                             _workfolder.absolutePath(), "Bitmaps (*.bmp)");
        _storageFname = fname.toStdString();
        if (fname.contains(".bmp"))
        {
            _guiLatticeElements->_latticeMaskParameters.fname = fname.toStdString();
            _guiLatticeElements->show_lattice_mask_window();
            start_stop_simulation();
        }
        else
        {
            _toolbar->comboBoxProgramType->setCurrentIndex(0);
        }
    }
    else
    {
        QString fname("");
        if (qString.contains("save"))
        {
            fname = QFileDialog::getSaveFileName(this, tr(qPrintable("Filename for " + qString)),
                                                 _workfolder.absolutePath(), "ALL files (*)");
            _storageFname = fname.toStdString();
            if (fname.size() != 0)
            {
                start_stop_simulation();
            }
            else
            {
                _toolbar->comboBoxProgramType->setCurrentIndex(0);
            }
        }
        else if (qString.contains("read"))
        {
            fname = QFileDialog::getOpenFileName(this, tr(qPrintable("Filename for " + qString)),
                                                 _workfolder.absolutePath(), "ALL files (*)");
            _storageFname = fname.toStdString();
            if (fname.size() != 0)
            {
                start_stop_simulation();
            }
            else
            {
                _toolbar->comboBoxProgramType->setCurrentIndex(0);
            }
        }
    }
}

void MainWindow::push_button_colors(void)
{
    /**
    Open window to specify graphical output in the GUI (Spins, color maps).
    */

    if (_simulationThread == NULL && _colorsWindow == NULL)
    {
        _colorsWindow = new ColorsWindow(_opengl_widget->openGLWidget, this);
        _colorsWindow->setAttribute(Qt::WA_DeleteOnClose, true);

        connect(_colorsWindow, &ColorsWindow::send_repaint_request,
            _opengl_widget->openGLWidget, static_cast<void (QWidget::*)()>(&QWidget::repaint));

        connect(_colorsWindow, &ColorsWindow::destroyed, this, &MainWindow::colors_window_destroyed);
        _colorsWindow->open();
    }
}


void MainWindow::push_button_atoms(void)
{
    /**
    Open window to specify Camera Position and Orientation.
    */

    if (_simulationThread == NULL && _atomsWindow == NULL)
    {
        _atomsWindow = new AtomsWindow(_opengl_widget->openGLWidget, this);
        _atomsWindow->setAttribute(Qt::WA_DeleteOnClose, true);

        connect(_atomsWindow, &CameraWindow::destroyed, this, &MainWindow::atoms_window_destroyed);
        _atomsWindow->open();
    }
}

void MainWindow::push_button_camera(void)
{
    /**
    Open window to specify Camera Position and Orientation.
    */

    if (_simulationThread == NULL && _cameraWindow == NULL)
    {
        _cameraWindow = new CameraWindow(_opengl_widget->openGLWidget, this);
        _cameraWindow->setAttribute(Qt::WA_DeleteOnClose, true);

        connect(_cameraWindow, &CameraWindow::destroyed, this, &MainWindow::camera_window_destroyed);
        _cameraWindow->open();
    }
}




void MainWindow::colors_window_destroyed()
{
    /**
    Reset member pointer to zero after window deletion.
    */

    _colorsWindow = NULL;
}
void MainWindow::atoms_window_destroyed()
{

    /**
    Reset member pointer to zero after window deletion.
    */

    _atomsWindow = NULL;
}


void MainWindow::camera_window_destroyed()
{

    /**
    Reset member pointer to zero after window deletion.
    */

    _cameraWindow = NULL;
}
void MainWindow::experiment01_window_destroyed()
{
    _experiment01Window = NULL;
    _toolbar->comboBoxProgramType->setCurrentIndex(0);
}

void MainWindow::excitation_freq_window_destroyed()
{
    _excitationFreqWindow = NULL;
    _toolbar->comboBoxProgramType->setCurrentIndex(0);
}

void MainWindow::push_button_stm(void)
{
    /**
    Open window to specify Camera Position and Orientation.
    */

    if (_stmWindow == NULL && _lattice != NULL)
    {

        _stmWindow = new StmWindow(_opengl_widget->openGLWidget,_lattice, this);
        _stmWindow ->setAttribute(Qt::WA_DeleteOnClose, true);



        connect(_stmWindow, &StmWindow::destroyed, this, &MainWindow::stm_window_destroyed);
        _stmWindow->open();
    }
}

void MainWindow::stm_window_destroyed()
{

    /**
    Reset member pointer to zero after window deletion.
    */

    _stmWindow = NULL;
}


void MainWindow::push_button_anisotropy(void)
{
    /**
    Open window for specification of anisotropy energies.
    */

    _guiEnergyElements->open_anisotropy_window();
}

void MainWindow::show_about_box(void)
{
    const auto about_box = new QMessageBox(this);
    const auto dirty_string = QString{"%1"}.arg(GIT_DIRTY);
    const auto git_dirty = dirty_string.toInt() == 1;
    about_box->setText(QString{ "Git revision %1" }.arg(GIT_HEAD_HASH)
        + (git_dirty ? QString{ " CAUTION: There are local untracked changes. "
            "Note that making the git commit alone will not remove this warning. "
            "You also need to rebuild afterwards." } : QString{}));
    about_box->show();
    about_box->raise();
}

void MainWindow::push_button_workfolder(void)
{
    /**
    Open dialog to specify a workfolder (used for text output).
    */

    read_workfolder();
}

void MainWindow::push_button_random_spin(void)
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

void MainWindow::push_button_ferromagnet(void)
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

void MainWindow::push_button_spin_spiral(void)
{
    /**
    When a spin configuration has been displayed to the GUI and a simulation is stopped, this button can be
    used to create a spin-spiral spin configuration.
    */

    if (_simulationThread == NULL && _spinOrientation != NULL && _lattice != NULL)
    {
        _guiSpinElements->open_spin_spiral_window(_opengl_widget->openGLWidget, _lattice->get_lattice_coordinate_array(),
            _spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms());
    }
}

void MainWindow::push_button_skyrmions(void)
{
    /**
    When a spin configuration has been displayed to the GUI and a simulation is stopped, this button can be
    used to add a skyrmion to the current spin configuration.
    */

    if (_simulationThread == NULL && _spinOrientation != NULL && _lattice != NULL)
    {
        _guiSpinElements->open_skyrmion_window(_opengl_widget->openGLWidget,_lattice->get_lattice_coordinate_array(),
            _spinOrientation->get_spin_array(), _spinOrientation->get_number_atoms());
    }
}

void MainWindow::lattice_type_selected(const QString &qString)
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
        _workfolder = QDir{dialog.selectedFiles()[0]};
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

int MainWindow::simulation_running(void) const
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

    QFile w_file{"workfolder"};
    if (!w_file.exists())
    {
        w_file.open(QIODevice::WriteOnly | QIODevice::Text);
        w_file.close();
        open_workfolder_dialog();
    }
    else
    {
        std::string folder;
        w_file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!w_file.atEnd())
        {
            QByteArray line = w_file.readLine();
            folder = line.toStdString();
        }

        _workfolder.setPath(QString::fromStdString(folder));
        w_file.close();
        if (folder.compare("") == 0)
        {
            open_workfolder_dialog();
        }
        WorkfolderWindow* workfolder = new WorkfolderWindow();
        workfolder->setAttribute(Qt::WA_DeleteOnClose, true);
        connect(workfolder, &WorkfolderWindow::send_select_other, this, &MainWindow::open_workfolder_dialog);
        workfolder->set_text_edit(_workfolder.absolutePath());
        workfolder->exec();
    }
}

void MainWindow::write_workfolder(void)
{
    /**
    Save currently selected workfolder (used for text file output) in a text file.
    */

    QFile file{"workfolder"};
    file.open(stderr, QIODevice::WriteOnly);
    file.write(_workfolder.absolutePath().toStdString().c_str());
    file.resize(file.pos());
    file.close();

    _workfolder.mkdir("Data");
}

void MainWindow::read_parameters_from_ui(const std::shared_ptr<Configuration> &config)
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

void MainWindow::read_experiment_windows(const std::shared_ptr<Configuration> &config)
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

void MainWindow::check_lattice_cache(const std::shared_ptr<Configuration> &config)
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
