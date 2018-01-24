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

WorkfolderWindow::WorkfolderWindow(QWidget * parent) : QDialog(parent)
{
	ui.setupUi(this);
}

WorkfolderWindow::~WorkfolderWindow()
{
	
}

void WorkfolderWindow::set_text_edit(QString qString)
{
	ui.textEdit->setText("Workfolder: " + qString);
}

void WorkfolderWindow::on_push_button_select_other(void)
{
	emit send_select_other();
}
