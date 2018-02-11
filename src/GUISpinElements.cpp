/*
* GUISpinElements.cpp
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

#include "GUISpinElements.h"

// forward
#include "Configuration.h"
#include "SpinOrientation.h"
#include "FerromagnetWindow.h"
#include "SkyrmionWindow.h"
#include "SpinSpiralWindow.h"

#include "MyHeaderView.h"

GUISpinElements::GUISpinElements(Ui::QtMainWindow* ui)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/

	_ui = ui;
	_ferromagnetWindow = NULL;
	_skyrmionWindow = NULL;
	_spinSpiralWindow = NULL;
	setup_elements();
	set_default_values();

	_skyrmionParameters = { Twodim{0,0},1,5,0,1,1 };

	_latticeCoordinateArray = NULL;
	_spinArray = NULL;
	_spinArrayTemp = NULL;
	_numberAtoms = 0;
}

GUISpinElements::~GUISpinElements()
{
	delete _spinArrayTemp;
}

void GUISpinElements::setup_elements(void)
{
	_ui->tableWidgetMagneticMoment->setColumnCount(1);
	_ui->tableWidgetMagneticMoment->setRowCount(1);
	_ui->tableWidgetMagneticMoment->horizontalHeader()->hide();
	_ui->tableWidgetMagneticMoment->setVerticalHeader(new MyHeaderView(Qt::Vertical, 0.1));
	QString text("<i>");
	text.append(QChar(956));
	text.append("</i> [<i>");
	text.append(QChar(956));
	text.append("</i><font size=4><sub><font size=1> </font size=1>B</sub></font size=4>];");
	_ui->tableWidgetMagneticMoment->setVerticalHeaderLabels(text.split(";"));

	QTableWidgetItem* magneticMoment = new QTableWidgetItem();
	_ui->tableWidgetMagneticMoment->setItem(0, 0, magneticMoment);
	
}

void GUISpinElements::set_default_values(void)
{
	_ui->tableWidgetMagneticMoment->item(0, 0)->setText("1");
}

void GUISpinElements::read_parameters(QSharedPointer<Configuration> &config)
{
	if (_ui->tableWidgetMagneticMoment->item(0, 0))
	{
		config->_magneticMoment = _ui->tableWidgetMagneticMoment->item(0, 0)->text().toDouble();
	}
}

void GUISpinElements::open_ferromagnet_window(Threedim* latticeCoordArray, Threedim* spinArray, 
	int numberAtoms)
{
	close_all_spin_state_windows();

	_latticeCoordinateArray = latticeCoordArray;
	_spinArray = spinArray;
	_numberAtoms = numberAtoms;

	delete _spinArrayTemp;
	_spinArrayTemp = new Threedim[numberAtoms];

	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArrayTemp[i] = _spinArray[i];
	}

	_ferromagnetWindow = new FerromagnetWindow(this);
	_ferromagnetWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	connect(_ferromagnetWindow, &QObject::destroyed, this, 
		&GUISpinElements::ferromagnet_window_destroyed);
	_ferromagnetWindow->open();
}

void GUISpinElements::open_skyrmion_window(OGLWidget* oglWidget, Threedim* latticeCoordArray, 
	Threedim* spinArray, int numberAtoms)
{
	close_all_spin_state_windows();

	_latticeCoordinateArray = latticeCoordArray;
	_spinArray = spinArray;
	_numberAtoms = numberAtoms;

	delete _spinArrayTemp;
	_spinArrayTemp = new Threedim[numberAtoms];

	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArrayTemp[i] = _spinArray[i];
	}

	_skyrmionWindow = new SkyrmionWindow(this, _skyrmionParameters);
	_skyrmionWindow->setAttribute(Qt::WA_DeleteOnClose, true);

	connect(oglWidget, &OGLWidget::mouse_position_changed, _skyrmionWindow,
		&SkyrmionWindow::receive_skyrmion_center);
	connect(_skyrmionWindow, &SkyrmionWindow::destroyed, this, 
		&GUISpinElements::skyrmion_window_destroyed);

	_skyrmionWindow->open();
}

void GUISpinElements::open_spin_spiral_window(OGLWidget * oglWidget, Threedim * latticeCoordArray, 
	Threedim * spinArray, int numberAtoms)
{
	close_all_spin_state_windows();

	_latticeCoordinateArray = latticeCoordArray;
	_spinArray = spinArray;
	_numberAtoms = numberAtoms;

	delete _spinArrayTemp;
	_spinArrayTemp = new Threedim[numberAtoms];

	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArrayTemp[i] = _spinArray[i];
	}

	_spinSpiralWindow = new SpinSpiralWindow(this);
	_spinSpiralWindow->setAttribute(Qt::WA_DeleteOnClose, true);

	connect(oglWidget, &OGLWidget::mouse_position_changed, _spinSpiralWindow,
		&SpinSpiralWindow::receive_mouse_position);
	connect(_spinSpiralWindow, &QObject::destroyed, this, 
		&GUISpinElements::spin_spiral_window_destroyed);

	_spinSpiralWindow->open();
}

void GUISpinElements::random_spin_configuration(SpinOrientation* _spinOrientation)
{
	_spinOrientation->random_orientation();
	_spinOrientation->set_all_sites_active();
	_ui->openGLWidget->set_spins_filled();
	_ui->openGLWidget->repaint();
}

void GUISpinElements::ferromagnet(Threedim direction)
{
	for (int i = 0; i < _numberAtoms; i++)
	{

		_spinArray[i] = direction;
	}
	_ui->openGLWidget->repaint();
}

void GUISpinElements::skyrmion(SkyrmionWindowParameters parameters)
{
	_skyrmionParameters = parameters;
	double phi0 = 0;
	if (parameters.chiral == TRUE)
	{
		phi0 = Pi/2.;
	}

	Threedim skyrmionCenter = {parameters.center.x, parameters.center.y, 0};
	double distance = 0;
	Threedim diff = { 0,0,0 };
	for (int i = 0; i < _numberAtoms; i++)
	{
		diff = MyMath::difference(_latticeCoordinateArray[i], skyrmionCenter);
		distance = MyMath::norm(diff);
		if (distance < 2 * parameters.radius)
		{
			double phi = atan2(diff.y, diff.x);
			double theta = parameters.order*Pi* ( 1 - distance / (2 * parameters.radius));
			Threedim spin = { 0,0,0 };
			spin.x = parameters.handed*sin(theta)*cos(phi+phi0);
			spin.y = parameters.handed*sin(theta)*sin(phi+phi0);
			spin.z = cos(theta);
			_spinArray[i] = MyMath::normalize(MyMath::mult(spin,parameters.centerDir));
		}
	}
	_ui->openGLWidget->repaint();
}

void GUISpinElements::spin_spiral(Threedim kVector, Threedim position, int helicity)
{
	double angle = 0;
	double dotProd = 0;
	double normK = MyMath::norm(kVector);
	Threedim ek = MyMath::normalize(kVector);
	Threedim helpVec = { 0, 0, 0 };
	Threedim helpVec1 = { 0, 0 ,0 };
	Threedim ez = { 0, 0, 1 };
	int n = _numberAtoms;
	for (int i = 0; i < _numberAtoms; i++)
	{
		helpVec = MyMath::difference(_latticeCoordinateArray[i], position);
		dotProd = MyMath::dot_product(ek, helpVec);
		angle = normK * dotProd * helicity;
		helpVec = MyMath::mult(ez, cos(angle));
		helpVec1 = MyMath::mult(ek, sin(angle));
		helpVec = MyMath::add(helpVec, helpVec1);
		_spinArray[i] = MyMath::normalize(helpVec);
	}
	_ui->openGLWidget->repaint();
}

void GUISpinElements::restore_spin_configuration(void)
{
	for (int i = 0; i < _numberAtoms; i++)
	{
		_spinArray[i] = _spinArrayTemp[i];
	}
	_ui->openGLWidget->repaint();
}

void GUISpinElements::ferromagnet_window_destroyed(void)
{
	_ferromagnetWindow = NULL;
}

void GUISpinElements::skyrmion_window_destroyed(void)
{
	_skyrmionWindow = NULL;
}

void GUISpinElements::spin_spiral_window_destroyed(void)
{
	_spinSpiralWindow = NULL;
}

void GUISpinElements::close_all_spin_state_windows(void)
{
	if (_ferromagnetWindow != NULL)
	{
		_ferromagnetWindow->close();
	}

	if (_skyrmionWindow != NULL)
	{
		_skyrmionWindow->close();
	}

	if (_spinSpiralWindow != NULL)
	{
		_spinSpiralWindow->close();
	}
}
