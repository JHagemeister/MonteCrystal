/*
* ExcitationFrequencyWindow.cpp
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

#include "ExcitationFrequencyWindow.h"

#include "OGLWidget.h"

#include "QFileDialog"

#include "MyMath.h"
#include "Functions.h"

#include "Configuration.h"

#ifdef WIN
#include "direct.h"
#endif

#ifdef LINUX
#include <sys/stat.h>
#endif



ExcitationFrequencyWindow::ExcitationFrequencyWindow(OGLWidget* oglWidget, std::string workfolder, 
	QWidget * parent) : QDialog(parent)
{
	_ui.setupUi(this);

	_oglWidget = oglWidget;
	_oglWidget->_points.clear();

	_workfolder = workfolder;

	_parameters.index = new int(0);
	_parameters.maxAngle = new double(30);
	_parameters.rescale = new int(0);
	_parameters.pause = new int(0);
	_parameters.videoSequence = new int(0);
	_parameters.terminate = new int(0);
	_parameters.sleepTime = new double(1);
	_parameters.segmentationThreshold = new double(0);

	_outputIndex = 0;

	_evecs = NULL;

	_spinArrayBackup = NULL;

	_visualizationThread = NULL;

	setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	setup_elements();
}

ExcitationFrequencyWindow::~ExcitationFrequencyWindow() 
{
	if (_evecs != NULL)
	{
		gsl_matrix_complex_free(_evecs);
	}

	_oglWidget->_points.clear();

	delete[] _spinArrayBackup;

	delete _parameters.index;
	delete _parameters.maxAngle;
	delete _parameters.rescale;
	delete _parameters.pause;
	delete _parameters.videoSequence;
	delete _parameters.terminate;
	delete _parameters.sleepTime;
	delete _parameters.segmentationThreshold;
}

void ExcitationFrequencyWindow::set_spin_array(Threedim* spinArray, int numberAtoms)
{
	/**
	@param[in] spinArray The array containing the spins displayed in GUI.
	@param[in] numberAtoms Number of spins
	*/
	
	_spinArray = spinArray;
	_numberAtoms = numberAtoms;
	delete[] _spinArrayBackup;
	// copy needed because _spinArray is constantly changed during visualization of eigenstates
	_spinArrayBackup = new Threedim[numberAtoms];
	for (int i = 0; i < numberAtoms; ++i)
	{
		_spinArrayBackup[i] = spinArray[i];
	}
}

void ExcitationFrequencyWindow::start_visualization()
{
	/**
	Start visualization of eigenstates in a separate thread.
	*/

	// only possible if not already running
	if (_visualizationThread == NULL && _evecs->size1 == 2 * _numberAtoms)
	{
		update_GUI(); // provide information of eigenstate being displayed

		ExcitationVisualization* excitationVisualization = new ExcitationVisualization(_spinArray, 
			_spinArrayBackup, _numberAtoms, &_evalues, _evecs, _parameters);
		
		_visualizationThread = new QThread();

		connect(_visualizationThread, SIGNAL(started()), excitationVisualization, SLOT(visualize()));

		connect(excitationVisualization, SIGNAL(send_repaint_request()), _oglWidget, SLOT(repaint()));
		
		connect(excitationVisualization, SIGNAL(send_save_image_request(int)), this,
			SLOT(receive_save_image_request(int)));

		connect(excitationVisualization, SIGNAL(send_points(std::vector<int>)), this,
			SLOT(receive_points(std::vector<int>)), Qt::DirectConnection);

		connect(excitationVisualization, SIGNAL(send_video_sequence_finished()), this,
			SLOT(receive_video_finished()));

		connect(_visualizationThread, &QThread::finished, excitationVisualization, &QObject::deleteLater);

		connect(excitationVisualization, SIGNAL(send_finished()), _visualizationThread, SLOT(quit()));

		connect(_visualizationThread, SIGNAL(finished()), _visualizationThread, SLOT(deleteLater()));

		connect(_visualizationThread, SIGNAL(destroyed()), this, SLOT(close()));
		
		excitationVisualization->moveToThread(_visualizationThread);
		_visualizationThread->start();
	}
}

void ExcitationFrequencyWindow::read_parameters(QSharedPointer<Configuration> &config)
{
	if (_ui.tableWidgetNumEigStates->item(0, 0))
	{
		config->_numEigenStates = _ui.tableWidgetNumEigStates->item(0, 0)->text().toDouble();
	}
}

void ExcitationFrequencyWindow::stop_visualization()
{
	/**
	Terminates running visualization and closes GUI window.
	*/

	if (_visualizationThread == NULL)
	{
		close();
	}
	else
	{
		*_parameters.terminate = 1;
	}
}

void ExcitationFrequencyWindow::on_button_eigenvalues(void)
{
	/**
	Open window to allow specification of file to read eigenvalues in from.
	*/

	QString fname = QFileDialog::getOpenFileName(this, tr("Eigenvalues"), QString::fromStdString(_workfolder)
		, "ALL files (*.*)");
	if (fname.size() != 0)
	{
		Functions::read_eigenvalues(fname.toStdString(), _evalues);

		// use folder of eigenvalues for potential output of png files of eigenstates
		QStringList list = fname.split("/");
		_outputFolder.clear();
		for (int i = 0; i < list.size() - 1; ++i)
		{
			_outputFolder.append(list[i] + "/");
		}

		if (_evecs != NULL)
		{
			start_visualization();
		}

		_ui.pushButton_Eigenvalues->setDisabled(TRUE);
	}
}

void ExcitationFrequencyWindow::on_button_eigenvectors(void)
{
	/**
	Open window to specify file for eigenvector read-in.
	*/
	QString fname;
	if (_outputFolder.size() > 1)
	{
		fname = QFileDialog::getOpenFileName(this, tr("Eigenvectors"), _outputFolder, "ALL files (*.*)");
	}
	else
	{
		fname = QFileDialog::getOpenFileName(this, tr("Eigenvectors"), QString::fromStdString(_workfolder)
			, "ALL files (*.*)");
	}
	if (fname.size() != 0)
	{
		_evecs = Functions::read_eigenvectors(fname.toStdString());

		if (_evalues.size() > 0)
		{
			start_visualization();
		}
		_ui.pushButton_Eigenvectors->setDisabled(TRUE);
	}
}

void ExcitationFrequencyWindow::on_button_next(void)
{
	/**
	Change to next eigenstate.
	*/

	// check if this action would not cause to cause overflow in eigenvalues and eigenvectors
	int minNumber = 0; 
	if ( _evecs->size2 > _evalues.size() )
	{
		minNumber = _evalues.size();
	}
	else
	{
		minNumber = _evecs->size2;
	}

	if (*_parameters.index < minNumber-1)
	{
		++*_parameters.index;
		*_parameters.rescale = 1;
		update_GUI();
	}
}

void ExcitationFrequencyWindow::on_button_previous(void)
{
	/**
	Show previous eigenstate if it is not already the first eigenstate.
	*/

	if (*_parameters.index > 0)
	{
		--*_parameters.index;
		*_parameters.rescale = 1;
		update_GUI();
	}
}

void ExcitationFrequencyWindow::on_button_pause(void)
{
	/**
	Pause or continue visualization of eigenstates.
	*/

	if (*_parameters.pause == FALSE)
	{
		*_parameters.pause = TRUE;
		_ui.pushButtonPause->setText("play");
	}
	else
	{
		*_parameters.pause = FALSE;
		_ui.pushButtonPause->setText("pause");
	}

}

void ExcitationFrequencyWindow::on_button_save_image_sequence(void)
{
	/**
	Trigger output of image sequence of currently visualized eigenstate.
	*/

	*_parameters.videoSequence = TRUE;
	_ui.pushButtonVideoSeq->setEnabled(FALSE);
	_currentSubfolder = _outputFolder + QString::number(_outputIndex); // create (hopefully) unique folder
#ifdef WIN
	_mkdir(_currentSubfolder.toStdString().c_str());
#elif LINUX
        mkdir(_currentSubfolder.toStdString().c_str(), 0777);
#endif
	_currentSubfolder.append("/" );
}

void ExcitationFrequencyWindow::on_change_in_table_widget_angle(int row, int column)
{
	/**
	Respond to user triggered rescale request of eigenvector.
	*/

	if (_ui.tableWidgetAngle->item(row, column))
	{
		*_parameters.maxAngle = _ui.tableWidgetAngle->item(row, column)->text().toDouble();
	}
	*_parameters.rescale = 1;
}

void ExcitationFrequencyWindow::on_change_in_table_widget_speed(int row, int column)
{
	/**
	Change delay time in visualization loop.
	*/

	if (_ui.tableWidgetSpeed->item(row, column))
	{
		*_parameters.sleepTime = _ui.tableWidgetSpeed->item(row, column)->text().toDouble();
	}
}

void ExcitationFrequencyWindow::on_change_in_table_widget_segmentation(int row, int column)
{
	if (_ui.tableWidgetSegmentation->item(row, column))
	{
		*_parameters.segmentationThreshold = _ui.tableWidgetSegmentation->item(row, column)->text().toDouble();
	}
}

void ExcitationFrequencyWindow::receive_save_image_request(int imageIndex)
{
	/**
	Store current state of openGl widget as png file and give it the index imageIndex.
	*/

	std::string name2 = Functions::get_three_digit_name(imageIndex);
	QString fname = _currentSubfolder + QString::fromStdString(name2) + ".png";
	_oglWidget->receive_save_request(fname);
	*_parameters.pause = FALSE;
}

void ExcitationFrequencyWindow::receive_points(std::vector<int> points)
{
	_oglWidget->_points.clear();
	for (int i = 0; i < points.size(); ++i)
	{
		_oglWidget->_points.push_back(points[i]);
	}
}

void ExcitationFrequencyWindow::receive_video_finished(void)
{
	/**
	When video sequence has been finished.
	*/

	_outputIndex += 1; // increment number of stored video sequences
	_ui.pushButtonVideoSeq->setEnabled(TRUE); // enable button for storage request of another video sequence
}

void ExcitationFrequencyWindow::setup_elements(void)
{
	/**
	Setup GUI elements.
	*/

	_ui.tableWidgetNumEigStates->setColumnCount(1);
	_ui.tableWidgetNumEigStates->setRowCount(1);
	_ui.tableWidgetNumEigStates->horizontalHeader()->hide();
	_ui.tableWidgetNumEigStates->verticalHeader()->setVisible(1);
	_ui.tableWidgetNumEigStates->setVerticalHeaderLabels(QString("NumEigModes;").split(";"));
	_ui.tableWidgetNumEigStates->custom_resize();
	_ui.tableWidgetNumEigStates->setItem(0, 0, new QTableWidgetItem(QString::number(5)));

	_ui.textEditEigInformation->setDisabled(1);

	_ui.tableWidgetAngle->setColumnCount(1);
	_ui.tableWidgetAngle->setRowCount(1);
	_ui.tableWidgetAngle->horizontalHeader()->hide();
	_ui.tableWidgetAngle->verticalHeader()->setVisible(1);
	_ui.tableWidgetAngle->setVerticalHeaderLabels(QString("alpha [deg];").split(";"));
	_ui.tableWidgetAngle->custom_resize();

	*_parameters.maxAngle = 30;
	_ui.tableWidgetAngle->setItem(0, 0, new QTableWidgetItem(QString::number(*_parameters.maxAngle)));

	_ui.tableWidgetSpeed->setColumnCount(1);
	_ui.tableWidgetSpeed->setRowCount(1);
	_ui.tableWidgetSpeed->horizontalHeader()->hide();
	_ui.tableWidgetSpeed->verticalHeader()->setVisible(1);
	_ui.tableWidgetSpeed->setVerticalHeaderLabels(QString("delay [ms];").split(";"));
	_ui.tableWidgetSpeed->custom_resize();

	_ui.tableWidgetSpeed->setItem(0, 0, new QTableWidgetItem(QString::number(*_parameters.sleepTime)));

	_ui.tableWidgetSegmentation->setColumnCount(1);
	_ui.tableWidgetSegmentation->setRowCount(1);
	_ui.tableWidgetSegmentation->horizontalHeader()->hide();
	_ui.tableWidgetSegmentation->verticalHeader()->setVisible(1);
	_ui.tableWidgetSegmentation->setVerticalHeaderLabels(QString("seg. [0,1];").split(";"));
	_ui.tableWidgetSegmentation->custom_resize();

	_ui.tableWidgetSegmentation->setItem(0, 0, new QTableWidgetItem(QString::number(*_parameters.segmentationThreshold)));

	connect(_ui.tableWidgetAngle, SIGNAL(cellChanged(int, int)), this,
		SLOT(on_change_in_table_widget_angle(int, int)));

	connect(_ui.tableWidgetSpeed, SIGNAL(cellChanged(int, int)), this,
		SLOT(on_change_in_table_widget_speed(int, int)));

	connect(_ui.tableWidgetSegmentation, SIGNAL(cellChanged(int, int)), this,
		SLOT(on_change_in_table_widget_segmentation(int, int)));
}

void ExcitationFrequencyWindow::update_GUI()
{
	/**
	Update GUI information about currently displayed eigenstate.
	*/

	_ui.textEditEigInformation->setText(QString::number(*_parameters.index) + " eval: " + 
		QString::number(_evalues[*_parameters.index]) );
}
