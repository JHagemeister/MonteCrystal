#include "CMap.h"
#include "MyMath.h"
#include <glm/vec4.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QSharedDataPointer>
CMap::CMap(Threedim colorMin,Threedim colorMax )
{

    name = "minMax";
    steps.clear();
    cMin.clear();
    cGrad.clear();
    steps.push_back(0);
    cMin.push_back(colorMin);
    cGrad.push_back(MyMath::difference( colorMax, colorMin));

}

CMap::CMap(QString filename){

    name = filename;

    steps.clear();
    cMin.clear();
    cGrad.clear();
    std::ifstream infile(std::string ("cmaps/")+ filename.toStdString());
    std::string line;
    std::getline(infile, line); //skip header
    while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            double step, r, g, b;
            if (!(iss >> step >> r >> g>> b)) { break; } // error
            steps.push_back(step);
            cMin.push_back(Threedim{r,g,b});

        }

    for (uint i=1;i< cMin.size();i++){
        cGrad.push_back(MyMath::mult(MyMath::difference(cMin[i],cMin[i-1]) ,1/(steps[i]-steps[i-1])));
    }


}
CMap::~CMap(){
}


void CMap::getMinMax(double* values,int _numElements){
    MyMath::min_max_double(values,  _numElements,realMinVal,realMaxVal);
}


void CMap::mapColors(int numElements,double *values,glm::vec3* colors){
    getMinMax(values,numElements);
    for(int atom=0;atom<numElements;atom++){
        float scaled =0;
        if (realMaxVal>realMinVal){

            if(_maxSet && _minSet){
                if (values[atom]>=_cutMax){
                    values[atom]=_cutMax;
                }
                else if (values[atom]<=_cutMin){
                    values[atom]=_cutMin;
                }
                scaled=(values[atom]-_cutMin)/(_cutMax-_cutMin);
            }else   if(_maxSet && !_minSet){
                if (values[atom]>=_cutMax){
                    values[atom]=_cutMax;
                }

                scaled=(values[atom]-realMinVal)/(_cutMax-realMinVal);
            }else  if(!_maxSet && _minSet){
                if (values[atom]<=_cutMin){
                    values[atom]=_cutMin;
                }

                scaled=(values[atom]-_cutMin)/(realMaxVal-_cutMin);
            }else{
            scaled= (values[atom]-realMinVal)/(realMaxVal-realMinVal);
            }
        }

        for (int step =cGrad.size()-1; step >-1  ;step--){

            if(scaled >=steps[step]){
                Threedim c=MyMath::add( cMin[step] , MyMath::mult(cGrad[step],(scaled-steps[step]))) ;
                colors[atom]=glm::vec4(c.x,c.y,c.z,1.0);

                break;
            }
        }
    }
}




void CMap::mapColors(int w,int h ,double** values,uchar*  imgBuff){

    MyMath::min_max_2d_double(values,w,h,realMinVal,realMaxVal);
    for(int y=0;y<h;y++){
        for (int x =0;x<w;x++){
            if (3*(y*w+x)+2 > 3*w*h ){break;}
            float scaled =0;
            if (realMaxVal>realMinVal){
                if(_maxSet && _minSet){
                    if (values[x][y]>=_cutMax){
                        values[x][y]=_cutMax;
                    }
                    else if (values[x][y]<=_cutMin){
                        values[x][y]=_cutMin;
                    }
                    scaled=(values[x][y]-_cutMin)/(_cutMax-_cutMin);
                }else   if(_maxSet && !_minSet){
                    if (values[x][y]>=_cutMax){
                        values[x][y]=_cutMax;
                    }

                    scaled=(values[x][y]-realMinVal)/(_cutMax-realMinVal);
                }else  if(!_maxSet && _minSet){
                    if (values[x][y]<=_cutMin){
                        values[x][y]=_cutMin;
                    }

                    scaled=(values[x][y]-_cutMin)/(realMaxVal-_cutMin);
                }else{
                scaled= (values[x][y]-realMinVal)/(realMaxVal-realMinVal);
                }
            }

            for (int step =cGrad.size()-1; step >-1  ;step--){

                if(scaled >=steps[step]){
                    Threedim c=MyMath::add( cMin[step] , MyMath::mult(cGrad[step],(scaled-steps[step]))) ;
                    imgBuff[3*(y*w+x)]=char(c.x*255);
                    imgBuff[3*(y*w+x)+1]=char(c.y*255);
                    imgBuff[3*(y*w+x)+2]=char(c.z*255);
                    break;
                }
            }

        }
    }

}


