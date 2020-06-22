/*
* StmWindow.cpp
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

#include "StmWindow.h"
#include "OGLWidget.h"
#include "iostream"
#include "Lattice.h"
#include "MyMath.h"
#include <math.h>
#include "glm/gtx/string_cast.hpp"
#include <QSharedPointer>
#include <QFileDialog>
#include "Hamiltonian.h"
#include "Energy.h"
#include "SphereMesh.h"
#include <QColorDialog>
#include <CMap.h>
#include <QMessageBox>
#include <QImage>
#include "Stm.h"
StmWindow::StmWindow(OGLWidget* oglWidget,QSharedPointer<Lattice> lattice, QWidget * parent) : QDialog(parent)
{

    _ui.setupUi(this);

    _oglWidget = oglWidget;



    if (_oglWidget->_stm && _oglWidget->_latticeCoordArray==_oglWidget->_stm->_latticeCoord ){
        _stm = _oglWidget->_stm;

    }else{
        _oglWidget->_stm =new STM(_oglWidget->_spinArray,&_oglWidget->_numberAtoms,lattice);
        _stm=_oglWidget->_stm;
    }

    resize(_stm->_WindowWidth,_stm->_WindowHeight);
    _ui.comboBoxCMaps->addItem(QString("cMin->cMax"));
    _ui.comboBoxCMaps->addItems(QDir(QString("cmaps/")).entryList(QDir::Filter::Files)) ;
    _ui.comboBoxCMaps->addItem(QString("custom?"));
    if(_stm){
    _ui.comboBoxCMaps->setCurrentText(_oglWidget->_stm->cmapSTM->name);
    }else{
    _ui.comboBoxCMaps->setCurrentText("brown");
    }

    connect(_ui.comboBoxCMaps,&QComboBox::currentTextChanged,this, &StmWindow::read_parameters);


    QColor qc;
    qc.setRgbF(_stm->minColor.x,_stm->minColor.y,_stm->minColor.z);
    _ui.pushButtonCMin->setStyleSheet("background-color: "+qc.name());      this->repaint();
    qc.setRgbF(_stm->maxColor.x,_stm->maxColor.y,_stm->maxColor.z);
    _ui.pushButtonCMax->setStyleSheet("background-color: "+qc.name());      this->repaint();
    connect(_ui.pushButtonCMin, &QAbstractButton::released, this, & StmWindow::chooseMinColor);
    connect(_ui.pushButtonCMax, &QAbstractButton::released, this, & StmWindow::chooseMaxColor);



    QString cutInfo="set cutoffs for colormap";
    cutInfo.append(QString("\nrange: %1 -> %2").arg(QString::number(_stm->cmapSTM->realMinVal,'e',2)).arg(QString::number(_stm->cmapSTM->realMaxVal,'e',2)));
    _ui.labelCutoffs->setText(cutInfo);




    _ui.tableWidgetCutoff->setColumnCount(2);
    _ui.tableWidgetCutoff->setRowCount(1);
    _ui.tableWidgetCutoff->verticalHeader()->hide();
    _ui.tableWidgetCutoff->horizontalHeader()->setVisible(1);
    _ui.tableWidgetCutoff->setHorizontalHeaderLabels(QString("min;max").split(";"));
    _ui.tableWidgetCutoff->custom_resize();
    QTableWidgetItem* cutOffMin= new QTableWidgetItem();
    if(_stm->cmapSTM->_minSet){
            cutOffMin->setText(QString::number(_stm->cmapSTM->_cutMin,'g',4));
    }else{
            cutOffMin->setText("");
    }
    _ui.tableWidgetCutoff->setItem(0, 0, cutOffMin);

    QTableWidgetItem* cutOffMax= new QTableWidgetItem();
    if(_stm->cmapSTM->_maxSet){
            cutOffMin->setText(QString::number(_stm->cmapSTM->_cutMax,'g',4));
    }else{
            cutOffMin->setText("");
    }
    _ui.tableWidgetCutoff->setItem(0, 1, cutOffMax);


    _ui.tableWidgetstmParams->setColumnCount(4);
    _ui.tableWidgetstmParams->setRowCount(1);
    _ui.tableWidgetstmParams->verticalHeader()->hide();
    _ui.tableWidgetstmParams->horizontalHeader()->setVisible(1);
    _ui.tableWidgetstmParams->setHorizontalHeaderLabels(QString::fromUtf8("radius;\u03BA;dpa;h").split(";"));
    _ui.tableWidgetstmParams->custom_resize();


    QTableWidgetItem* radius = new QTableWidgetItem();
    radius->setText(QString::number(_stm->_radius));
    _ui.tableWidgetstmParams->setItem(0, 0, radius);
    QTableWidgetItem* kappa= new QTableWidgetItem();
    kappa->setText(QString::number(_stm->_kappa));
    _ui.tableWidgetstmParams->setItem(0, 1, kappa);
    QTableWidgetItem* dpa= new QTableWidgetItem();
    dpa->setText(QString::number(_stm->_dpa));
    _ui.tableWidgetstmParams->setItem(0, 2, dpa);
    QTableWidgetItem* h= new QTableWidgetItem();
    h->setText(QString::number(_stm->_h));
    _ui.tableWidgetstmParams->setItem(0, 3, h);




    _ui.tableWidgetMTip->setColumnCount(3);
    _ui.tableWidgetMTip->setRowCount(1);
    _ui.tableWidgetMTip->verticalHeader()->hide();
    _ui.tableWidgetMTip->horizontalHeader()->setVisible(1);
    _ui.tableWidgetMTip->setHorizontalHeaderLabels(QString("Mx;My;Mz").split(";"));
    _ui.tableWidgetMTip->custom_resize();


    QTableWidgetItem* Mx = new QTableWidgetItem();
    Mx->setText(QString::number(_stm->_MTip.x));
    _ui.tableWidgetMTip->setItem(0, 0, Mx);
    QTableWidgetItem* My= new QTableWidgetItem();
    My->setText(QString::number(_stm->_MTip.y));
    _ui.tableWidgetMTip->setItem(0, 1, My);
    QTableWidgetItem* Mz= new QTableWidgetItem();
    Mz->setText(QString::number(_stm->_MTip.z));
    _ui.tableWidgetMTip->setItem(0, 2, Mz);



    _ui.tableWidgetContrast->setColumnCount(4);
    _ui.tableWidgetContrast->setRowCount(1);
    _ui.tableWidgetContrast->verticalHeader()->hide();
    _ui.tableWidgetContrast->horizontalHeader()->setVisible(1);
    _ui.tableWidgetContrast->setHorizontalHeaderLabels(QString::fromUtf8("I\u2080 ;TMR;TAMR;NCMR").split(";"));
    _ui.tableWidgetContrast->custom_resize();

    QTableWidgetItem* I0 = new QTableWidgetItem();
    I0->setText(QString::number(_stm->I0));
    _ui.tableWidgetContrast->setItem(0, 0, I0);
    QTableWidgetItem* TMR = new QTableWidgetItem();
    TMR->setText(QString::number(_stm->TMR));
    _ui.tableWidgetContrast->setItem(0, 1, TMR);
    QTableWidgetItem* TAMR= new QTableWidgetItem();
    TAMR->setText(QString::number(_stm->TAMR));
    _ui.tableWidgetContrast->setItem(0, 2, TAMR);
    QTableWidgetItem* NCMR= new QTableWidgetItem();
    NCMR->setText(QString::number(_stm->NCMR));
    _ui.tableWidgetContrast->setItem(0, 3, NCMR);



    connect(_ui.tableWidgetCutoff,&QTableWidget::cellChanged,this,&StmWindow::read_parameters);
    connect(_ui.tableWidgetContrast,&QTableWidget::cellChanged,this,&StmWindow::read_parameters);
    connect(_ui.tableWidgetstmParams,&QTableWidget::cellChanged,this,&StmWindow::read_parameters);
    connect(_ui.tableWidgetMTip,&QTableWidget::cellChanged,this,&StmWindow::read_parameters);



    connect(_ui.pushButtonSaveIMG, &QAbstractButton::released, this, &StmWindow::saveCurrendImg);




    _stm->calcSTM();
    getIMG();

}
StmWindow::~StmWindow(){

}





void StmWindow::read_parameters(void){
    std::string cmType=_ui.comboBoxCMaps->currentText().toStdString();
    if (cmType=="cMin->cMax"){
        delete _stm->cmapSTM;
        _stm->cmapSTM = new CMap(_stm->minColor,_stm->maxColor);
    }else if(cmType=="custom?"){
        QMessageBox msgBox;
        msgBox.setText("If you need a custom color-map, \njust copy, rename and modify one of the files in the 'cmaps/'- directory.");
        msgBox.exec();
    }else{
        delete _stm->cmapSTM;
        _stm->cmapSTM = new CMap(_ui.comboBoxCMaps->currentText());
    }




    if (_ui.tableWidgetstmParams->item(0, 0))
    {
        _stm->_radius=_ui.tableWidgetstmParams->item(0,0)->text().toFloat();
    }

    if (_ui.tableWidgetstmParams->item(0, 1))
    {
        _stm->_kappa=_ui.tableWidgetstmParams->item(0,1)->text().toFloat();
    }

    if (_ui.tableWidgetstmParams->item(0, 2))
    {
        _stm->_dpa=_ui.tableWidgetstmParams->item(0,2)->text().toInt();
    }
    if (_ui.tableWidgetstmParams->item(0, 3))
    {
        _stm->_h=_ui.tableWidgetstmParams->item(0,3)->text().toFloat();
    }


    if(_ui.tableWidgetCutoff->item(0,0)){
        double cutoff;
        cutoff=_ui.tableWidgetCutoff->item(0, 0)->text().toDouble(&_stm->cmapSTM->_minSet);

        if (_stm->cmapSTM->_minSet){

            _stm->cmapSTM->_cutMin=cutoff;

        }
    }

    if(_ui.tableWidgetCutoff->item(0,1)){
        double cutoff;
        cutoff=_ui.tableWidgetCutoff->item(0, 1)->text().toDouble(&_stm->cmapSTM->_maxSet);

        if (_stm->cmapSTM->_maxSet){

            _stm->cmapSTM->_cutMax=cutoff;

        }
    }


    if (_ui.tableWidgetMTip->item(0, 0))
    {
        _stm->_MTip.x=_ui.tableWidgetMTip->item(0,0)->text().toFloat();
    }

    if (_ui.tableWidgetMTip->item(0, 1))
    {
        _stm->_MTip.y=_ui.tableWidgetMTip->item(0,1)->text().toFloat();
    }

    if (_ui.tableWidgetMTip->item(0, 2))
    {
        _stm->_MTip.z=_ui.tableWidgetMTip->item(0,2)->text().toFloat();
    }


    if (_ui.tableWidgetContrast->item(0, 0))
    {
        _stm->I0=_ui.tableWidgetContrast->item(0,0)->text().toFloat();
    }
    if (_ui.tableWidgetContrast->item(0, 1))
    {
        _stm->TMR=_ui.tableWidgetContrast->item(0,1)->text().toFloat();
    }

    if (_ui.tableWidgetContrast->item(0, 2))
    {
        _stm->TAMR=_ui.tableWidgetContrast->item(0,2)->text().toFloat();
    }

    if (_ui.tableWidgetContrast->item(0, 3))
    {
        _stm->NCMR=_ui.tableWidgetContrast->item(0,3)->text().toFloat();
    }




    _stm->initOrbital();
    _stm->calcSTM();
    getIMG();

}






void StmWindow::getIMG(void){

    int w= _stm->_sizeX;
    int h= _stm->_sizeY;
    int bpp = 3;

   img =QImage(_stm->stmImage.get(),w,h,w*bpp,QImage::Format::Format_RGB888);
   scaleIMG();

}


void StmWindow::scaleIMG(void){
    int h= _stm->_sizeY;
    int w= _stm->_sizeX;
    int wmax=int(_ui.stmOutput->width());
    int hmax=int(_ui.stmOutput->height());
    QImage imgs;
   if (_stm->isTriangular){
       h= int(h*sqrt(3)/2);
       if ((double(h)/w )> (double(hmax)/wmax )) {
           imgs =img.scaled(int(w* double(hmax)/h),hmax,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
       }else{
           imgs = img.scaled(wmax,int(h*double(wmax)/w),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
       }
   }else {
        imgs = img.scaled(wmax,hmax,Qt::KeepAspectRatio,Qt::SmoothTransformation);
   }



   QPixmap pix;
   pix.convertFromImage(imgs,Qt::ColorOnly);
   _ui.stmOutput->setPixmap(pix);//,Qt::SmoothTransformation
}


void StmWindow::resizeEvent(QResizeEvent* event){
    scaleIMG();
}





void StmWindow::chooseMinColor(void){
    double r,g,b;
    _colorPicker.getColor().getRgbF(&r,&g,&b);
    _stm->minColor=Threedim{r,g,b};
    QColor qc;
    qc.setRgbF(r,g,b);
    _ui.pushButtonCMin->setStyleSheet("background-color: "+qc.name());
    if (_ui.comboBoxCMaps->currentText().toStdString()=="cMin->cMax"){
        delete _stm->cmapSTM;
        _stm->cmapSTM = new CMap(_stm->minColor,_stm->maxColor);
        _stm->calcSTM();
        getIMG();
    }


}
void StmWindow::chooseMaxColor(void){
    double r,g,b;
    _colorPicker.getColor().getRgbF(&r,&g,&b);
    _stm->maxColor=Threedim{r,g,b};
    QColor qc;
    qc.setRgbF(r,g,b);
    _ui.pushButtonCMax->setStyleSheet("background-color: "+qc.name());
    if (_ui.comboBoxCMaps->currentText().toStdString()=="cMin->cMax"){
        delete _stm->cmapSTM;
        _stm->cmapSTM = new CMap(_stm->minColor,_stm->maxColor);
        _stm->calcSTM();
        getIMG();
    }

}



void StmWindow::saveCurrendImg(void){
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Save PNG"), "",
           tr(" images (*.png,*.bmp ,*.jpg,*.jpeg);;All Files (*)"));
    img.save(fileName);
}
