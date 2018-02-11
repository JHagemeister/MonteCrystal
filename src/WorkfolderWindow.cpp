/*
* WorkfolderWindow.cpp
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

#include "WorkfolderWindow.h"

WorkfolderWindow::WorkfolderWindow(QWidget *parent) : QDialog(parent)
{
	_ui.setupUi(this);
	connect(_ui.pushButtonSelectOther, &QAbstractButton::released, this, &WorkfolderWindow::push_button_select_other);
	connect(_ui.pushButtonOk, &QAbstractButton::released, this, &WorkfolderWindow::close);
	connect(_ui.pushButtonSelectOther, &QAbstractButton::released, this, &WorkfolderWindow::close);
}

WorkfolderWindow::~WorkfolderWindow()
{
	
}

void WorkfolderWindow::set_text_edit(QString qString)
{
	_ui.textEdit->setText("Workfolder: " + qString);
}

void WorkfolderWindow::push_button_select_other(void)
{
	emit send_select_other();
}
