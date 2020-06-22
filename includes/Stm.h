#ifndef STM_H
#define STM_H
#include "vector"
#include "OGLWidget.h"
#include "CMap.h"
#include "typedefs.h"
#include "Lattice.h"
#include <cmath>
#include <QImage>
#include <glm/vec3.hpp>
#include <QSharedPointer>
class STM
{

public:
    STM( Threedim * spins, int*  numberAtoms,QSharedPointer<Lattice> lattice);
    ~STM();
    void initParams(void);
    void initParams(float radius, float kappa, int dpa, float h);
    void initOrbital(void);
    void initImage(void);
    void calcSTM(void);
    double calcScale(int atom);
    void checkSym(void);
    CMap * cmapSTM;
    QSharedPointer<Lattice> _lattice;
    Threedim* _latticeCoord;
    Threedim* _spins;
    int * _firstNeighbors;
    int *_numberAtoms;
    float _radius;
    float _kappa;
    int _dpa;
    int _rDots;
    int _wGauss;
    float _h;
    int _sizeX;
    int _sizeY;
    double** _gaussarray;
    bool isTriangular=FALSE;
    std::vector<double> layers;
    QSharedPointer<double*> stmData;
    QSharedPointer<uchar> stmImage;
    Threedim min = { 0,0,0 };
    Threedim max = { 0,0,0 };
    Threedim _MTip= {0,0,1};
    float I0=1, TMR=1, TAMR=0 ,NCMR=0;
    int _WindowWidth=800, _WindowHeight = 800;
    Threedim minColor=Threedim{1,0.1,0.1};
    Threedim maxColor=Threedim{1,1,1};

};

#endif // STM_H
