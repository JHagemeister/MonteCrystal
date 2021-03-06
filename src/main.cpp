/*
* main.cpp
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

//#include <vld.h>

#include "MainWindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>
///Contains the entry of the MonteCrystal program with a GUI.
int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
    std::cout << "Tried to set OGL version 3.3. Obtained version: "
		<< format.majorVersion() << "." << format.minorVersion() << std::endl;
	QSurfaceFormat::setDefaultFormat(format);

	MainWindow w;
	w.setWindowTitle("Monte Crystal 3.2.0");
	w.showMaximized();
	w.read_workfolder();
	return a.exec();
}
