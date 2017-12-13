/*
* ModulatedExchangeInteraction.cpp
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

#include "ModulatedExchangeInteraction.h"

// forward and further includes
#include "Lattice.h"
#include "MyMath.h"

ModulatedExchangeInteraction::ModulatedExchangeInteraction(Threedim* spinArray, Lattice* lattice, 
						double Jx, double Jy, double lambda, double alpha, int modulationNumber) :
	Energy(0.5, "ModulatedExchange_Energy ", spinArray, 0)
{
	_Jx = Jx;
	_Jy = Jy;
	_lambda = lambda;
	_alpha = alpha;

	_nbors = lattice->get_number_nth_neighbors(1); //number of first nearest neighbours
	_neighborArray = lattice->get_neighbor_array(1); //neighbour information
	_numberAtoms = lattice->get_number_atoms();
	_modulationArray = new double[_numberAtoms * _nbors];
	_latticeCoordArray = lattice->get_lattice_coordinate_array();
	_firstNeighborVectorArray = lattice->get_neighbor_vector_array(1);
	lattice->find_center_site();
	_centerSite = lattice->get_center_site();
	setup_modulation_array(modulationNumber);
}

ModulatedExchangeInteraction::~ModulatedExchangeInteraction()
{
	delete[] _modulationArray;
}

double ModulatedExchangeInteraction::single_energy(int position)
{
	/**
	* The function evaluates the exchange energy of the atom at site _position.
	*
	* @param[in] _position lattice site
	* @param[in] _spinArray spin configuration
	* @return The energy of the single atom
	*/
	double energy = 0;
	int neighbor = 0; // index of neighbor atom
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position + i] != -1) // -1 refers to empty entry
		{
			neighbor = _neighborArray[_nbors * position + i]; // index of neighbor atom
			energy = energy
				+ _modulationArray[_nbors * position + i]
				* MyMath::dot_product(_spinArray[position], _spinArray[neighbor]);
		}
	}
	return -1 * energy; // energy of single atom
}

double ModulatedExchangeInteraction::interaction_energy_between_two_spins(int position1, int position2)
{
	/**
	* @param[in] _position lattice
	*/
	double energy = 0;
	for (int i = 0; i < _nbors; ++i) // -1 refers to empty entry
	{
		if (_neighborArray[_nbors * position1 + i] == position2) // -1 refers to empty entry
		{
			energy = -_modulationArray[_nbors * position1 + i]
				* MyMath::dot_product(_spinArray[position1], _spinArray[position2]);
		}
	}
	return energy; // energy of single atom
}

Threedim ModulatedExchangeInteraction::effective_field(int position)
{
	return Threedim{ 0,0,0 };
}

void ModulatedExchangeInteraction::setup_modulation_array(int modulationNumber)
{
	switch (modulationNumber)
	{
	case 0:
		gradient_method();
		break;
	case 1:
		wiggleing_method1();
		break;
	case 2:
		wiggleing_method2();
		break;
	case 3:
		wiggleing_method3();
		break;
	case 4:
		wiggleing_method4();
		break;
	case 5:
		wiggleing_method5();
		break;
	case 6:
		wiggleing_method6();
		break;
	case 7:
		wiggleing_method7();
		break;
	case 8:
		wiggleing_method8();
		break;
	case 100:
		anisotropy_method1();
		break;
	default:
		wiggleing_method1();
		break;
	}
}

void ModulatedExchangeInteraction::wiggleing_method1(void)
{
	// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// along x-direction

	Threedim x = {0,0,0};
	int neighbor;
	Threedim ex = {1.0, 0.0, 0.0};
	//	Threedim ey = {0.0, 1.0, 0.0);
	double angle = 0;
	double cosalph = 0;
	double sinalph = 0;
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::add(_latticeCoordArray[neighbor],
					MyMath::mult(_firstNeighborVectorArray[i], -0.5)); // position vector of middle position
				angle = _alpha * sin((x.x - x0) * 2.0 * Pi / _lambda);
				ex.x = cos(angle);
				ex.y = sin(angle);
				//				ey.x = -sin(angle);
				//				ey.y = cos(angle);
				cosalph = MyMath::dot_product(ex, _firstNeighborVectorArray[i])
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				sinalph = MyMath::norm(MyMath::vector_product(ex, _firstNeighborVectorArray[i]))
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				_modulationArray[i] = sqrt(
					pow(_Jx, 2) * pow(cosalph, 2) + pow(_Jy, 2) * pow(sinalph, 2));
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method2(void)
{
	// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// modulation of _Jx,_Jy keeping the ration the same
	// along x-direction

	Threedim x = {0,0,0};
	int neighbor;
	Threedim ex = {1.0,0.0,0.0};
	//	Threedim ey = {0.0, 1.0, 0.0);
	double angle = 0;
	double cosalph = 0;
	double sinalph = 0;
	double _JxMod = 0;
	double _JyMod = 0;
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::difference(_latticeCoordArray[neighbor], _firstNeighborVectorArray[i]); // vector from site to neighboring site
				x = MyMath::add(x, MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
				angle = _alpha * sin((x.x - x0) * 2.0 * Pi / _lambda);
				ex.x = cos(angle);
				ex.y = sin(angle);
				//				ey.x = -sin(angle);
				//				ey.y = cos(angle);
				_JxMod = _Jx * (1. + 0.1 * cos((x.x - x0) * 2.0 * Pi / _lambda));
				_JyMod = _Jy * (1. + 0.1 * cos((x.x - x0) * 2.0 * Pi / _lambda));
				cosalph = MyMath::dot_product(ex, _firstNeighborVectorArray[i])
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				sinalph = MyMath::norm(MyMath::vector_product(ex, _firstNeighborVectorArray[i]))
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				_modulationArray[i] = sqrt(
					pow(_JxMod, 2) * pow(cosalph, 2) + pow(_JyMod, 2) * pow(sinalph, 2));
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method3(void)
{
	// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// modulation of _Jx,_Jy keeping the ration the same
	// along x-direction (sharp transitions between area of larger J and smaller one)

	Threedim x = {0,0,0};
	int neighbor;
	Threedim ex = {1.0,0.0,0.0};
	//	Threedim ey = {0.0, 1.0, 0.0);
	double angle = 0;
	double cosalph = 0;
	double sinalph = 0;
	double _JxMod = 0;
	double _JyMod = 0;
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::difference(_latticeCoordArray[neighbor], _firstNeighborVectorArray[i]); // vector from site to neighboring site
				x = MyMath::add(x, MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
				angle = _alpha * sin((x.x - x0) * 2.0 * Pi / _lambda);
				ex.x = cos(angle);
				ex.y = sin(angle);
				//				ey.x = -sin(angle);
				//				ey.y = cos(angle);
				_JxMod = _Jx * (1. + 2 * MyMath::rectangular_function(2, _lambda, 50, (x.x - x0)));
				_JyMod = _Jy * (1. + 2 * MyMath::rectangular_function(2, _lambda, 50, (x.x - x0)));
				cosalph = MyMath::dot_product(ex, _firstNeighborVectorArray[i])
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				sinalph = MyMath::norm(MyMath::vector_product(ex, _firstNeighborVectorArray[i]))
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				_modulationArray[i] = sqrt(
					pow(_JxMod, 2) * pow(cosalph, 2) + pow(_JyMod, 2) * pow(sinalph, 2));
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method4(void)
{
	// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// along y-direction

	Threedim x = { 0,0,0 };
	int neighbor;
	Threedim ex = {1.0,0.0,0.0};
	//	Threedim ey = {0.0, 1.0, 0.0);
	double angle = 0;
	double cosalph = 0;
	double sinalph = 0;
	double y0 = _latticeCoordArray[_centerSite].y;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::difference(_latticeCoordArray[neighbor], _firstNeighborVectorArray[i]); // vector from site to neighboring site
				x = MyMath::add(x, MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
				angle = _alpha * sin((x.y - y0) * 2.0 * Pi / _lambda);
				ex.x = cos(angle);
				ex.y = sin(angle);
				//				ey.x = -sin(angle);
				//				ey.y = cos(angle);
				cosalph = MyMath::dot_product(ex, _firstNeighborVectorArray[i])
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				sinalph = MyMath::norm(MyMath::vector_product(ex, _firstNeighborVectorArray[i]))
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				_modulationArray[i] = sqrt(
					pow(_Jx, 2) * pow(cosalph, 2) + pow(_Jy, 2) * pow(sinalph, 2));
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method5(void)
{
	/// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// modulation of _Jx,_Jy keeping the ration the same
	// along y-direction (sharp transitions between area of larger J and smaller one)

	Threedim x = { 0,0,0 };
	int neighbor;
	Threedim ex = {1.0,0.0,0.0};
	//	Threedim ey = {0.0, 1.0, 0.0);
	double angle = 0;
	double cosalph = 0;
	double sinalph = 0;
	double _JxMod = 0;
	double _JyMod = 0;
	double y0 = _latticeCoordArray[_centerSite].y;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::difference(_latticeCoordArray[neighbor], _firstNeighborVectorArray[i]); // vector from site to neighboring site
				x = MyMath::add(x, MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
				angle = _alpha * sin((x.y - y0) * 2.0 * Pi / _lambda);
				ex.x = cos(angle);
				ex.y = sin(angle);
				//				ey.x = -sin(angle);
				//				ey.y = cos(angle);
				_JxMod = _Jx * (1. + 2 * MyMath::rectangular_function(0.8, _lambda, 50, (x.y - y0)));
				_JyMod = _Jy * (1. + 2 * MyMath::rectangular_function(0.8, _lambda, 50, (x.y - y0)));
				cosalph = MyMath::dot_product(ex, _firstNeighborVectorArray[i])
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				sinalph = MyMath::norm(MyMath::vector_product(ex, _firstNeighborVectorArray[i]))
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				_modulationArray[i] = sqrt(
					pow(_JxMod, 2) * pow(cosalph, 2) + pow(_JyMod, 2) * pow(sinalph, 2));
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method6(void)
{
	// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// modulation of _Jx,_Jy keeping the ration the same
	// along x-direction (sharp transitions between area of larger J and smaller one)

	Threedim x = { 0,0,0 };
	int neighbor;
	Threedim ex = {1.0,0.0,0.0};
	//	Threedim ey = {0.0, 1.0, 0.0);
	double angle = 0;
	double cosalph = 0;
	double sinalph = 0;
	double _JxMod = 0;
	double _JyMod = 0;
	double x0 = _latticeCoordArray[_centerSite].x;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::difference(_latticeCoordArray[neighbor], _firstNeighborVectorArray[i]); // vector from site to neighboring site
				x = MyMath::add(x, MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
				angle = _alpha * sin((x.x - x0) * 2.0 * Pi / _lambda);
				ex.x = cos(angle);
				ex.y = sin(angle);
				//				ey.x = -sin(angle);
				//				ey.y = cos(angle);
				_JxMod = _Jx * (1. - 0.4 * MyMath::rectangular_function(2, _lambda, 50, (x.x - x0)));
				_JyMod = _Jy * (1. - 0.4 * MyMath::rectangular_function(2, _lambda, 50, (x.x - x0)));
				cosalph = MyMath::dot_product(ex, _firstNeighborVectorArray[i])
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				sinalph = MyMath::norm(MyMath::vector_product(ex, _firstNeighborVectorArray[i]))
					/ MyMath::norm(_firstNeighborVectorArray[i]);
				_modulationArray[i] = sqrt(
					pow(_JxMod, 2) * pow(cosalph, 2) + pow(_JyMod, 2) * pow(sinalph, 2));
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method7(void)
{
	// ellipse (_Jx,_Jy) rotation by angle _alpha with period _lambda
	// along x-direction

	Threedim ex = {1.0,0.0,0.0};
	double neighbor = 1;
	double scalarProd = 0.0;
	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				scalarProd = abs(MyMath::dot_product(_firstNeighborVectorArray[i], ex));
				if (scalarProd > 0.9)
				{
					_modulationArray[i] = _Jx;
				}
				else
				{
					_modulationArray[i] = _Jy;
				}
			}
		}
	}
}

void ModulatedExchangeInteraction::wiggleing_method8(void)
{
	// linear variation of exchange coupling strength along x-direction.
	// isotrop exchange _Jy at minimal x site and _Jx (along x) and _Jy along other bonds at maximum x site.
	Threedim ex = {1.0,0.0,0.0};
	Threedim x = {0,0,0};
	int neighbor;
	double scalarProd = 0.0;
	double xmin = _latticeCoordArray[0].x;
	double xmax = _latticeCoordArray[0].x;
	for (int i = 1; i < _numberAtoms; i++)
	{
		if (_latticeCoordArray[i].x < xmin)
		{
			xmin = _latticeCoordArray[i].x;
		}
		if (_latticeCoordArray[i].x > xmax)
		{
			xmax = _latticeCoordArray[i].x;
		}
	}
	xmin += 0.5;
	xmax -= 0.5;
	double a = (_Jy - _Jx) / (xmin - xmax);
	double b = (_Jx * xmin - _Jy * xmax) / (xmin - xmax);

	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				scalarProd = abs(MyMath::dot_product(_firstNeighborVectorArray[i], ex));
				if (scalarProd > 0.9)
				{
					x = MyMath::difference(_latticeCoordArray[neighbor],
						MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
					_modulationArray[i] = a * x.x + b;
				}
				else
				{
					_modulationArray[i] = _Jy;
				}
			}
		}
	}
}

void ModulatedExchangeInteraction::gradient_method(void)
{

	// linear variation of exchange coupling strength. _Jx at minimal x site and _Jy at maximum x site.

	Threedim x = {0,0,0};
	int neighbor;

	double xmin = _latticeCoordArray[0].x;
	double xmax = _latticeCoordArray[0].x;
	for (int i = 1; i < _numberAtoms; i++)
	{
		if (_latticeCoordArray[i].x < xmin)
		{
			xmin = _latticeCoordArray[i].x;
		}
		if (_latticeCoordArray[i].x > xmax)
		{
			xmax = _latticeCoordArray[i].x;
		}
	}
	xmin += 0.5;
	xmax -= 0.5;
	double a = (_Jx - _Jy) / (xmin - xmax);
	double b = (_Jy * xmin - _Jx * xmax) / (xmin - xmax);

	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 1;
		neighbor = _neighborArray[i];
		if (neighbor != -1)
		{
			if (MyMath::norm(_firstNeighborVectorArray[i]) > PRECISION)
			{
				x = MyMath::difference(_latticeCoordArray[neighbor],
					MyMath::mult(_firstNeighborVectorArray[i], 0.5)); // position vector of middle position
				_modulationArray[i] = a * x.x + b;
			}
		}
	}
}

void ModulatedExchangeInteraction::anisotropy_method1(void)
{

	// linear variation of exchange coupling strength. _Jx at minimal x site and _Jy at maximum x site.

	Threedim ex = {1,0,0};
	double val = 0;

	for (int i = 0; i < _numberAtoms * _nbors; ++i)
	{
		_modulationArray[i] = 0;
		if (_neighborArray[i] != -1)
		{
			val = abs(MyMath::dot_product(ex, _firstNeighborVectorArray[i]));
			if (val > 0.9)
			{
				_modulationArray[i] = _Jx;
			}
			else
			{
				_modulationArray[i] = _Jy;
			}
		}
	}
}

