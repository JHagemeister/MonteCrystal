/*
* SphereMesh.cpp
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

#include "SphereMesh.h"

#include "MyMath.h"

SphereMesh::SphereMesh(QOpenGLFunctions_3_3_Core *glf): Mesh(glf)
{

	generate_mesh_data();
	Mesh::update_buffers();
}

SphereMesh::~SphereMesh()
{
}


void SphereMesh::generate_mesh_data()
{
	_vertices.clear();
	_indices.clear();



    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal

    float sectorStep = 2 * M_PI / segments;
    float stackStep = M_PI / segments;
    float sectorAngle, stackAngle;


    for(int i = 0; i <= segments; ++i)
    {

        stackAngle = M_PI / 2 - i * stackStep;      // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);         // r * sin(u)

        // add (segments+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= segments; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi


            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)


            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            _vertices.push_back(Vertex{{x,y,z},{nx,ny,nz}});

        }
    }
    int k1, k2;
    for(int i = 0; i < segments; ++i)
    {
        k1 = i * (segments + 1);     // beginning of current stack
        k2 = k1 + segments + 1;      // beginning of next stack

        for(int j = 0; j < segments; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                _indices.push_back(k1);
                _indices.push_back(k2);
                _indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (segments-1))
            {
                _indices.push_back(k1 + 1);
                _indices.push_back(k2);
                _indices.push_back(k2 + 1);
            }
        }
     }

}
