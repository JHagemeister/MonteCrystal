/*
* SkyrmionWindow.cpp
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

#include "SkyrmionWindow.h"

#include "GUISpinElements.h"

SkyrmionWindow::SkyrmionWindow(GUISpinElements* guiSpinElements, SkyrmionWindowParameters parameters, QWidget* parent):
	QDialog(parent)
{
	_ui.setupUi(this);

	_guiSpinElements = guiSpinElements;

	_parameters = parameters;

	// setup of GUI elements:
	_ui.tableWidget_Skyrmion->setColumnCount(3);
	_ui.tableWidget_Skyrmion->setRowCount(1);
	_ui.tableWidget_Skyrmion->verticalHeader()->hide();
	_ui.tableWidget_Skyrmion->horizontalHeader()->setVisible(1);
	_ui.tableWidget_Skyrmion->setHorizontalHeaderLabels(QString("x;y;r").split(";"));
	_ui.tableWidget_Skyrmion->custom_resize();

	QTableWidgetItem* xPos = new QTableWidgetItem();
	xPos->setText(QString::number(_parameters.center.x));
	_ui.tableWidget_Skyrmion->setItem(0, 0, xPos);
	QTableWidgetItem* yPos = new QTableWidgetItem();
	yPos->setText(QString::number(_parameters.center.y));
	_ui.tableWidget_Skyrmion->setItem(0, 1, yPos);
	QTableWidgetItem* radius = new QTableWidgetItem();
	radius->setText(QString::number(_parameters.radius));
	_ui.tableWidget_Skyrmion->setItem(0, 2, radius);

	_ui.tableWidget_order->setColumnCount(1);
	_ui.tableWidget_order->setRowCount(1);
	_ui.tableWidget_order->verticalHeader()->hide();
	_ui.tableWidget_order->horizontalHeader()->setVisible(1);
	_ui.tableWidget_order->setHorizontalHeaderLabels(QString("k-pi;").split(";"));
	_ui.tableWidget_order->custom_resize();
	_ui.tableWidget_order->setItem(0, 0, new QTableWidgetItem(QString::number(parameters.order)));

	_ui.comboBox_CenterDirection->addItem((tr("up (+z)")));
	_ui.comboBox_CenterDirection->addItem((tr("down (-z)")));

	_ui.comboBox_Type->addItem((tr("Neel")));
	_ui.comboBox_Type->addItem((tr("Chiral")));

	_ui.comboBox_Handedness->addItem((tr("right-handed")));
	_ui.comboBox_Handedness->addItem((tr("left-handed")));

	if (_parameters.centerDir == 1)
	{
		_ui.comboBox_CenterDirection->setCurrentIndex(1);
	}

	if (_parameters.chiral == 1)
	{
		_ui.comboBox_Type->setCurrentIndex(1);
	}

	if (_parameters.handed == -1)
	{
		_ui.comboBox_Handedness->setCurrentIndex(1);
	}

	connect(_ui.pushButton_undo, &QAbstractButton::released, this, &SkyrmionWindow::push_button_undo);
	connect(_ui.pushButton_apply, &QAbstractButton::released, this, &SkyrmionWindow::push_button_apply);
	connect(_ui.pushButton_ok, &QAbstractButton::released, this, &QDialog::close);
}

SkyrmionWindow::~SkyrmionWindow()
{
	
}

void SkyrmionWindow::push_button_apply()
{
	read_parameters();
	_guiSpinElements->skyrmion(_parameters);
}

void SkyrmionWindow::push_button_undo()
{
	_guiSpinElements->restore_spin_configuration();
}

void SkyrmionWindow::read_parameters()
{
	if (_ui.tableWidget_Skyrmion->item(0, 0))
	{
		_parameters.center.x = _ui.tableWidget_Skyrmion->item(0, 0)->text().toDouble();
	}
	if (_ui.tableWidget_Skyrmion->item(0, 1))
	{
		_parameters.center.y = _ui.tableWidget_Skyrmion->item(0, 1)->text().toDouble();
	}
	if (_ui.tableWidget_Skyrmion->item(0, 2))
	{
		_parameters.radius = _ui.tableWidget_Skyrmion->item(0, 2)->text().toDouble();
	}

	if (_ui.comboBox_CenterDirection->currentText().compare("up (+z)") == 0)
	{
		_parameters.centerDir = -1;
	}
	if (_ui.comboBox_CenterDirection->currentText().compare("down (-z)") == 0)
	{
		_parameters.centerDir = 1;
	}

	if (_ui.comboBox_Type->currentText().compare("Neel") == 0)
	{
		_parameters.chiral = 0;
	}
	if (_ui.comboBox_Type->currentText().compare("Chiral") == 0)
	{
		_parameters.chiral = 1;
	}

	if (_ui.comboBox_Handedness->currentText().compare("right-handed") == 0)
	{
		_parameters.handed = 1;
	}
	if (_ui.comboBox_Handedness->currentText().compare("left-handed") == 0)
	{
		_parameters.handed = -1;
	}

	if (_ui.tableWidget_order->item(0, 0))
	{
		_parameters.order = _ui.tableWidget_order->item(0, 0)->text().toInt();
	}
}

void SkyrmionWindow::receive_skyrmion_center(Twodim center)
{
	_ui.tableWidget_Skyrmion->item(0, 0)->setText(QString::number(center.x)); // show on GUI
	_ui.tableWidget_Skyrmion->item(0, 1)->setText(QString::number(center.y)); // show on GUI
	this->raise();
}
