/*
* SpinSpiralWindow.cpp
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

#include "SpinSpiralWindow.h"

#include "GUISpinElements.h"
#include "MyMath.h"

SpinSpiralWindow::SpinSpiralWindow(GUISpinElements* guiSpinElements, QWidget * parent): 
	QDialog(parent)
{
	/**
	* Window to specify spin spiral parameters
	*/

	_ui.setupUi(this);

	_guiSpinElements = guiSpinElements;

	_position = { 0, 0 }; // position coordinates
	_helicity = 1; // +1 or -1
	_lambda = 10; // spin spiral period
	_direction = { 1, 0, 0 }; // propagation direction of spin spiral

	// Setup GUI elements and set default values:
	_ui.tableWidget->setColumnCount(6);
	_ui.tableWidget->setRowCount(1);
	_ui.tableWidget->verticalHeader()->hide();
	_ui.tableWidget->horizontalHeader()->setVisible(1);
	_ui.tableWidget->setHorizontalHeaderLabels(QString("px;py;lambda;dir_x;dir_y;dir_z").split(";"));
	_ui.tableWidget->custom_resize();
	QTableWidgetItem* xPos = new QTableWidgetItem();
	xPos->setText("0");
	_ui.tableWidget->setItem(0, 0, xPos);
	QTableWidgetItem* yPos = new QTableWidgetItem();
	yPos->setText("0");
	_ui.tableWidget->setItem(0, 1, yPos);
	QTableWidgetItem* lambda = new QTableWidgetItem();
	lambda->setText("10");
	_ui.tableWidget->setItem(0, 2, lambda);
	QTableWidgetItem* dir_x = new QTableWidgetItem();
	dir_x->setText("1");
	_ui.tableWidget->setItem(0, 3, dir_x);
	QTableWidgetItem* dir_y = new QTableWidgetItem();
	dir_y->setText("0");
	_ui.tableWidget->setItem(0, 4, dir_y);
	QTableWidgetItem* dir_z = new QTableWidgetItem();
	dir_z->setText("0");
	_ui.tableWidget->setItem(0, 5, dir_z);

	_ui.comboBoxHelicity->addItem((tr("helicity +1")));
	_ui.comboBoxHelicity->addItem((tr("helicity -1")));
}

SpinSpiralWindow::~SpinSpiralWindow() 
{
	
}

void SpinSpiralWindow::on_push_button_apply(void)
{
	/**
	* Apply spin spiral state according to specified parameters
	*/

	read_parameters(); // read GUI parameters
	Threedim position = {_position.x, _position.y, 0};
	_direction = MyMath::normalize(_direction); // ensures that _direction is indeed only direction vector

	// create k-vector from direction vector and spin spiral period
	Threedim kVector = { _direction.x * 2 * Pi / _lambda, _direction.y * 2 * Pi / _lambda,
						_direction.z * 2 * Pi / _lambda };

	_guiSpinElements->spin_spiral(kVector, position, _helicity);
}

void SpinSpiralWindow::on_push_button_undo(void)
{
	/**
	* Restore initial spin configuration.
	*/
	_guiSpinElements->restore_spin_configuration();
}

void SpinSpiralWindow::receive_mouse_position(Twodim position)
{
	/**
	* Receive position of mouse click. two-dimensional coordinates since openGL widget is two-dimensional.
	*/

	_ui.tableWidget->item(0, 0)->setText(QString::number(position.x)); // show on GUI
	_ui.tableWidget->item(0, 1)->setText(QString::number(position.y)); // show on GUI
	_position = position;
	this->raise();
}

void SpinSpiralWindow::read_parameters(void)
{
	/**
	* Read user specified parameters.
	*/

	_direction.z = 0;
	if (_ui.tableWidget->item(0, 0))
	{
		_position.x = _ui.tableWidget->item(0, 0)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 1))
	{
		_position.y = _ui.tableWidget->item(0, 1)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 2))
	{
		_lambda = _ui.tableWidget->item(0, 2)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 3))
	{
		_direction.x = _ui.tableWidget->item(0, 3)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 4))
	{
		_direction.y = _ui.tableWidget->item(0, 4)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 5))
	{
	}

	if (_ui.comboBoxHelicity->currentText().compare("helicity +1") == 0)
	{
		_helicity = 1;
	}
	if (_ui.comboBoxHelicity->currentText().compare("helicity -1") == 0)
	{
		_helicity = -1;
	}
}
