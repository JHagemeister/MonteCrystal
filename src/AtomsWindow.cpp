/*
* AtomsWindow.cpp
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

#include "AtomsWindow.h"
#include "OGLWidget.h"
#include "iostream"
#include "Lattice.h"
#include "MyMath.h"
#include <math.h>
#include "glm/gtx/string_cast.hpp"
#include <QSharedPointer>
#include "Hamiltonian.h"
#include "Energy.h"
#include "SphereMesh.h"
#include <QColorDialog>
#include <CMap.h>
#include <QMessageBox>

AtomsWindow::AtomsWindow(OGLWidget* oglWidget, QWidget * parent) : QDialog(parent)
{
    _ui.setupUi(this);
    _oglWidget = oglWidget;


    _ui.checkAtoms->setChecked(oglWidget->_showAtoms);
    connect(_ui.checkAtoms, &QCheckBox::stateChanged, this, &AtomsWindow::check_atoms);

    _ui.comboBoxMapValue->addItem(QString("single color(cMin)"));
    _ui.comboBoxMapValue->addItem(QString("rotation as rgb"));
    _ui.comboBoxMapValue->addItem(QString("MTip"));
    _ui.comboBoxMapValue->addItem(QString("E"));

    if (_oglWidget->_hamiltonian != NULL){
        int numE = _oglWidget->_hamiltonian->get_number_energies();
        std::vector<std::shared_ptr<Energy>> _energies =_oglWidget->_hamiltonian->get_energies();
        for (int i =0; i<numE ;i++ ) {
            _ui.comboBoxMapValue->addItem(QString::fromStdString( _energies.at(i)->get_string_id()));
        }
    }

    _ui.comboBoxMapValue->setCurrentText(QString::fromStdString(_oglWidget->_atomColorValue));


    _ui.comboBoxCMaps->addItem(QString("cMin->cMax"));
    _ui.comboBoxCMaps->addItems(QDir(QString("cmaps/")).entryList(QDir::Filter::Files)) ;
    _ui.comboBoxCMaps->addItem(QString("custom?"));
    if(_oglWidget->cmap){
       _ui.comboBoxCMaps->setCurrentText(_oglWidget->cmap->name);
    }




    _ui.tableWidgetGeometry->setColumnCount(2);
    _ui.tableWidgetGeometry->setRowCount(1);
    _ui.tableWidgetGeometry->verticalHeader()->hide();
    _ui.tableWidgetGeometry->horizontalHeader()->setVisible(1);
    _ui.tableWidgetGeometry->setHorizontalHeaderLabels(QString("radius; slices/rows").split(";"));
    _ui.tableWidgetGeometry->custom_resize();




    QTableWidgetItem* radius = new QTableWidgetItem();
    radius->setText(QString::number(_oglWidget->_sphereMesh->radius));
    _ui.tableWidgetGeometry->setItem(0, 0, radius);


    QTableWidgetItem* segments = new QTableWidgetItem();
    segments->setText(QString::number(_oglWidget->_sphereMesh->segments));
    _ui.tableWidgetGeometry->setItem(0, 1, segments);


    QColor qc;
    qc.setRgbF(_oglWidget->_atomeMinColor.x,_oglWidget->_atomeMinColor.y,_oglWidget->_atomeMinColor.z);
    _ui.pushButtonCMin->setStyleSheet("background-color: "+qc.name());      this->repaint();
    qc.setRgbF(_oglWidget->_atomeMaxColor.x,_oglWidget->_atomeMaxColor.y,_oglWidget->_atomeMaxColor.z);
    _ui.pushButtonCMax->setStyleSheet("background-color: "+qc.name());      this->repaint();




    _ui.tableWidgetCutoff->setColumnCount(2);
    _ui.tableWidgetCutoff->setRowCount(1);
    _ui.tableWidgetCutoff->verticalHeader()->hide();
    _ui.tableWidgetCutoff->horizontalHeader()->setVisible(1);
    _ui.tableWidgetCutoff->setHorizontalHeaderLabels(QString("min;max").split(";"));
    _ui.tableWidgetCutoff->custom_resize();

    QString cutInfo="set cutoffs for colormap.";
    if (_oglWidget->cmap && (_oglWidget->_atomColorValue!="rotation as rgb" ||_oglWidget->_atomColorValue!="single color(cMin)" )){
        cutInfo.append(QString("\ndata range: %1 -> %2").arg(_oglWidget->cmap->realMinVal).arg(_oglWidget->cmap->realMaxVal));
    }
    _ui.labelCutoffs->setText(cutInfo);
    QTableWidgetItem* cutOffMin= new QTableWidgetItem();
    if(_oglWidget->cmap && _oglWidget->cmap->_minSet){
            cutOffMin->setText(QString::number(_oglWidget->cmap->_cutMin));
    }else{
            cutOffMin->setText("");
    }
    _ui.tableWidgetCutoff->setItem(0, 0, cutOffMin);

    QTableWidgetItem* cutOffMax= new QTableWidgetItem();
    if(_oglWidget->cmap && _oglWidget->cmap->_maxSet){
            cutOffMin->setText(QString::number(_oglWidget->cmap->_cutMax));
    }else{
            cutOffMin->setText("");
    }
    _ui.tableWidgetCutoff->setItem(0, 1, cutOffMax);



    _ui.tableWidgetMtip->setColumnCount(3);
    _ui.tableWidgetMtip->setRowCount(1);
    _ui.tableWidgetMtip->verticalHeader()->hide();
    _ui.tableWidgetMtip->horizontalHeader()->setVisible(1);
    _ui.tableWidgetMtip->setHorizontalHeaderLabels(QString("Mx;My;Mz").split(";"));
    _ui.tableWidgetMtip->custom_resize();

    QTableWidgetItem* Mx = new QTableWidgetItem();
    Mx->setText(QString::number(_oglWidget->_MTip.x));
    _ui.tableWidgetMtip->setItem(0, 0, Mx);
    QTableWidgetItem* My = new QTableWidgetItem();
    My->setText(QString::number(_oglWidget->_MTip.y));
    _ui.tableWidgetMtip->setItem(0, 1, My);
    QTableWidgetItem* Mz = new QTableWidgetItem();
    Mz->setText(QString::number(_oglWidget->_MTip.z));
    _ui.tableWidgetMtip->setItem(0, 2, Mz);

    connect(_ui.checkAtoms,&QAbstractButton::released,this, &AtomsWindow::read_parameters);
    connect(_ui.comboBoxCMaps,&QComboBox::currentTextChanged,this, &AtomsWindow::read_parameters);
    connect(_ui.comboBoxMapValue,&QComboBox::currentTextChanged,this, &AtomsWindow::read_parameters);
    connect(_ui.pushButtonCMin, &QAbstractButton::released, this, & AtomsWindow::chooseMinColor);
    connect(_ui.pushButtonCMax, &QAbstractButton::released, this, & AtomsWindow::chooseMaxColor);
    connect( _ui.tableWidgetCutoff,&QTableWidget::cellChanged,this,&AtomsWindow::read_parameters);
    connect(_ui.tableWidgetGeometry,&QTableWidget::cellChanged,this,&AtomsWindow::read_parameters);
    connect(_ui.tableWidgetMtip,&QTableWidget::cellChanged,this,&AtomsWindow::read_parameters);





}


AtomsWindow::~AtomsWindow()
{

}



void AtomsWindow::read_parameters(void)
{
    if (_ui.tableWidgetGeometry->item(0, 0))
    {
        _oglWidget->_sphereMesh->radius = _ui.tableWidgetGeometry->item(0, 0)->text().toFloat();
        _oglWidget->_sphereMesh->generate_mesh_data();
        _oglWidget->_sphereMesh->update_buffers();
    }
    if (_ui.tableWidgetGeometry->item(0, 1))
    {
        _oglWidget->_sphereMesh->segments = _ui.tableWidgetGeometry->item(0, 1)->text().toInt();
        _oglWidget->_sphereMesh->generate_mesh_data();
        _oglWidget->_sphereMesh->update_buffers();
    }

    _oglWidget->_atomColorValue=_ui.comboBoxMapValue->currentText().toStdString();
    std::string cmType=_ui.comboBoxCMaps->currentText().toStdString();
    if (cmType=="cMin->cMax"){

        delete _oglWidget->cmap;

        _oglWidget->cmap = new CMap(_oglWidget->_atomeMinColor,_oglWidget->_atomeMaxColor);
    }else if(cmType=="custom?"){
        QMessageBox msgBox;
        msgBox.setText("If you need a custom color-map, \njust copy, rename and modify one of the files in the 'cmaps/'- directory.");
        msgBox.exec();
        delete _oglWidget->cmap;

        _oglWidget->cmap = new CMap(_oglWidget->_atomeMinColor,_oglWidget->_atomeMaxColor);
    }else{
       delete _oglWidget->cmap;

    _oglWidget->cmap = new CMap(_ui.comboBoxCMaps->currentText());
    }

    if (_ui.tableWidgetCutoff->item(0, 0))
    {
        double cutoff;
        cutoff=_ui.tableWidgetCutoff->item(0, 0)->text().toDouble(&_oglWidget->cmap->_minSet);

        if (_oglWidget->cmap->_minSet){

            _oglWidget->cmap->_cutMin=cutoff;

        }
    }
    if (_ui.tableWidgetCutoff->item(0, 1))
    {
        double cutoff;
        cutoff=_ui.tableWidgetCutoff->item(0, 1)->text().toDouble(&_oglWidget->cmap->_maxSet);

        if (_oglWidget->cmap->_maxSet){

            _oglWidget->cmap->_cutMax=cutoff;

        }
    }


    if(_ui.tableWidgetMtip->item(0,0)){
        _oglWidget->_MTip.x=_ui.tableWidgetMtip->item(0,0)->text().toDouble();
    }
    if(_ui.tableWidgetMtip->item(0,1)){
        _oglWidget->_MTip.y=_ui.tableWidgetMtip->item(0,1)->text().toDouble();
    }
    if(_ui.tableWidgetMtip->item(0,2)){
        _oglWidget->_MTip.z=_ui.tableWidgetMtip->item(0,2)->text().toDouble();
    }



    _oglWidget->repaint();
    QString cutInfo="set cutoffs for colormap.";
    if (_oglWidget->cmap && _oglWidget->_atomColorValue!="rotation as rgb" &&_oglWidget->_atomColorValue!="single color(cMin)" ){
        cutInfo.append(QString("\ndata range: %1 -> %2").arg(_oglWidget->cmap->realMinVal).arg(_oglWidget->cmap->realMaxVal));
    }
    _ui.labelCutoffs->setText(cutInfo);






}





void AtomsWindow::check_atoms(void){

    _oglWidget->_showAtoms= _ui.checkAtoms->isChecked();

    read_parameters();

}





void AtomsWindow::chooseMinColor(void){
    double r,g,b;
    _colorPicker.getColor().getRgbF(&r,&g,&b);
    _oglWidget->_atomeMinColor=Threedim{r,g,b};
    QColor qc;
    qc.setRgbF(r,g,b);
    _ui.pushButtonCMin->setStyleSheet("background-color: "+qc.name());
    if (_ui.comboBoxCMaps->currentText().toStdString()=="cMin->cMax"){
        if (_oglWidget->cmap){
            delete _oglWidget->cmap;
        }
        _oglWidget->cmap = new CMap(_oglWidget->_atomeMinColor,_oglWidget->_atomeMaxColor);
    }

   read_parameters();
}
void AtomsWindow::chooseMaxColor(void){
    double r,g,b;
    _colorPicker.getColor().getRgbF(&r,&g,&b);
    _oglWidget->_atomeMaxColor=Threedim{r,g,b};
    QColor qc;
    qc.setRgbF(r,g,b);
    _ui.pushButtonCMax->setStyleSheet("background-color: "+qc.name());
    if (_ui.comboBoxCMaps->currentText().toStdString()=="cMin->cMax"){
        if (_oglWidget->cmap){
            delete _oglWidget->cmap;
        }
        _oglWidget->cmap = new CMap(_oglWidget->_atomeMinColor,_oglWidget->_atomeMaxColor);
    }
    read_parameters();
}

QStringList getStoredCmaps(void){
    return QDir(QString("cmaps/")).entryList();

}
