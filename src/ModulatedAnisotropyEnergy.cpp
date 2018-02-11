/*
* ModulatedAnisotropyEnergy.cpp
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

#include "ModulatedAnisotropyEnergy.h"

// forward and further includes
#include "Lattice.h"
#include "MyMath.h"

ModulatedAnisotropyEnergy::ModulatedAnisotropyEnergy(Threedim* spinArray, double energyParameter, 
	Threedim direction, Lattice* lattice, double lambda, double width, int modulationNumber):
	Energy(1, "ModulatedAnisotropy_Energy ", spinArray, energyParameter)
{
	_direction = direction;
	_lambda = lambda;
	_width = width;

	_numberAtoms = lattice->get_number_atoms();
	lattice->find_center_site();
	_centerSite = lattice->get_center_site();
	_latticeCoordArray = lattice->get_lattice_coordinate_array();

	_directionArray = new Threedim[_numberAtoms];
	_anisotropyArray = new double[_numberAtoms];
	_anisotropyArrayDefectLine = new double[_numberAtoms];

	for (int i = 0; i < _numberAtoms; i++)
	{
		_directionArray[i] = _direction;
		_anisotropyArray[i] = _energyParameter;
		_anisotropyArrayDefectLine[i] = 0;
	}
	
	setup_anisotropy_array(modulationNumber);
}

ModulatedAnisotropyEnergy::~ModulatedAnisotropyEnergy()
{
	delete[] _anisotropyArray;
	delete[] _directionArray;
	delete[] _anisotropyArrayDefectLine;
}

double ModulatedAnisotropyEnergy::single_energy(const int &position) const
{
	/**
	* The function evaluates the exchange energy of the atom at a  given
	* lattice site.
	*
	* @param[in] _position lattice site
	* @param[in] _spinArray spin configuration
	* @return The energy of the single atom.
	*/
	double cos = MyMath::cosine_vectors(_spinArray[position], _directionArray[position]);
	double cos2 = MyMath::cosine_vectors(_spinArray[position], _direction);
	return (_anisotropyArray[position] * (1 - cos * cos)
		+ _anisotropyArrayDefectLine[position] * (1 - cos2 * cos2));
}

Threedim ModulatedAnisotropyEnergy::effective_field(const int &position) const
{
	return Threedim{ 0,0,0 };
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array(int modulationNumber)
{
	switch (modulationNumber)
	{
	case 1:
		setup_anisotropy_array_method1();
		break;
	case 2:
		setup_anisotropy_array_method2();
		break;
	case 3:
		setup_anisotropy_array_method3();
		break;
	case 4:
		setup_anisotropy_array_method4();
		break;
	case 5:
		setup_anisotropy_array_method5();
		break;
	case 6:
		setup_anisotropy_array_method6();
		break;
	case 7:
		setup_anisotropy_array_method7();
		break;
	case 8:
		setup_anisotropy_array_method8();
		break;
	default:
		setup_anisotropy_array_method1();
		break;
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method1()
{
	double x = 0;
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		_anisotropyArray[i] = _energyParameter * (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = {0,1,0};
		_direction = {0,0,1};
		_anisotropyArrayDefectLine[i] = _energyParameter * MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method2()
{
	double x = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		_anisotropyArray[i] = 0.8 * _energyParameter
			* (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = {0,1,0};
		_direction = {0,0,1};
		_anisotropyArrayDefectLine[i] = -_energyParameter
			* MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method3()
{
	double x = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		_anisotropyArray[i] = 0.6 * _energyParameter
			* (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = {0,1,0};
		_direction = {0,0,1};
		_anisotropyArrayDefectLine[i] = -_energyParameter
			* MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method4()
{
	//	Threedim ex = {1.0, 0.0, 0.0);
	Threedim ey = { 0.0, 1.0, 0.0 };
	double alpha = 0.5;
	double angle = 0;
	double x = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		angle = alpha * sin((x - x0) * 2.0 * Pi / _lambda);
		//		ex.x = cos(angle);
		//		ex.y = sin(angle);
		ey.x = -sin(angle);
		ey.y = cos(angle);
		_anisotropyArray[i] = _energyParameter * (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = { ey.x, ey.y, 0 };
		_direction = {0,0,1};
		_anisotropyArrayDefectLine[i] = -_energyParameter
			* MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method5()
{
	//	Threedim ex = {1.0, 0.0, 0.0);
	Threedim ey = { 0.0, 1.0, 0.0 };
	double alpha = 0.5;
	double angle = 0;
	double x = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		angle = alpha * sin((x - x0) * 2.0 * Pi / _lambda);
		//		ex.x = cos(angle);
		//		ex.y = sin(angle);
		ey.x = -sin(angle);
		ey.y = cos(angle);
		_anisotropyArray[i] = _energyParameter * (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = { ey.x, ey.y, 0 };
		_direction = { 1, 0, 0 };
		_anisotropyArrayDefectLine[i] = _energyParameter * MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method6()
{
	//	Threedim ex = {1.0, 0.0, 0.0);
	Threedim ey = { 0.0, 1.0, 0.0 };
	double alpha = 0.5;
	double angle = 0;
	double x = 0;
	double z = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		angle = alpha * sin((x - x0) * 2.0 * Pi / _lambda);
		//		ex.x = cos(angle);
		//		ex.y = sin(angle);
		ey.x = -sin(angle);
		ey.y = cos(angle);
		_anisotropyArray[i] = _energyParameter
			* MyMath::rectangular_function(_lambda - _width, _lambda, 50, (x - x0 + _lambda / 2.0));
		z = 20
			* MyMath::rectangular_function(_lambda - _width, _lambda * 2.0, 50,
			(x - x0 + _lambda / 2.0));
		_directionArray[i] = MyMath::normalize({ ey.x, ey.y, z });
		_direction = { 1, 0, 0 };
		_anisotropyArrayDefectLine[i] = _energyParameter * MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method7()
{
	//	Threedim ex = {1.0, 0.0, 0.0);
	Threedim ey = { 0.0, 1.0, 0.0 };
	double alpha = 0.5;
	double angle = 0;
	double x = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		angle = alpha * sin((x - x0) * 2.0 * Pi / _lambda);
		//		ex.x = cos(angle);
		//		ex.y = sin(angle);
		ey.x = -sin(angle);
		ey.y = cos(angle);
		_anisotropyArray[i] = _energyParameter * (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = { ey.x, ey.y, 0 };
		_direction = {0,0,1};
		_anisotropyArrayDefectLine[i] = -_energyParameter
			* MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}

void ModulatedAnisotropyEnergy::setup_anisotropy_array_method8()
{
	double x = 0;
	
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms; i++)
	{
		x = _latticeCoordArray[i].x;
		_anisotropyArray[i] = _energyParameter * (1 - MyMath::rectangular_function(_width, _lambda, 50, (x - x0)));
		_directionArray[i] = {0,1,0};
		_direction = { 1, 0, 0 };
		_anisotropyArrayDefectLine[i] = _energyParameter * MyMath::rectangular_function(_width, _lambda, 50, (x - x0));
	}
}
