/*
* GUIEnergyElements.cpp
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

#include "GUIEnergyElements.h"

// forward includes
#include "AnisotropyWindow.h"
#include "Configuration.h"
#include "MainWindow.h"

#include "MyHeaderView.h"

GUIEnergyElements::GUIEnergyElements(MainWindow* mw)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/

	_mw = mw;
	setup_elements();
	set_default_values();

	_anisoWindow = NULL;

	_anisotropies = std::make_shared<std::vector<UniaxialAnisotropyStruct>>();

	_hexAnisotropies = std::make_shared<std::vector<double>>();
	_hexAnisotropies->push_back(0);
	_hexAnisotropies->push_back(0);
	_hexAnisotropies->push_back(0);
}

void GUIEnergyElements::setup_elements(void)
{
	/**
	* The possible energies and corresponding energy parameters are specified here.
	*/
	std::vector<int> ratio;

	_mw->_toolbar->tableWidgetExchEnergies1->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_mw->_toolbar->tableWidgetExchEnergies1->setRowCount(1);
	_mw->_toolbar->tableWidgetExchEnergies1->setColumnCount(5);
	_mw->_toolbar->tableWidgetExchEnergies1->verticalHeader()->hide();
	_mw->_toolbar->tableWidgetExchEnergies1->setHorizontalHeaderLabels(
		QString("<i>J</i><font size=4><sub> 1</sub>;<i>J</i><font size=4><sub> 2</sub>;\
<i>J</i><font size=4><sub> 3</sub>;<i>J</i><font size=4><sub> 4</sub>;\
<i>J</i><font size=4><sub> 5</sub>").split(";"));
	ratio.push_back(68);
	ratio.push_back(68);
	ratio.push_back(68);
	ratio.push_back(70);
	ratio.push_back(74);
	_mw->_toolbar->tableWidgetExchEnergies1->set_column_ratio(ratio);
	_mw->_toolbar->tableWidgetExchEnergies1->set_horizontal_header_height(40);
	ratio.clear();
	ratio.push_back(40);
	_mw->_toolbar->tableWidgetExchEnergies1->set_row_ratio(ratio);

	_mw->_toolbar->tableWidgetExchEnergies2->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_mw->_toolbar->tableWidgetExchEnergies2->setRowCount(1);
	_mw->_toolbar->tableWidgetExchEnergies2->setColumnCount(6);
	_mw->_toolbar->tableWidgetExchEnergies2->verticalHeader()->hide();
	_mw->_toolbar->tableWidgetExchEnergies2->setHorizontalHeaderLabels(
		QString("<i>J</i><font size=4><sub> 6</sub>;<i>J</i><font size=4><sub> 7</sub>;\
<i>J</i><font size=4><sub> 8</sub>;BiQ;4Spin;3Spin").split(";"));
	ratio.clear();
	ratio.push_back(58);
	ratio.push_back(58);
	ratio.push_back(58);
	ratio.push_back(60);
	ratio.push_back(64);
	ratio.push_back(50);
	_mw->_toolbar->tableWidgetExchEnergies2->set_column_ratio(ratio);
	_mw->_toolbar->tableWidgetExchEnergies2->set_horizontal_header_height(45);
	ratio.clear();
	ratio.push_back(40);
	_mw->_toolbar->tableWidgetExchEnergies2->set_row_ratio(ratio);

	_mw->_toolbar->tableWidgetDMEnergy->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_mw->_toolbar->tableWidgetDMEnergy->setColumnCount(5);
	_mw->_toolbar->tableWidgetDMEnergy->setRowCount(1);
	_mw->_toolbar->tableWidgetDMEnergy->verticalHeader()->hide();
	_mw->_toolbar->tableWidgetDMEnergy->setHorizontalHeaderLabels(
		QString("<i>D</i><font size=4><sub>1</sub>;<i>D</i><font size=4><sub>2</sub>;\
<i>D</i><font size=4><sub>3</sub>;<i>D</i><font size=4><sub>4</sub>;\
<i>D</i><font size=4><sub>5</sub>").split(";"));

	ratio.clear();
	ratio.push_back(87);
	ratio.push_back(87);
	ratio.push_back(87);
	ratio.push_back(87);
	ratio.push_back(87);
	_mw->_toolbar->tableWidgetDMEnergy->set_column_ratio(ratio);
	_mw->_toolbar->tableWidgetDMEnergy->set_horizontal_header_height(40);
	ratio.clear();
	ratio.push_back(40);
	_mw->_toolbar->tableWidgetDMEnergy->set_row_ratio(ratio);

	connect(_mw->_toolbar->pushButtonNeel, &QAbstractButton::released, 
		this, &GUIEnergyElements::push_button_neel);
	connect(_mw->_toolbar->pushButtonChiral, &QAbstractButton::released, 
		this, &GUIEnergyElements::push_button_chiral);
}

void GUIEnergyElements::set_default_values(void)
{
	/**
	* Set default values for energy parameters
	*/
	QString styleSheet = "background: rgb(255, 254, 242)";
	_mw->_toolbar->pushButtonNeel->setStyleSheet(styleSheet);
	_dmType = Neel;
}

void GUIEnergyElements::read_parameters(const std::shared_ptr<Configuration> &config)
{
	/**
	Determines the energy parameters from the GUI.

	@param[in] config Object to store parameters in.
	*/

	for (int i = 0; i < 5; ++i)
	{
		if (_mw->_toolbar->tableWidgetExchEnergies1->item(0, i))
		{
			double energy = _mw->_toolbar->tableWidgetExchEnergies1->item(0, i)->text().toDouble();
			config->_exchangeEnergies.push_back({ energy, i + 1 });
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (_mw->_toolbar->tableWidgetExchEnergies2->item(0, i))
		{
			double energy = _mw->_toolbar->tableWidgetExchEnergies2->item(0, i)->text().toDouble();
			config->_exchangeEnergies.push_back({ energy, i + 6 });
		}
	}

	if (_mw->_toolbar->tableWidgetExchEnergies2->item(0, 3))
	{
		config->_biQuadraticEnergy = _mw->_toolbar->tableWidgetExchEnergies2->item(0, 3)->text().toDouble();
	}

	if (_mw->_toolbar->tableWidgetExchEnergies2->item(0, 4))
	{
		config->_fourSpinEnergy = _mw->_toolbar->tableWidgetExchEnergies2->item(0, 4)->text().toDouble();
	}

	if (_mw->_toolbar->tableWidgetExchEnergies2->item(0, 5))
	{
		config->_threeSiteEnergy = _mw->_toolbar->tableWidgetExchEnergies2->item(0, 5)->text().toDouble();
	}

	for (int i = 0; i < 5; ++i)
	{
		if (_mw->_toolbar->tableWidgetDMEnergy->item(0, i))
		{
			double energy = _mw->_toolbar->tableWidgetDMEnergy->item(0, i)->text().toDouble();
			config->_DMEnergies.push_back({ energy, i+1});
		}
	}
	config->_dmType = _dmType;

	if (_mw->_toolbar->checkBoxDipol->isChecked())
	{
		config->_dipolEnergy = TRUE;
	}

	for (int i = 0; i < _anisotropies->size(); ++i)
	{
		config->_uniaxialAnisotropyEnergies.push_back((*_anisotropies)[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		config->_hexagonalAnisotropyEnergies[i] = (*_hexAnisotropies)[i];
	}

	MagneticFieldStruct magneticField = { 0, 0, 1,{ 0,0,1 } };
	double magneticMoment = config->_magneticMoment;
	if (_mw->_toolbar->tableWidgetMagneticMoment->item(0, 0))
	{
		magneticMoment = _mw->_toolbar->tableWidgetMagneticMoment->item(0, 0)->text().toDouble();
	}
	if (_mw->_toolbar->tableWidgetMagneticField->item(0, 0))
	{
		magneticField.start = _mw->_toolbar->tableWidgetMagneticField->item(0, 0)->text().toDouble();
		magneticField.start *= magneticMoment*muBohr;
	}
	if (_mw->_toolbar->tableWidgetMagneticField->item(0, 1))
	{
		magneticField.end = _mw->_toolbar->tableWidgetMagneticField->item(0, 1)->text().toDouble();
		magneticField.end *= magneticMoment*muBohr;
	}
	if (_mw->_toolbar->tableWidgetMagneticField->item(0, 2))
	{
		magneticField.steps = _mw->_toolbar->tableWidgetMagneticField->item(0, 2)->text().toInt();
	}
	if (_mw->_toolbar->tableWidgetMagneticField->item(0, 3) && _mw->_toolbar->tableWidgetMagneticField->item(0, 4)
		&& _mw->_toolbar->tableWidgetMagneticField->item(0, 5))
	{
		Threedim direction = { 0,0,0 };
		direction.x = _mw->_toolbar->tableWidgetMagneticField->item(0, 3)->text().toDouble();
		direction.y = _mw->_toolbar->tableWidgetMagneticField->item(0, 4)->text().toDouble();
		direction.z = _mw->_toolbar->tableWidgetMagneticField->item(0, 5)->text().toDouble();
		magneticField.direction = direction;
	}
	config->_magneticField = magneticField;
}

void GUIEnergyElements::open_anisotropy_window(void)
{
	/**
	* Window to add anisotropies.
	*/

	if (_anisoWindow == NULL)
	{
		_anisoWindow = new AnisotropyWindow(_hexAnisotropies, _anisotropies);
		_anisoWindow->setAttribute(Qt::WA_DeleteOnClose, true);
		connect(_anisoWindow, &QObject::destroyed, this, &GUIEnergyElements::anisotropy_window_destroyed);
		_anisoWindow->open();
	}
	else
	{
		_anisoWindow->raise();
	}
}

void GUIEnergyElements::push_button_neel(void)
{
	QString styleSheet = "background: rgb(255, 254, 242)";
	_mw->_toolbar->pushButtonNeel->setStyleSheet(styleSheet);
	_dmType = Neel;

	styleSheet = "background: rgb(242, 242, 242)";
	_mw->_toolbar->pushButtonChiral->setStyleSheet(styleSheet);
}

void GUIEnergyElements::push_button_chiral(void)
{
	QString styleSheet = "background: rgb(255, 254, 242)";
	_mw->_toolbar->pushButtonChiral->setStyleSheet(styleSheet);
	_dmType = Chiral;

	styleSheet = "background: rgb(242, 242, 242)";
	_mw->_toolbar->pushButtonNeel->setStyleSheet(styleSheet);
}

void GUIEnergyElements::anisotropy_window_destroyed(void)
{
	_anisoWindow = NULL;
}
