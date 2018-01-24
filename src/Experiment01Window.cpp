/*
* Experiment01Window.cpp
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

#include "Experiment01Window.h"

Experiment01Window::Experiment01Window(QWidget *parent)
	: QDialog(parent)
{
	_ui.setupUi(this);

	_ui.tableWidget->setColumnCount(1);
	_ui.tableWidget->setRowCount(1);
	_ui.tableWidget->verticalHeader()->hide();
	_ui.tableWidget->horizontalHeader()->setVisible(1);
	_ui.tableWidget->setHorizontalHeaderLabels(QString("frequency [1/SimStep];").split(";"));
	_ui.tableWidget->custom_resize();
}

Experiment01Window::~Experiment01Window()
{

}

Experiment01Struct Experiment01Window::read_parameters(void)
{
	/**
	* Reads and returns user specified parameters from window.
	*
	* @return Experiment01Struct Structure defined to contain all parameters needed for the specialized 
	*                            simulation.
	*/

	Experiment01Struct experiment01 = { 0 };
	
	if (_ui.tableWidget->item(0, 0)) // rotation frequency
	{
		experiment01.freq = _ui.tableWidget->item(0, 0)->text().toDouble();
	}
	return experiment01;
}