#ifndef COLORMAP_H
#define COLORMAP_H
#include "MyMath.h"
#include <glm/vec4.hpp>
#include <vector>
#include <QString>
#include <QSharedPointer>
class CMap
{
public:

    CMap(Threedim colorMin,Threedim colorMax);
    CMap(QString name);
    ~CMap();

    double _cutMin=0, _cutMax=0;
    bool _minSet=0,_maxSet=0;
    QString name;
    std::vector<double>  steps;
    std::vector<Threedim>    cMin;
    std::vector<Threedim>   cGrad;
    double realMinVal; double realMaxVal;    
    void getMinMax(double* values,int _numElements);
    void mapColors(int numElements,double* values,glm::vec3* colors);

};

#endif // COLORMAP_H
