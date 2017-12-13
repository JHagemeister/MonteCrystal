/*
* SpinMesh.cpp
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

#include "SpinMesh.h"

#include "MyMath.h"

SpinMesh::SpinMesh(SpinMeshParams params): Mesh()
{
	adjust_parameters(params);
	generate_mesh_data();
	Mesh::update_buffers();
}

SpinMesh::~SpinMesh()
{
}

void SpinMesh::update(SpinMeshParams params)
{
	adjust_parameters(params);
	generate_mesh_data();
	Mesh::update_buffers();
}

void SpinMesh::adjust_parameters(SpinMeshParams params)
{
	_n = params.n;
	_r1 = 0.25;
	_r2 = params.r2divr1*_r1;
	_l1 = params.r2divr1*params.l1divl2*_r1 / params.r2divl2;
	if (_l1 < 0.05)
	{
		_l1 = 0;
	}
	_l2 = params.r2divr1*_r1 / params.r2divl2;

	_zg = (-0.5*pow(_l1, 2)*pow(_r1, 2) + (1. / 12.)*pow(_l2, 2)*pow(_r2, 2)) /
		(_l1*pow(_r1, 2) + (1. / 3.)*_l2*pow(_r2, 2));

	float val1 = abs(_l1 - _zg);
	float val2 = abs(_l2 - _zg);
	float max = val1;
	if (val2 > val1) { max = val2; }
	if (max > 1)
	{
		_r1 /= max;
		_r2 /= max;
		_l1 /= max;;
		_l2 /= max;
		_zg /= max;
	}

	if (_r2 > 1)
	{
		_r1 /= _r2;
		_r2 /= _r2;
		_l1 /= _r2;
		_l2 /= _r2;
		_zg /= _r2;
	}
}

void SpinMesh::generate_mesh_data(void)
{
	_vertices.clear();
	_indices.clear();
	
	// Cone
	
	// Vertices
	for (int i = 0; i < _n; ++i)
	{
		double alpha = i * 2 * Pi / _n;
		Threedim vec1 = MyMath::normalize({ _l2*cos(alpha), _l2*sin(alpha), _r2 });
		alpha = (i+1) * 2 * Pi / _n;
		Threedim vec2 = MyMath::normalize({ _l2*cos(alpha), _l2*sin(alpha), _r2 });
		Threedim vec = MyMath::normalize(MyMath::add(vec1, vec2));
		
		_vertices.push_back(Vertex{ {0, 0, _l2-_zg },
		{ (float)vec.x, (float)vec.y, (float)vec.z } });
		_vertices.push_back(Vertex{ { _r2*cos(alpha), _r2*sin(alpha), -_zg },
		{ (float)vec1.x, (float)vec1.y, (float)vec1.z } });
	}

	_vertices.push_back(Vertex{ { 0,0,-_zg },{ 0,0,-1 } });
	for (int i = 0; i < _n; ++i)
	{
		double alpha = i*2*Pi / _n;
		_vertices.push_back(Vertex{ { _r2*cos(alpha), _r2*sin(alpha), -_zg },
		{ 0,0,-1 } });
	}
	
	
	// Indices
	for (GLuint i = 0; i < (GLuint) (_n-1); ++i)
	{
		_indices.push_back(2*i);
		_indices.push_back(2*i+1);
		_indices.push_back(2*i+3);
	}
	_indices.push_back((GLuint) 2*(_n-1));
	_indices.push_back((GLuint) 2*(_n-1)+1);
	_indices.push_back(1);

	for (GLuint i = 1; i < (GLuint)_n; ++i)
	{
		_indices.push_back((GLuint) 2*_n);
		_indices.push_back(((GLuint) (2*_n))+i);
		_indices.push_back(((GLuint)(2 * _n)) + i + 1);
	}
	_indices.push_back((GLuint)2 * _n);
	_indices.push_back((GLuint)(2*_n+_n));
	_indices.push_back((GLuint)2 * _n + 1);
	

	// Cylinder
	if (_l1 > 0.01)
	{
		// bottom
		int index = _vertices.size();
		_vertices.push_back(Vertex{ { 0,0, -_l1-_zg },{ 0,0,-1 } });
		for (int i = 0; i < _n; ++i)
		{
			double alpha = i*2*Pi / _n;
			_vertices.push_back(Vertex{ { _r1*cos(alpha),_r1*sin(alpha),-_l1-_zg },{ 0,0,-1 } });
		}

		for (GLuint i = 1; i < (GLuint)_n; ++i)
		{
			_indices.push_back((GLuint)index);
			_indices.push_back(((GLuint)index)+i);
			_indices.push_back(((GLuint)index)+i+1);
		}
		_indices.push_back((GLuint)index);
		_indices.push_back((GLuint)index+_n);
		_indices.push_back((GLuint)index + 1);

		// sides
		index = _vertices.size();
		for (int i = 0; i < _n; ++i)
		{
			double alpha = i*2*Pi / _n;
			_vertices.push_back(Vertex{ { _r1*cos(alpha),_r1*sin(alpha),-_l1-_zg },{ cos(alpha),sin(alpha),0 } });
		}
		for (int i = 0; i < _n; ++i)
		{
			double alpha = i*2*Pi / _n;
			_vertices.push_back(Vertex{ { _r1*cos(alpha),_r1*sin(alpha), -_zg },{ cos(alpha),sin(alpha),0 } });
		}

		for (GLuint i = 0; i < (GLuint)_n - 1; ++i)
		{
			_indices.push_back(((GLuint)index) + i);
			_indices.push_back(((GLuint)index) + i + 1);
			_indices.push_back(((GLuint)index + _n) + i);

			_indices.push_back(((GLuint)index) + i + 1);
			_indices.push_back(((GLuint)index + _n) + i + 1);
			_indices.push_back(((GLuint)index + _n) + i);
		}
		_indices.push_back((GLuint)index + _n - 1);
		_indices.push_back((GLuint)index);
		_indices.push_back((GLuint)index + 2*_n - 1);

		_indices.push_back((GLuint)index);
		_indices.push_back((GLuint)index + 2 * _n - 1);
		_indices.push_back((GLuint)index + _n);
	}
}
