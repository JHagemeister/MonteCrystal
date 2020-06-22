#include "Stm.h"
#include "OGLWidget.h"
#include "typedefs.h"
#include "MyMath.h"
#include "Lattice.h"
#include <iostream>
#include <cmath>
#include <glm/vec3.hpp>
#include "NCMRContrastObservable.h"
#include <QSharedPointer>
STM::STM( Threedim * spins, int* numberAtoms,QSharedPointer<Lattice> lattice)
{
    _lattice=lattice;
    _latticeCoord =lattice->get_lattice_coordinate_array();
    _spins = spins;
    _numberAtoms = numberAtoms;
    _firstNeighbors =lattice->get_neighbor_array(1);
    _spins = spins;
    _numberAtoms = numberAtoms;
    checkSym();
    initParams();
    cmapSTM = new CMap("seismic");
    calcSTM();
}
STM::~STM(){

    }

void  STM::initOrbital(){
    _rDots=int(_radius*_dpa);
    _wGauss=2*_rDots+1;
    layers =MyMath::get_layer_heigts(*_numberAtoms,_latticeCoord);



    _gaussarray = new double * [layers.size()];
    double z0= layers[layers.size()-1];
    for (uint l=0; l < layers.size();l++){
        _gaussarray[l]=new double[_wGauss*_wGauss];
        for (int x =0;x<_wGauss;x++){
            for (int y =0;y<_wGauss;y++){
                float r=0;
                if(isTriangular){
                     r= sqrt(pow((-_rDots+x),2)+pow((-_rDots+y)/2*sqrt(3),2) +pow(z0-layers[l]+_h,2))/_dpa;

                }else{
                     r= sqrt(pow((-_rDots+x),2)+pow((-_rDots+y),2) +pow(z0-layers[l]+_h,2))/_dpa;

                }
                _gaussarray[l][x*(_wGauss)+y]=exp(-r*2*_kappa);

            }
        }
    }


}


void STM::initParams(void){
    checkSym();
    if (isTriangular){
        _radius = 3;
        _dpa = 31;
        _kappa = 1.2;
        _h=2;
    }else{
        _radius = 4;
        _dpa = 20;
        _kappa = 1;
        _h=2;
    }


    initOrbital();

}

void STM::initParams(float radius, float kappa, int dpa, float h){
    _radius = radius;
    _dpa = dpa;
    _kappa = kappa;

    _h=h;

    initOrbital();

}
void STM::initImage(){

    MyMath::min_max_threedim(_latticeCoord,* _numberAtoms, min, max);
    _sizeX=int(( max.x - min.x)*_dpa)+_wGauss+1;
    if (isTriangular){
       _sizeY=int(( max.y - min.y)/sqrt(3)*2*_dpa)+_wGauss+1;
    }else{
        _sizeY=int(( max.y - min.y)*_dpa)+_wGauss+1;
    }
    stmData.clear();
    stmData= QSharedPointer<double *>(new double* [_sizeX]);
    for (int i=0 ;i<_sizeX;i++){
        stmData.get()[i]= new double[_sizeY]{0};
    }
    stmImage=QSharedPointer<uchar> (new uchar[_sizeX*_sizeY*3]);
}
void STM::calcSTM(){
    initImage();
    int startx =0;
    int starty =0;
    double scale = 0;
    for (int atom=0;atom<*_numberAtoms;atom++){

         startx= qRound((_latticeCoord[atom].x-min.x)*_dpa);
         if (isTriangular){
             starty= qRound((_latticeCoord[atom].y-min.y)*_dpa/sqrt(3)*2);
         }else {
             starty= qRound((_latticeCoord[atom].y-min.y)*_dpa);
         }

        scale = calcScale(atom);
        if (scale >0){
            for (uint l =0 ;l<layers.size();l++){
                if (_latticeCoord[atom].z == layers[l]){
                    for (int y = 0 ; y <_wGauss;y++ ){
                        for(int x =0;x < _wGauss;x++){
                            if(startx+x<_sizeX && starty+y <_sizeY){
                                double val =_gaussarray[l][y*_wGauss+x]*scale;
                                stmData.get()[startx+x][ y+starty]+=val;
                            }
                        }

                    }

                }
            }
        }


    }


    cmapSTM->mapColors(_sizeX,_sizeY,stmData.get(),stmImage.get());


}


double STM::calcScale(int atom){
    float scale=I0;
    if (TMR!=0){
        scale += TMR *(1+ MyMath::dot_product(_MTip,_spins[atom]));
    }
    if (TAMR!=0){
        scale += TAMR * pow( MyMath::dot_product(Threedim{0,0,1},_spins[atom]),2);
    }
    if (NCMR!=0){
        int neighbor=0;
        int _nbors=  _lattice->get_number_nth_neighbors(1);
        double ncmrVal=0;
        int realNbors =0;

        for (int n = 0; n < _lattice->get_number_nth_neighbors(1);n++){
            if (_firstNeighbors[_nbors * atom+ n] != -1) // -1 refers to empty entry
            {
                neighbor = _firstNeighbors[_nbors * atom + n]; // index of neighbor atom

                double val = acos (MyMath::cosine_vectors(_spins[atom], _spins[neighbor]));

                realNbors ++ ;
                ncmrVal += val;

            }
        }
        scale +=NCMR * ( ncmrVal/realNbors);



    }
    return scale;
}

void STM::checkSym(void){

    if(_lattice->_latticeType==triangularDisk ||
       _lattice->_latticeType==triangularStripe ||
       _lattice->_latticeType==triangularHalfDisk ||
       _lattice->_latticeType==triangularArrowHead ||
       _lattice->_latticeType==triangularHexagonal ||
       _lattice->_latticeType==triangularTriangular){
       isTriangular=TRUE;
    }
}
