/*
* AnisotropyWindow.cpp
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

#include "AnisotropyWindow.h"
#include <QStringList>
#include <QStringListModel>
#include "MyHeaderView.h"

AnisotropyWindow::AnisotropyWindow(std::shared_ptr<std::vector<double>> hexAnisotropies,
	std::shared_ptr<std::vector<UniaxialAnisotropyStruct>> anisotropies,
	QWidget *parent)
	: QDialog(parent)
{
	/**
	* Setup of window to manage anisotropy energies. 
	*
	* @param[in] anisotropies Previously defined anisotropies.
	*/

	_ui.setupUi(this);

	_hexAnisotropies = hexAnisotropies;
	_anisotropies = anisotropies;

	QString qString("Additional uniaxial anisotropies <i>K</i>sin<font size=4><sup>2</sup></font size=4>" + 
		QString(QChar(920)));
	_ui.textEditUniaxial->setText(qString);

	qString = "Hexagonal anisotropy \
<i>K</i><font size=4><sub>1</sub></font size=4>sin<font size=4><sup>2</sup></font size=4>" 
+ QString(QChar(920)) +
" + <i>K</i><font size=4><sub>2</sub></font size=4>sin<font size = 4><sup>4</sup></font size=4>" 
+ QString(QChar(920)) + 
" + <i>K</i><font size=4><sub>3</sub></font size=4>sin<font size = 4><sup>6</sup></font size = 4>" 
+ QString(QChar(920)) + "cos6" + QString(QChar(966));
	_ui.textEditHexagonal->setText(qString);

	// Create model
	_model = new QStringListModel(this);

	// Make data
	_list = new QStringList();
	for (int i = 0; i < (*_anisotropies).size(); ++i)
	{
		_list->append("k = " + QString::number((*anisotropies)[i].energyParameter) 
			+ " , (" + QString::number((*anisotropies)[i].direction.x)
			+ " " + QString::number((*anisotropies)[i].direction.y)
			+ " " + QString::number((*anisotropies)[i].direction.z) + ")");
	}
	

	// Populate our model
	_model->setStringList(*_list);

	// Glue model and view together
	_ui.listViewUniaxial->setModel(_model);

	// Add additional feature so that
	// we can manually modify the data in ListView
	// It may be triggered by hitting any key or double-click etc.
	_ui.listViewUniaxial->setEditTriggers(QAbstractItemView::AnyKeyPressed |QAbstractItemView::DoubleClicked);

	std::vector<int> ratio;

	_ui.tableWidgetUniaxial->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_ui.tableWidgetUniaxial->setColumnCount(4);
	_ui.tableWidgetUniaxial->setRowCount(1);
	_ui.tableWidgetUniaxial->verticalHeader()->hide();
	_ui.tableWidgetUniaxial->setHorizontalHeaderLabels(QString("<i>K</i>;eX;eY;eZ").split(";"));
	ratio.push_back(117);
	ratio.push_back(114);
	ratio.push_back(114);
	ratio.push_back(114);
	_ui.tableWidgetUniaxial->set_column_ratio(ratio);
	_ui.tableWidgetUniaxial->set_horizontal_header_height(40);
	ratio.clear();
	ratio.push_back(40);
	_ui.tableWidgetUniaxial->set_row_ratio(ratio);

	_ui.tableWidgetUniaxial->setItem(0, 0, new QTableWidgetItem());
	_ui.tableWidgetUniaxial->setItem(0, 1, new QTableWidgetItem());
	_ui.tableWidgetUniaxial->setItem(0, 2, new QTableWidgetItem());
	_ui.tableWidgetUniaxial->setItem(0, 3, new QTableWidgetItem());

	_ui.tableWidgetUniaxial->item(0, 0)->setText("0.4");
	_ui.tableWidgetUniaxial->item(0, 1)->setText("0");
	_ui.tableWidgetUniaxial->item(0, 2)->setText("0");
	_ui.tableWidgetUniaxial->item(0, 3)->setText("1");

	_ui.tableWidgetHexagonal->setHorizontalHeader(new MyHeaderView(Qt::Horizontal));
	_ui.tableWidgetHexagonal->setColumnCount(6);
	_ui.tableWidgetHexagonal->setRowCount(1);
	_ui.tableWidgetHexagonal->verticalHeader()->hide();
	_ui.tableWidgetHexagonal->setHorizontalHeaderLabels(QString(
		"<i>K</i><font size=4><sub>1</sub>;<i>K</i><font size=4><sub>2</sub>;<i>K</i><font size=4><sub>3</sub>;\
eX;eY;eZ").split(";"));
	ratio.clear();
	ratio.push_back(117);
	ratio.push_back(117);
	ratio.push_back(117);
	ratio.push_back(114);
	ratio.push_back(114);
	ratio.push_back(114);
	_ui.tableWidgetHexagonal->set_column_ratio(ratio);
	_ui.tableWidgetHexagonal->set_horizontal_header_height(40);
	ratio.clear();
	ratio.push_back(40);
	_ui.tableWidgetHexagonal->set_row_ratio(ratio);

	_ui.tableWidgetHexagonal->setItem(0, 3, new QTableWidgetItem());
	_ui.tableWidgetHexagonal->setItem(0, 4, new QTableWidgetItem());
	_ui.tableWidgetHexagonal->setItem(0, 5, new QTableWidgetItem());

	for (int i = 0; i < 3; i++)
	{
		if (fabs((*_hexAnisotropies)[i]) > 0)
		{
			_ui.tableWidgetHexagonal->setItem(0, i, new QTableWidgetItem());
			_ui.tableWidgetHexagonal->item(0, i)->setText(QString::number((*_hexAnisotropies)[i]));
		}
	}

	_ui.tableWidgetHexagonal->item(0, 3)->setText("0");
	_ui.tableWidgetHexagonal->item(0, 4)->setText("0");
	_ui.tableWidgetHexagonal->item(0, 5)->setText("1");

	_ui.tableWidgetHexagonal->item(0, 3)->setFlags(0);
	_ui.tableWidgetHexagonal->item(0, 4)->setFlags(0);
	_ui.tableWidgetHexagonal->item(0, 5)->setFlags(0);

	connect(_ui.pushButtonOk, &QAbstractButton::released, this, &AnisotropyWindow::ok_clicked);
	connect(_ui.addButton, &QAbstractButton::released, this, &AnisotropyWindow::add_clicked);
	connect(_ui.deleteButton, &QAbstractButton::released, this, &AnisotropyWindow::delete_clicked);
}

AnisotropyWindow::~AnisotropyWindow()
{
}

void AnisotropyWindow::add_clicked()
{
	/**
	* Add button clicked. Add current specified anisotropy energy to end of existing anisotropy list. If a 
	* value was not specified, 0 will be used as default value.
	*/

	QString qString  = "";
	UniaxialAnisotropyStruct anisotropy = { 0, {0,0,0} };
	
	if (_ui.tableWidgetUniaxial->item(0, 0))
	{
		double energy = _ui.tableWidgetUniaxial->item(0, 0)->text().toDouble();
		qString.append("k = " + QString::number( energy) + " , (");
		anisotropy.energyParameter = energy;
	}
	if (_ui.tableWidgetUniaxial->item(0, 1))
	{
		double ex = _ui.tableWidgetUniaxial->item(0, 1)->text().toDouble();
		qString.append(QString::number(ex) + " ");
		anisotropy.direction.x = ex;
	}
	if (_ui.tableWidgetUniaxial->item(0, 2))
	{
		double ey = _ui.tableWidgetUniaxial->item(0, 2)->text().toDouble();
		qString.append(QString::number(ey) + " ");
		anisotropy.direction.y = ey;
	}
	if (_ui.tableWidgetUniaxial->item(0, 3))
	{
		double ez = _ui.tableWidgetUniaxial->item(0, 3)->text().toDouble();
		qString.append(QString::number(ez) + ")");
		anisotropy.direction.z = ez;
	}
	_list->append(qString);
	_anisotropies->push_back(anisotropy);
	_model->setStringList(*_list);
}

void AnisotropyWindow::delete_clicked()
{
	/**
	* Current selected anisotropy energy is removed.
	*/

	int index = _ui.listViewUniaxial->currentIndex().row();
	_list->removeAt(index);
	_model->setStringList(*_list);
	_anisotropies->erase(_anisotropies->begin() + index);
}

void AnisotropyWindow::ok_clicked()
{
	if (_ui.tableWidgetHexagonal->item(0, 0))
	{
		double k = _ui.tableWidgetHexagonal->item(0, 0)->text().toDouble();
		(*_hexAnisotropies)[0] = k;
	}
	if (_ui.tableWidgetHexagonal->item(0, 1))
	{
		double k = _ui.tableWidgetHexagonal->item(0, 1)->text().toDouble();
		(*_hexAnisotropies)[1] = k;
	}
	if (_ui.tableWidgetHexagonal->item(0, 2))
	{
		double k = _ui.tableWidgetHexagonal->item(0, 2)->text().toDouble();
		(*_hexAnisotropies)[2] = k;
	}
	this->close();
}