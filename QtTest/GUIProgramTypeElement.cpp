/*
* GUIProgramTypeElement.cpp
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

#include "GUIProgramTypeElement.h"

// forward includes
#include "AnisotropyWindow.h"
#include "Configuration.h"

GUIProgramTypeElement::GUIProgramTypeElement(Ui::QtMainWindow* ui)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/

	_ui = ui;

	setup_elements();
}

void GUIProgramTypeElement::setup_elements(void)
{
	/**
	* The possible program types are specified here.
	*/

	_ui->comboBoxProgramType->addItem(tr("temperature-magnetic-field-loop"));
	_ui->comboBoxProgramType->addItem(tr("spin-seebeck"));
	/*_ui->comboBoxProgramType->addItem(tr("tip-movement"));*/
	_ui->comboBoxProgramType->addItem(tr("experiment01"));
	_ui->comboBoxProgramType->addItem(tr("eigenFreq"));
	_ui->comboBoxProgramType->addItem(tr("read lattice configuration"));
	_ui->comboBoxProgramType->addItem(tr("read bitmap lattice mask"));
	_ui->comboBoxProgramType->addItem(tr("read spin configuration"));
	_ui->comboBoxProgramType->addItem(tr("save lattice configuration"));
	_ui->comboBoxProgramType->addItem(tr("save spin configuration"));
	_ui->comboBoxProgramType->addItem(tr("save site resolved energies"));
	_ui->comboBoxProgramType->addItem(tr("save site resolved topological charge"));
}

void GUIProgramTypeElement::read_parameter(QSharedPointer<Configuration> &config)
{
	/**
	* Read specified simulation type.
	*/

	QString qString = _ui->comboBoxProgramType->currentText();

	if (qString.compare("temperature-magnetic-field-loop") == 0)
	{
		config->_programType = temperatureMagneticFieldLoop;
	}
	if (qString.compare("spin-seebeck") == 0)
	{
		config->_programType = spinSeebeck;
	}
	if (qString.compare("tip-movement") == 0)
	{
		config->_programType = tipMovement;
	}
	if (qString.compare("experiment01") == 0)
	{
		config->_programType = Experiment01;
	}
	if (qString.compare("eigenFreq") == 0)
	{
		config->_programType = EigenFrequency;
	}
	if (qString.compare("read lattice configuration") == 0)
	{
		config->_programType = readLatticeConfiguration;
	}
	if (qString.compare("read bitmap lattice mask") == 0)
	{
		config->_programType = latticeMaskRead;
	}
	if (qString.compare("read spin configuration") == 0)
	{
		config->_programType = readSpinConfiguration;
	}
	if (qString.compare("save lattice configuration") == 0)
	{
		config->_programType = saveLatticeConfiguration;
	}
	if (qString.compare("save spin configuration") == 0)
	{
		config->_programType = saveSpinConfiguration;
	}
	if (qString.compare("save site resolved energies") == 0)
	{
		config->_programType = latticeSiteEnergies;
	}
	if (qString.compare("save site resolved topological charge") == 0)
	{
		config->_programType = latticeSiteWindingNumber;
	}
}