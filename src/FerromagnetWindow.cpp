/*
* FerromagnetWindow.cpp
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

#include "FerromagnetWindow.h"

#include "GUISpinElements.h"
#include "MyMath.h"

FerromagnetWindow::FerromagnetWindow(GUISpinElements* guiSpinElements, QWidget * parent) : QDialog(parent) 
{
	_ui.setupUi(this);

	_guiSpinElements = guiSpinElements;

	_direction = { 0, 0, 1 };

	_ui.tableWidget->setColumnCount(3);
	_ui.tableWidget->setRowCount(1);
	_ui.tableWidget->verticalHeader()->hide();
	_ui.tableWidget->horizontalHeader()->setVisible(1);
	_ui.tableWidget->setHorizontalHeaderLabels(QString("dir_x;dir_y;dir_z").split(";"));
	_ui.tableWidget->custom_resize();

	QTableWidgetItem* dir_x = new QTableWidgetItem();
	dir_x->setText(QString::number(_direction.x));
	_ui.tableWidget->setItem(0, 0, dir_x);
	QTableWidgetItem* dir_y = new QTableWidgetItem();
	dir_y->setText(QString::number(_direction.y));
	_ui.tableWidget->setItem(0, 1, dir_y);
	QTableWidgetItem* dir_z = new QTableWidgetItem();
	dir_z->setText(QString::number(_direction.z));
	_ui.tableWidget->setItem(0, 2, dir_z);

	connect(_ui.pushButtonApply, &QAbstractButton::released, this, &FerromagnetWindow::push_button_apply);
	connect(_ui.pushButtonUndo, &QAbstractButton::released, this, &FerromagnetWindow::push_button_undo);
	connect(_ui.pushButtonOk, &QAbstractButton::released, this, &QDialog::close);
}

FerromagnetWindow::~FerromagnetWindow() 
{
	
}

void FerromagnetWindow::push_button_apply(void)
{
	read_parameters();
	_direction = MyMath::normalize(_direction);
	_guiSpinElements->ferromagnet(_direction);
}

void FerromagnetWindow::push_button_undo(void)
{
	_guiSpinElements->restore_spin_configuration();
}

void FerromagnetWindow::read_parameters(void)
{
	if (_ui.tableWidget->item(0, 0))
	{
		_direction.x = _ui.tableWidget->item(0, 0)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 1))
	{
		_direction.y = _ui.tableWidget->item(0, 1)->text().toDouble();
	}
	if (_ui.tableWidget->item(0, 2))
	{
		_direction.z = _ui.tableWidget->item(0, 2)->text().toDouble();
	}
}
