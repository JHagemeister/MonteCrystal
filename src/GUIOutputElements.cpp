/*
* GUIOutputElements.cpp
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

#include "GUIOutputElements.h"

#include <QCheckBox>
#include <QTableWidget>

// forward includes
#include "Configuration.h"
#include "MainWindow.h"

GUIOutputElements::GUIOutputElements(MainWindow* mw)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/
	_mw = mw;
	setup_elements();
	set_default_values();
}

void GUIOutputElements::setup_elements(void)
{
	/**
	The output parameters are specified here. This includes the update of the OGLwidget and the output into 
	text files.
	*/

	_mw->_toolbar->tableWidgetUIUpdate->setColumnCount(1);
	_mw->_toolbar->tableWidgetUIUpdate->setRowCount(1);
	_mw->_toolbar->tableWidgetUIUpdate->verticalHeader()->hide();
	_mw->_toolbar->tableWidgetUIUpdate->setHorizontalHeaderLabels(QString("uiWidth;").split(";"));

	_mw->_toolbar->tableWidgetMovie->setColumnCount(3);
	_mw->_toolbar->tableWidgetMovie->setRowCount(1);
	_mw->_toolbar->tableWidgetMovie->verticalHeader()->hide();
	_mw->_toolbar->tableWidgetMovie->setHorizontalHeaderLabels(QString("start;stop;width").split(";"));

	_checkBox_E = new QCheckBox(tr("E"));
	_checkBox_M = new QCheckBox(tr("M"));
	_checkBox_Mabs = new QCheckBox(tr("|M|"));
	_checkBox_NCMR = new QCheckBox(tr("NCMR"));
	_checkBox_espin = new QCheckBox(tr("espin"));
	_checkBox_spinConfig = new QCheckBox(tr("spin"));
	_checkBox_Skn = new QCheckBox(tr("SkN"));
	_checkBox_outSimStep = new QCheckBox(tr("Step"));

	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_E, 0, 0);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_M, 0, 1);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_Mabs, 0, 2);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_NCMR, 0, 3);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_espin, 1, 0);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_Skn, 1, 1);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_spinConfig, 1, 2);
	_mw->_toolbar->gridLayoutOutput->addWidget(_checkBox_outSimStep, 1, 3);

	QTableWidgetItem* uiWidth = new QTableWidgetItem();
	_mw->_toolbar->tableWidgetUIUpdate->setItem(0, 0, uiWidth);
}

void GUIOutputElements::set_default_values(void)
{
	/**
	* Set default values for output parameters
	*/

	_mw->_toolbar->tableWidgetUIUpdate->item(0, 0)->setText("500");
}

void GUIOutputElements::read_parameters(QSharedPointer<Configuration> &config)
{
	/**
	Reads output parameters from GUI.

	@param[in] config Object to store parameters in.
	*/

	if (_mw->_toolbar->tableWidgetUIUpdate->item(0, 0))
	{
		config->_uiUpdateWidth = _mw->_toolbar->tableWidgetUIUpdate->item(0, 0)->text().toInt();
	}
	config->_doEnergyOutput = _checkBox_E->isChecked();
	config->_doMagnetisationOutput = _checkBox_M->isChecked();
	config->_doAbsoluteMagnetisationOutput = _checkBox_Mabs->isChecked();
	config->_doNCMROutput = _checkBox_NCMR->isChecked();
	config->_doWindingNumberOutput = _checkBox_Skn->isChecked();
	config->_doSpinConfigOutput = _checkBox_spinConfig->isChecked();
	config->_doSpinResolvedOutput = _checkBox_espin->isChecked();
	config->_doSimulationStepsOutput = _checkBox_outSimStep->isChecked();
	if (_mw->_toolbar->tableWidgetMovie->item(0, 0))
	{
		config->_movieStart = _mw->_toolbar->tableWidgetMovie->item(0, 0)->text().toDouble();
	}
	if (_mw->_toolbar->tableWidgetMovie->item(0, 1))
	{
		config->_movieEnd = _mw->_toolbar->tableWidgetMovie->item(0, 1)->text().toDouble();
	}
	if (_mw->_toolbar->tableWidgetMovie->item(0, 2))
	{
		config->_movieWidth = _mw->_toolbar->tableWidgetMovie->item(0, 2)->text().toDouble();
	}
}
