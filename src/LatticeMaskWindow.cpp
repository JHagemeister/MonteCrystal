/*
* LatticeMaskWindow.cpp
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

#include "LatticeMaskWindow.h"

#include "GUILatticeElements.h"

LatticeMaskWindow::LatticeMaskWindow(GUILatticeElements* guiLatticeElements, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_guiLatticeElements = guiLatticeElements;
	_latticeMaskParameters = _guiLatticeElements->_latticeMaskParameters;

	ui.tableWidget_Dimensions->setColumnCount(2);
	ui.tableWidget_Dimensions->setRowCount(1);
	ui.tableWidget_Dimensions->verticalHeader()->hide();
	ui.tableWidget_Dimensions->horizontalHeader()->setVisible(1);
	ui.tableWidget_Dimensions->setHorizontalHeaderLabels(QString("height [a];width [a]").split(";"));
	ui.tableWidget_Dimensions->custom_resize();
	ui.tableWidget_Dimensions->setItem(0, 0, new QTableWidgetItem());
	ui.tableWidget_Dimensions->setItem(0, 1, new QTableWidgetItem());

	ui.comboBox_Type->addItem(tr("square"));
	ui.comboBox_Type->addItem(tr("triangular"));

	connect(ui.pushButton_Ok, &QAbstractButton::released, this, &LatticeMaskWindow::ok_button);
}

LatticeMaskWindow::~LatticeMaskWindow()
{
}

void LatticeMaskWindow::read_parameters(void)
{
	if (ui.tableWidget_Dimensions->item(0, 0))
	{
		_latticeMaskParameters.height = ui.tableWidget_Dimensions->item(0, 0)->text().toDouble();
	}
	if (ui.tableWidget_Dimensions->item(0, 1))
	{
		_latticeMaskParameters.width = ui.tableWidget_Dimensions->item(0, 1)->text().toDouble();
	}

	QString qString = ui.comboBox_Type->currentText();

	if (qString.compare("square") == 0)
	{
		_latticeMaskParameters.latticeType = squareLattice;
	}
	if (qString.compare("triangular") == 0)
	{
		_latticeMaskParameters.latticeType = hexagonalLattice;
	}
}

void LatticeMaskWindow::ok_button(void)
{
	read_parameters();
	_guiLatticeElements->_latticeMaskParameters = _latticeMaskParameters;
	this->close();
}
