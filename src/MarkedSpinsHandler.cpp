/*
* MarkedSpinsHandler.cpp
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


#include "MarkedSpinsHandler.h"

#include "SpinOrientation.h"
#include "OGLWidget.h"
#include "MarkedSpinsRequestWindow.h"

#include <QMessageBox>

#include "MyMath.h"

#include "MainWindow.h"

MarkedSpinsHandler::MarkedSpinsHandler(MainWindow* mainWindow, OGLWidget* oglWidget)
{
	_mainWindow = mainWindow;
	_oglWidget = oglWidget;
	_requestWindow = NULL;
}

MarkedSpinsHandler::~MarkedSpinsHandler()
{
	delete _requestWindow;
}

void MarkedSpinsHandler::set_spin_orientation(QSharedPointer<SpinOrientation> spinOrientation, int boolNew)
{
	_spinOrientation = spinOrientation;
	if (boolNew)
	{
		_exchangeDefects.clear();
		_dmDefects.clear();
		_anisotropyDefects.clear();
	}
}

void MarkedSpinsHandler::process_spins(std::vector<int> indexes)
{
	if (!_mainWindow->simulation_running())
	{
		if (_requestWindow == NULL)
		{
			_requestWindow = new MarkedSpinsRequestWindow(this, _mainWindow);
			connect(_requestWindow, &QDialog::rejected,
				this, &MarkedSpinsHandler::request_window_rejected);
			_mainWindow->block_simulation_start();
		}
		else
		{
			int order = 0;
			double parameter = 0;
			_requestWindow->get_exchange_defect_parameters(order, parameter);
			set_as_exchange_defects(order, parameter);
			_requestWindow->get_dm_defect_parameters(order, parameter);
			set_as_dm_defects(order, parameter);
			UniaxialAnisotropyStruct anisotropy{ 0,Threedim{0,0,0} };
			_requestWindow->get_anisotropy_defect_parameters(anisotropy);
			set_as_anisotropy_defects(anisotropy);
			_requestWindow->clear_tables();
		}
		_tempIndexes = indexes;
		_requestWindow->set_indexes(indexes);
		_requestWindow->show();
	}
}

void MarkedSpinsHandler::lock_spins()
{
	if (_spinOrientation)
	{
		for (auto it = _tempIndexes.begin(); it != _tempIndexes.end(); ++it)
		{
			_spinOrientation->set_inactive_site(*it);
		}
		_oglWidget->set_wire_frame_spins(_tempIndexes);
		_oglWidget->repaint();
	}
}

void MarkedSpinsHandler::unlock_spins()
{
	if (_spinOrientation)
	{
		for (auto it = _tempIndexes.begin(); it != _tempIndexes.end(); ++it)
		{
			_spinOrientation->set_active_site(*it);
		}
		_oglWidget->set_filled_spins(_tempIndexes);
		_oglWidget->repaint();
	}
}

void MarkedSpinsHandler::set_direction(Threedim direction)
{
	if (_spinOrientation)
	{
		for (auto it = _tempIndexes.begin(); it != _tempIndexes.end(); ++it)
		{
			_spinOrientation->set_spin(direction,*it);
		}
		_oglWidget->repaint();
	}
}

void MarkedSpinsHandler::set_as_exchange_defects(int order, double parameter)
{
	if (order > 0 && order < 9 && abs(parameter) > PRECISION)
	{
		for (auto it = _tempIndexes.begin(); it != _tempIndexes.end(); ++it)
		{
			_exchangeDefects[order][*it] = parameter;
		}
		_oglWidget->set_wire_frame_spins(_tempIndexes);
		_oglWidget->repaint();
	}
}

void MarkedSpinsHandler::set_as_dm_defects(int order, double parameter)
{
	if (order > 0 && order < 6 && abs(parameter) > PRECISION)
	{
		for (auto it = _tempIndexes.begin(); it != _tempIndexes.end(); ++it)
		{
			_dmDefects[order][*it] = parameter;
		}
		_oglWidget->set_wire_frame_spins(_tempIndexes);
		_oglWidget->repaint();
	}
}

void MarkedSpinsHandler::set_as_anisotropy_defects(UniaxialAnisotropyStruct anisotropy)
{
	if (abs(anisotropy.energyParameter) > PRECISION && MyMath::norm(anisotropy.direction) > PRECISION)
	{
		for (auto it = _tempIndexes.begin(); it != _tempIndexes.end(); ++it)
		{
			_anisotropyDefects[*it] = anisotropy;
		}
		_oglWidget->set_wire_frame_spins(_tempIndexes);
		_oglWidget->repaint();
	}
}

void MarkedSpinsHandler::clear_defects_and_unlock_all(void)
{
	if (_requestWindow != NULL)
	{
		_requestWindow->clear_tables();
	}

	_exchangeDefects.clear();
	_dmDefects.clear();
	_anisotropyDefects.clear();

	for (int i = 0; i < _spinOrientation->get_number_atoms(); ++i)
	{
		_spinOrientation->set_active_site(i);
	}
	_oglWidget->set_spins_filled();
	_oglWidget->repaint();
}

void MarkedSpinsHandler::show_defects(void) const
{
	QMessageBox messageBox;
	QString qString("Defects: ");
	
	for (auto it = _exchangeDefects.begin(); it != _exchangeDefects.end(); ++it)
	{
		qString.append("\nJ" + QString::number((*it).first) + "(site,energy): ");
		for (auto it1 = (*it).second.begin(); it1 != (*it).second.end(); ++it1)
		{
			qString.append("(" + QString::number((*it1).first) + " " + QString::number((*it1).second) + "), ");
		}
	}
	for (auto it = _dmDefects.begin(); it != _dmDefects.end(); ++it)
	{
		qString.append("\nD" + QString::number((*it).first) + "(site,energy): ");
		for (auto it1 = (*it).second.begin(); it1 != (*it).second.end(); ++it1)
		{
			qString.append("(" + QString::number((*it1).first) + "," + QString::number((*it1).second) + "), ");
		}
	}
	if (_anisotropyDefects.size() > 0)
	{
		qString.append("\nK (site,energy,direction): ");
	}
	for (auto it = _anisotropyDefects.begin(); it != _anisotropyDefects.end(); ++it)
	{
		qString.append("(" + QString::number((*it).first) + "," + 
			QString::number((*it).second.energyParameter) + ",(" +
			QString::number((*it).second.direction.x) + "," +
			QString::number((*it).second.direction.y) + "," +
			QString::number((*it).second.direction.z) + ")), ");
	}
	messageBox.setText(qString);
	messageBox.exec();
}

void MarkedSpinsHandler::show_locked_spins(void) const
{
	QMessageBox messageBox;
	QString qString("Inactive sites: ");
	int* inactiveSites = _spinOrientation->get_inactive_sites();
	for (int i = 0; i < _spinOrientation->get_number_inactive_sites(); ++i)
	{
		qString.append(QString::number(inactiveSites[i]) + " ");
	}
	messageBox.setText(qString);
	messageBox.exec();
}

std::unordered_map<int, std::unordered_map<int, double>> MarkedSpinsHandler::get_exchange_defects(void)
{
	return _exchangeDefects;
}

std::unordered_map<int, std::unordered_map<int, double>> MarkedSpinsHandler::get_dm_defects(void)
{
	return _dmDefects;
}

std::unordered_map <int, UniaxialAnisotropyStruct> MarkedSpinsHandler::get_anisotropy_defects(void)
{
	return _anisotropyDefects;
}

void MarkedSpinsHandler::request_window_rejected(void)
{
	int order = 0;
	double parameter = 0;
	_requestWindow->get_exchange_defect_parameters(order, parameter);
	set_as_exchange_defects(order, parameter);
	_requestWindow->get_dm_defect_parameters(order, parameter);
	set_as_dm_defects(order, parameter);
	UniaxialAnisotropyStruct anisotropy{ 0,Threedim{ 0,0,0 } };
	_requestWindow->get_anisotropy_defect_parameters(anisotropy);
	set_as_anisotropy_defects(anisotropy);

	delete _requestWindow;
	_requestWindow = NULL;
	_oglWidget->delete_rubber_band();
	_mainWindow->unblock_simulation_start();
}
