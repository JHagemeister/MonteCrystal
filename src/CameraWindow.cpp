/*
* CameraWindow.cpp
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

#include "CameraWindow.h"
#include "OGLWidget.h"
#include "iostream"
#include "Lattice.h"
#include "MyMath.h"
#include <math.h>
#include "glm/gtx/string_cast.hpp"
CameraWindow::CameraWindow(OGLWidget* oglWidget, QWidget * parent) : QDialog(parent)
{
    _ui.setupUi(this);
    _oglWidget = oglWidget;




    _ui.comboBoxViewType->addItem(QString("orthogonal projection"));
    _ui.comboBoxViewType->addItem(QString("free camera"));
    if (_oglWidget->_orthoView){
        _ui.comboBoxViewType->setCurrentText(QString("orthogonal projection"));
    }
    else{
        _ui.comboBoxViewType->setCurrentText(QString("free camera"));
    }


    _ui.tableWidgetPos->setColumnCount(3);
    _ui.tableWidgetPos->setRowCount(1);
    _ui.tableWidgetPos->verticalHeader()->hide();
    _ui.tableWidgetPos->horizontalHeader()->setVisible(1);
    _ui.tableWidgetPos->setHorizontalHeaderLabels(QString("\u03b8;\u03c8;\u03c1").split(";"));
    _ui.tableWidgetPos->custom_resize();

    QTableWidgetItem* theta = new QTableWidgetItem();
    theta->setText(QString::number(_oglWidget->_cameraAngle.x));
    _ui.tableWidgetPos->setItem(0, 0, theta);
    QTableWidgetItem* phi = new QTableWidgetItem();
    phi->setText(QString::number(_oglWidget->_cameraAngle.y));
    _ui.tableWidgetPos->setItem(0, 1, phi);
    QTableWidgetItem* rho = new QTableWidgetItem();
    rho->setText(QString::number(_oglWidget->_cameraAngle.z));
    _ui.tableWidgetPos->setItem(0, 2, rho);


    _ui.tableWidgetCamOffset->setColumnCount(3);
    _ui.tableWidgetCamOffset->setRowCount(1);
    _ui.tableWidgetCamOffset->verticalHeader()->hide();
    _ui.tableWidgetCamOffset->horizontalHeader()->setVisible(1);
    _ui.tableWidgetCamOffset->setHorizontalHeaderLabels(QString("x;y;z").split(";"));
    _ui.tableWidgetCamOffset->custom_resize();

    QTableWidgetItem* x_off = new QTableWidgetItem();
    x_off->setText(QString::number(_oglWidget->_viewOffset.x));
    _ui.tableWidgetCamOffset->setItem(0, 0, x_off);
    QTableWidgetItem* y_off = new QTableWidgetItem();
    y_off->setText(QString::number(_oglWidget->_viewOffset.y));
    _ui.tableWidgetCamOffset->setItem(0, 1, y_off);
    QTableWidgetItem* z_off = new QTableWidgetItem();
    z_off->setText(QString::number(_oglWidget->_viewOffset.z));
    _ui.tableWidgetCamOffset->setItem(0, 2, z_off);





    _ui.checkBase->setChecked(oglWidget->_showBase);

    _ui.tableWidgetBaseOffset->setColumnCount(3);
    _ui.tableWidgetBaseOffset->setRowCount(1);
    _ui.tableWidgetBaseOffset->verticalHeader()->hide();
    _ui.tableWidgetBaseOffset->horizontalHeader()->setVisible(1);
    _ui.tableWidgetBaseOffset->setHorizontalHeaderLabels(QString("boarder x;boarder y;offset_z").split(";"));

    QTableWidgetItem* boarderX = new QTableWidgetItem();
    boarderX->setText(QString::number(_oglWidget->_baseOffset.x));
    _ui.tableWidgetBaseOffset->setItem(0, 0, boarderX);
    QTableWidgetItem* boarderY = new QTableWidgetItem();
     boarderY->setText(QString::number(_oglWidget->_baseOffset.y));
    _ui.tableWidgetBaseOffset->setItem(0, 1, boarderY);
    QTableWidgetItem* off_z = new QTableWidgetItem();
    off_z->setText(QString::number(_oglWidget->_baseOffset.z));
    _ui.tableWidgetBaseOffset->setItem(0, 2, off_z);


    _ui.tableWidgetBaseColor->setColumnCount(3);
    _ui.tableWidgetBaseColor->setRowCount(1);
    _ui.tableWidgetBaseColor->verticalHeader()->hide();
    _ui.tableWidgetBaseColor->horizontalHeader()->setVisible(1);
    _ui.tableWidgetBaseColor->setHorizontalHeaderLabels(QString("base r;base g;base b").split(";"));

    QTableWidgetItem* baseR = new QTableWidgetItem();
    baseR->setText(QString::number(_oglWidget->_baseColor.r));
    _ui.tableWidgetBaseColor->setItem(0, 0, baseR);
    QTableWidgetItem* baseG = new QTableWidgetItem();
     baseG->setText(QString::number(_oglWidget->_baseColor.y));
    _ui.tableWidgetBaseColor->setItem(0, 1, baseG);
    QTableWidgetItem* baseB = new QTableWidgetItem();
    baseB->setText(QString::number(_oglWidget->_baseColor.z));
    _ui.tableWidgetBaseColor->setItem(0, 2, baseB);





    connect(_ui.checkBase, &QCheckBox::stateChanged, this, &CameraWindow::check_base);


    connect(_ui.pushButtonApply, &QAbstractButton::released, this, &CameraWindow::push_button_apply);
    connect(_ui.pushButtonOk, &QAbstractButton::released, this, &QDialog::close);

}


CameraWindow::~CameraWindow()
{
	
}



void CameraWindow::read_parameters(void)
{
    if (_ui.comboBoxViewType->currentText()==QString("orthogonal projection")){
        _oglWidget->_orthoView=TRUE;
    }
    else{
        _oglWidget->_orthoView=FALSE;
    }



    if (_ui.tableWidgetPos->item(0, 0))
	{
        _oglWidget->_cameraAngle.x = _ui.tableWidgetPos->item(0, 0)->text().toDouble();
	}
    if (_ui.tableWidgetPos->item(0, 1))
	{
        _oglWidget->_cameraAngle.y = _ui.tableWidgetPos->item(0, 1)->text().toDouble();
	}
    if (_ui.tableWidgetPos->item(0, 2))
	{
        _oglWidget->_cameraAngle.z = _ui.tableWidgetPos->item(0, 2)->text().toDouble();
	}


    if (_ui.tableWidgetCamOffset->item(0, 0))
    {
        _oglWidget->_viewOffset.x = _ui.tableWidgetCamOffset->item(0, 0)->text().toDouble();
    }
    if (_ui.tableWidgetCamOffset->item(0, 1))
    {
        _oglWidget->_viewOffset.y = _ui.tableWidgetCamOffset->item(0, 1)->text().toDouble();
    }
    if (_ui.tableWidgetCamOffset->item(0, 2))
    {
        _oglWidget->_viewOffset.z = _ui.tableWidgetCamOffset->item(0, 2)->text().toDouble();
    }



    if (_ui.tableWidgetBaseOffset->item(0, 0))
    {
        _oglWidget->_baseOffset.x = _ui.tableWidgetBaseOffset->item(0, 0)->text().toDouble();
    }
    if (_ui.tableWidgetBaseOffset->item(0, 1))
    {
        _oglWidget->_baseOffset.y = _ui.tableWidgetBaseOffset->item(0, 1)->text().toDouble();
    }
    if (_ui.tableWidgetPos->item(0, 2))
    {
        _oglWidget->_baseOffset.z = _ui.tableWidgetBaseOffset->item(0, 2)->text().toDouble();
    }



    if (_ui.tableWidgetBaseColor->item(0, 0))
    {
        _oglWidget->_baseColor.r = _ui.tableWidgetBaseColor->item(0, 0)->text().toDouble();
    }
    if (_ui.tableWidgetBaseColor->item(0, 1))
    {
        _oglWidget->_baseColor.g = _ui.tableWidgetBaseColor->item(0, 1)->text().toDouble();
    }
    if (_ui.tableWidgetBaseColor->item(0, 2))
    {
        _oglWidget->_baseColor.b = _ui.tableWidgetBaseColor->item(0, 2)->text().toDouble();
    }

}
void CameraWindow::push_button_apply(void)
{
    read_parameters();



    _oglWidget->set_camera();
    _oglWidget->repaint();

}


void CameraWindow::check_base(void){
    _oglWidget->_showBase=_ui.checkBase->isChecked();
    _oglWidget->repaint();
}


void CameraWindow::close(void)
{
    read_parameters();
    this->close();
}

