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

#include "MyHeaderView.h"

GUIEnergyElements::GUIEnergyElements(Ui::QtMainWindow* ui)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/

	_ui = ui;
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

	_ui->tableWidgetExchEnergies1->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_ui->tableWidgetExchEnergies1->setRowCount(1);
	_ui->tableWidgetExchEnergies1->setColumnCount(5);
	_ui->tableWidgetExchEnergies1->verticalHeader()->hide();
	_ui->tableWidgetExchEnergies1->setHorizontalHeaderLabels(
		QString("<i>J</i><font size=4><sub> 1</sub>;<i>J</i><font size=4><sub> 2</sub>;\
<i>J</i><font size=4><sub> 3</sub>;<i>J</i><font size=4><sub> 4</sub>;\
<i>J</i><font size=4><sub> 5</sub>").split(";"));
	ratio.push_back(68);
	ratio.push_back(68);
	ratio.push_back(68);
	ratio.push_back(70);
	ratio.push_back(74);
	_ui->tableWidgetExchEnergies1->set_column_ratio(ratio);
	_ui->tableWidgetExchEnergies1->set_horizontal_header_height(40);
	ratio.clear();
	ratio.push_back(40);
	_ui->tableWidgetExchEnergies1->set_row_ratio(ratio);

	_ui->tableWidgetExchEnergies2->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_ui->tableWidgetExchEnergies2->setRowCount(1);
	_ui->tableWidgetExchEnergies2->setColumnCount(5);
	_ui->tableWidgetExchEnergies2->verticalHeader()->hide();
	_ui->tableWidgetExchEnergies2->setHorizontalHeaderLabels(
		QString("<i>J</i><font size=4><sub> 6</sub>;<i>J</i><font size=4><sub> 7</sub>;\
<i>J</i><font size=4><sub> 8</sub>;BiQ;4Spin").split(";"));
	ratio.clear();
	ratio.push_back(68);
	ratio.push_back(68);
	ratio.push_back(68);
	ratio.push_back(70);
	ratio.push_back(74);
	_ui->tableWidgetExchEnergies2->set_column_ratio(ratio);
	_ui->tableWidgetExchEnergies2->set_horizontal_header_height(45);
	ratio.clear();
	ratio.push_back(40);
	_ui->tableWidgetExchEnergies2->set_row_ratio(ratio);

	_ui->tableWidgetDMEnergy->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_ui->tableWidgetDMEnergy->setColumnCount(5);
	_ui->tableWidgetDMEnergy->setRowCount(1);
	_ui->tableWidgetDMEnergy->verticalHeader()->hide();
	_ui->tableWidgetDMEnergy->setHorizontalHeaderLabels(
		QString("<i>D</i><font size=4><sub>1</sub>;<i>D</i><font size=4><sub>2</sub>;\
<i>D</i><font size=4><sub>3</sub>;<i>D</i><font size=4><sub>4</sub>;\
<i>D</i><font size=4><sub>5</sub>").split(";"));

	ratio.clear();
	ratio.push_back(87);
	ratio.push_back(87);
	ratio.push_back(87);
	ratio.push_back(87);
	ratio.push_back(87);
	_ui->tableWidgetDMEnergy->set_column_ratio(ratio);
	_ui->tableWidgetDMEnergy->set_horizontal_header_height(40);
	ratio.clear();
	ratio.push_back(40);
	_ui->tableWidgetDMEnergy->set_row_ratio(ratio);

	connect(_ui->pushButtonNeel, SIGNAL(released()), this, SLOT(on_push_button_neel()));
	connect(_ui->pushButtonChiral, SIGNAL(released()), this, SLOT(on_push_button_chiral()));
}

void GUIEnergyElements::set_default_values(void)
{
	/**
	* Set default values for energy parameters
	*/
	QString styleSheet = "background: rgb(255, 254, 242)";
	_ui->pushButtonNeel->setStyleSheet(styleSheet);
	_dmType = Neel;
}

void GUIEnergyElements::read_parameters(QSharedPointer<Configuration> &config)
{
	/**
	Determines the energy parameters from the GUI.

	@param[in] config Object to store parameters in.
	*/

	for (int i = 0; i < 5; ++i)
	{
		if (_ui->tableWidgetExchEnergies1->item(0, i))
		{
			double energy = _ui->tableWidgetExchEnergies1->item(0, i)->text().toDouble();
			config->_exchangeEnergies.push_back({ energy, i + 1 });
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (_ui->tableWidgetExchEnergies2->item(0, i))
		{
			double energy = _ui->tableWidgetExchEnergies2->item(0, i)->text().toDouble();
			config->_exchangeEnergies.push_back({ energy, i + 6 });
		}
	}

	if (_ui->tableWidgetExchEnergies2->item(0, 3))
	{
		config->_biQuadraticEnergy = _ui->tableWidgetExchEnergies2->item(0, 3)->text().toDouble();
	}

	if (_ui->tableWidgetExchEnergies2->item(0, 4))
	{
		config->_fourSpinEnergy = _ui->tableWidgetExchEnergies2->item(0, 4)->text().toDouble();
	}

	for (int i = 0; i < 5; ++i)
	{
		if (_ui->tableWidgetDMEnergy->item(0, i))
		{
			double energy = _ui->tableWidgetDMEnergy->item(0, i)->text().toDouble();
			config->_DMEnergies.push_back({ energy, i+1});
		}
	}
	config->_dmType = _dmType;

	if (_ui->checkBoxDipol->isChecked())
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
	if (_ui->tableWidgetMagneticMoment->item(0, 0))
	{
		magneticMoment = _ui->tableWidgetMagneticMoment->item(0, 0)->text().toDouble();
	}
	if (_ui->tableWidgetMagneticField->item(0, 0))
	{
		magneticField.start = _ui->tableWidgetMagneticField->item(0, 0)->text().toDouble();
		magneticField.start *= magneticMoment*muBohr;
	}
	if (_ui->tableWidgetMagneticField->item(0, 1))
	{
		magneticField.end = _ui->tableWidgetMagneticField->item(0, 1)->text().toDouble();
		magneticField.end *= magneticMoment*muBohr;
	}
	if (_ui->tableWidgetMagneticField->item(0, 2))
	{
		magneticField.steps = _ui->tableWidgetMagneticField->item(0, 2)->text().toInt();
	}
	if (_ui->tableWidgetMagneticField->item(0, 3) && _ui->tableWidgetMagneticField->item(0, 4)
		&& _ui->tableWidgetMagneticField->item(0, 5))
	{
		Threedim direction = { 0,0,0 };
		direction.x = _ui->tableWidgetMagneticField->item(0, 3)->text().toDouble();
		direction.y = _ui->tableWidgetMagneticField->item(0, 4)->text().toDouble();
		direction.z = _ui->tableWidgetMagneticField->item(0, 5)->text().toDouble();
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
		connect(_anisoWindow, SIGNAL(destroyed()), this, SLOT(on_anisotropy_window_destroyed()));
		_anisoWindow->open();
	}
	else
	{
		_anisoWindow->raise();
	}
}

void GUIEnergyElements::on_push_button_neel(void)
{
	QString styleSheet = "background: rgb(255, 254, 242)";
	_ui->pushButtonNeel->setStyleSheet(styleSheet);
	_dmType = Neel;

	styleSheet = "background: rgb(242, 242, 242)";
	_ui->pushButtonChiral->setStyleSheet(styleSheet);
}

void GUIEnergyElements::on_push_button_chiral(void)
{
	QString styleSheet = "background: rgb(255, 254, 242)";
	_ui->pushButtonChiral->setStyleSheet(styleSheet);
	_dmType = Chiral;

	styleSheet = "background: rgb(242, 242, 242)";
	_ui->pushButtonNeel->setStyleSheet(styleSheet);
}

void GUIEnergyElements::on_anisotropy_window_destroyed(void)
{
	_anisoWindow = NULL;
}
