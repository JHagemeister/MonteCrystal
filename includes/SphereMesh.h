#ifndef SPHEREMODEL_H
#define SPHEREMODEL_H

#include "Mesh.h"

#include "typedefs.h"

#include "Shader.h"

class SphereMesh: public Mesh
{
public:
    SphereMesh(QOpenGLFunctions_3_3_Core *glf);
    ~SphereMesh();

    void generate_mesh_data(void);

    int segments=10;
    float radius=0.5;

private:
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep ;
    float stackStep ;
    float sectorAngle, stackAngle;



};

#endif // SPHEREMODEL_H
