/*
* GUILatticeElements.cpp
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

#include "GUILatticeElements.h"

#include "LatticeMaskWindow.h"

// forward
#include "Configuration.h"
#include "MainWindow.h"

GUILatticeElements::GUILatticeElements(MainWindow* mw)
{
	/**
	* @param[in] ui Pointer to object with ui elements
	*/

	_mw = mw;
	setup_elements();
	set_default_values();

	_latticeMaskWindow = NULL;
}

GUILatticeElements::~GUILatticeElements()
{
	delete _latticeMaskWindow;
}

void GUILatticeElements::setup_elements(void)
{
	/**
	* The possible lattice types are specified here.
	*/

	_mw->_toolbar->comboBoxLatticeType->addItem(tr("simple cubic"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("body centered cubic"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("face centered cubic"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("2D triangular, hexagonal boundary"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("2D triangular, stripe"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("2D triangular, triangular boundary"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("2D triangular, half disk"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("2D triangular, disk"));
	_mw->_toolbar->comboBoxLatticeType->addItem(tr("2D triangular, arrow head"));
}

void GUILatticeElements::set_default_values(void)
{
	/**
	* set default lattice parameters
	*/

	update_to_lattice_type(_mw->_toolbar->comboBoxLatticeType->currentText());
}

void GUILatticeElements::read_parameters(const std::shared_ptr<Configuration> &config)
{
	/**
	Determines the lattice parameters from the GUI.

	@param[in] config Object to store parameters in.
	*/

	QString qString = _mw->_toolbar->comboBoxLatticeType->currentText();

	if (qString.compare("simple cubic") == 0)
	{
		config->_latticeType = simpleCubic;
	}
	if (qString.compare("body centered cubic") == 0)
	{
		config->_latticeType = bodyCenteredCubic;
	}
	if (qString.compare("face centered cubic") == 0)
	{
		config->_latticeType = faceCenteredCubic;
	}
	if (qString.compare("2D triangular, hexagonal boundary") == 0)
	{
		config->_latticeType = triangularHexagonal;
	}
	if (qString.compare("2D triangular, stripe") == 0)
	{
		config->_latticeType = triangularStripe;
	}
	if (qString.compare("2D triangular, triangular boundary") == 0)
	{
		config->_latticeType = triangularTriangular;
	}
	if (qString.compare("2D triangular, half disk") == 0)
	{
		config->_latticeType = triangularHalfDisk;
	}
	if (qString.compare("2D triangular, disk") == 0)
	{
		config->_latticeType = triangularDisk;
	}
	if (qString.compare("2D triangular, arrow head") == 0)
	{
		config->_latticeType = triangularArrowHead;
	}

	int nCol = _mw->_toolbar->tableWidgetLatticeDimensions->columnCount();
	config->_latticeDimensions.clear();
	for (int i = 0; i < nCol -1; ++i)
	{
		if (_mw->_toolbar->tableWidgetLatticeDimensions->item(0, i))
		{
			int dimension = _mw->_toolbar->tableWidgetLatticeDimensions->item(0, i)->text().toInt();
			config->_latticeDimensions.push_back(dimension);
		}
	}

	if (_mw->_toolbar->tableWidgetLatticeDimensions->item(0, nCol - 1))
	{
		config->_latticeConstant = _mw->_toolbar->tableWidgetLatticeDimensions->item(0, nCol - 1)->text().toDouble();
	}

	qString = _mw->_toolbar->comboBoxBoundaryConditions->currentText();
	if (qString.compare("open boundary conditions") == 0)
	{
		config->_boundaryConditions = openBound;
	}
	if (qString.compare("periodic boundary conditions") == 0)
	{
		config->_boundaryConditions = periodic;
	}
	if (qString.compare("x_periodic boundary conditions") == 0)
	{
		config->_boundaryConditions = periodicX;
	}
	if (qString.compare("y_periodic boundary conditions") == 0)
	{
		config->_boundaryConditions = periodicY;
	}
	if (qString.compare("helical boundary conditions") == 0)
	{
		config->_boundaryConditions = helical;
	}
}

void GUILatticeElements::update_to_lattice_type(QString qString)
{
	/**
	* Possible boundary conditions and lattice dimensions depend on the chosen lattice type. Adjusts these
	* when a different lattice type was chosen.
	*
	* @param[in] qString Specifier of lattice type.
	*/

	update_boundary_conditions_to_lattice_type(qString);
	update_lattice_dimensions_to_lattice_type(qString);
}

void GUILatticeElements::show_lattice_mask_window(void)
{
	if (_latticeMaskWindow == NULL)
	{
		_latticeMaskWindow = new LatticeMaskWindow(this);
	}
	_latticeMaskWindow->exec();
	delete _latticeMaskWindow;
	_latticeMaskWindow = NULL;
}

void GUILatticeElements::update_lattice_dimensions_to_lattice_type(QString qString)
{
	/**
	* Set possible lattice dimension parameters according to chosen lattice.
	*
	* @param[in] qString Specifier of lattice type.
	*/

	std::vector<int> ratio;

	if (qString.compare("simple cubic") == 0 || qString.compare("body centered cubic") == 0
		|| qString.compare("face centered cubic") == 0)
	{
		_mw->_toolbar->tableWidgetLatticeDimensions->clear();
		_mw->_toolbar->tableWidgetLatticeDimensions->setColumnCount(4);
		_mw->_toolbar->tableWidgetLatticeDimensions->setRowCount(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->verticalHeader()->hide();
		QString qString("DimX;DimY;DimZ;a [");
		qString.append(QChar(8491));
		qString.append("]");
		_mw->_toolbar->tableWidgetLatticeDimensions->setHorizontalHeaderLabels(qString.split(";"));
		ratio.push_back(1);
		ratio.push_back(1);
		ratio.push_back(1);
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_column_ratio(ratio);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_horizontal_header_height(1);
		ratio.clear();
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_row_ratio(ratio);
		QTableWidgetItem* dimX = new QTableWidgetItem("30");
		dimX->setText("30");
		dimX->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 0, dimX);
		QTableWidgetItem* dimY = new QTableWidgetItem();
		dimY->setText("18");
		dimY->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 1, dimY);
		QTableWidgetItem* dimZ = new QTableWidgetItem();
		dimZ->setText("1");
		dimZ->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 2, dimZ);
		QTableWidgetItem* latticeConstant = new QTableWidgetItem();
		latticeConstant->setText("1");
		latticeConstant->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 3, latticeConstant);
	}

	if (qString.compare("2D triangular, hexagonal boundary") == 0
		|| qString.compare("2D triangular, triangular boundary") == 0
		|| qString.compare("2D triangular, half disk") == 0
		|| qString.compare("2D triangular, disk") == 0)
	{
		_mw->_toolbar->tableWidgetLatticeDimensions->clear();
		_mw->_toolbar->tableWidgetLatticeDimensions->setColumnCount(2);
		_mw->_toolbar->tableWidgetLatticeDimensions->setRowCount(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->verticalHeader()->hide();
		QString qString("size n;a [");
		qString.append(QChar(8491));
		qString.append("]");
		_mw->_toolbar->tableWidgetLatticeDimensions->setHorizontalHeaderLabels(qString.split(";"));
		ratio.clear();
		ratio.push_back(1);
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_column_ratio(ratio);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_horizontal_header_height(1);
		ratio.clear();
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_row_ratio(ratio);
		QTableWidgetItem* dim = new QTableWidgetItem();
		dim->setText("17");
		dim->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 0, dim);
		QTableWidgetItem* latticeConstant = new QTableWidgetItem();
		latticeConstant->setText("1");
		latticeConstant->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 1, latticeConstant);
	}

	if (qString.compare("2D triangular, stripe") == 0)
	{
		_mw->_toolbar->tableWidgetLatticeDimensions->clear();
		_mw->_toolbar->tableWidgetLatticeDimensions->setColumnCount(3);
		_mw->_toolbar->tableWidgetLatticeDimensions->setRowCount(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->verticalHeader()->hide();
		QString qString("m rows;n sites per row;a [");
		qString.append(QChar(8491));
		qString.append("]");
		_mw->_toolbar->tableWidgetLatticeDimensions->setHorizontalHeaderLabels(qString.split(";"));
		ratio.clear();
		ratio.push_back(134);
		ratio.push_back(214);
		ratio.push_back(134);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_column_ratio(ratio);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_horizontal_header_height(1);
		ratio.clear();
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_row_ratio(ratio);
		QTableWidgetItem* mrows = new QTableWidgetItem();
		mrows->setText("18");
		mrows->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 0, mrows);
		QTableWidgetItem* sites = new QTableWidgetItem();
		sites->setText("30");
		sites->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 1, sites);
		QTableWidgetItem* latticeConstant = new QTableWidgetItem();
		latticeConstant->setText("1");
		latticeConstant->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 2, latticeConstant);
	}

	if (qString.compare("2D triangular, arrow head") == 0)
	{
		_mw->_toolbar->tableWidgetLatticeDimensions->clear();
		_mw->_toolbar->tableWidgetLatticeDimensions->setColumnCount(3);
		_mw->_toolbar->tableWidgetLatticeDimensions->setRowCount(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->verticalHeader()->hide();
		QString qString("m;n;a [");
		qString.append(QChar(8491));
		qString.append("]");
		_mw->_toolbar->tableWidgetLatticeDimensions->setHorizontalHeaderLabels(qString.split(";"));
		ratio.clear();
		ratio.push_back(1);
		ratio.push_back(1);
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_column_ratio(ratio);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_horizontal_header_height(1);
		ratio.clear();
		ratio.push_back(1);
		_mw->_toolbar->tableWidgetLatticeDimensions->set_row_ratio(ratio);
		QTableWidgetItem* mval = new QTableWidgetItem();
		mval->setText("20");
		mval->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 0, mval);
		QTableWidgetItem* nval = new QTableWidgetItem();
		nval->setText("6");
		nval->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 1, nval);
		QTableWidgetItem* latticeConstant = new QTableWidgetItem();
		latticeConstant->setText("1");
		latticeConstant->setTextAlignment(Qt::AlignCenter);
		_mw->_toolbar->tableWidgetLatticeDimensions->setItem(0, 2, latticeConstant);
	}

	_mw->_toolbar->tableWidgetLatticeDimensions->custom_resize();
}

void GUILatticeElements::update_boundary_conditions_to_lattice_type(QString qString)
{
	/**
	* Set possible boundary conditions according to chosen lattice.
	*
	* @param[in] qString Specifier of lattice type.
	*/

	if (qString.compare("simple cubic") == 0 )
	{
		int count = _mw->_toolbar->comboBoxBoundaryConditions->count();
		for (int i = 0; i < count; ++i)
		{
			_mw->_toolbar->comboBoxBoundaryConditions->removeItem(0);
		}
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("open boundary conditions"));
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("periodic boundary conditions"));
	}

	if (qString.compare("body centered cubic") == 0 || qString.compare("face centered cubic") == 0)
	{
		int count = _mw->_toolbar->comboBoxBoundaryConditions->count();
		for (int i = 0; i < count; ++i)
		{
			_mw->_toolbar->comboBoxBoundaryConditions->removeItem(0);
		}
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("open boundary conditions"));
	}

	if (qString.compare("2D triangular, hexagonal boundary") == 0)
	{
		int count = _mw->_toolbar->comboBoxBoundaryConditions->count();
		for (int i = 0; i < count; ++i)
		{
			_mw->_toolbar->comboBoxBoundaryConditions->removeItem(0);
		}
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("open boundary conditions"));
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("helical boundary conditions"));
	}

	if (qString.compare("2D triangular, triangular boundary") == 0
		|| qString.compare("2D triangular, half disk") == 0
		|| qString.compare("2D triangular, disk") == 0 || qString.compare("2D triangular, arrow head") == 0)
	{
		int count = _mw->_toolbar->comboBoxBoundaryConditions->count();
		for (int i = 0; i < count; ++i)
		{
			_mw->_toolbar->comboBoxBoundaryConditions->removeItem(0);
		}
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("open boundary conditions"));
	}

	if (qString.compare("2D triangular, stripe") == 0)
	{
		int count = _mw->_toolbar->comboBoxBoundaryConditions->count();
		for (int i = 0; i < count; ++i)
		{
			_mw->_toolbar->comboBoxBoundaryConditions->removeItem(0);
		}
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("open boundary conditions"));
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("periodic boundary conditions"));
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("x_periodic boundary conditions"));
		_mw->_toolbar->comboBoxBoundaryConditions->addItem(tr("y_periodic boundary conditions"));
	}
}