/*
* HexagonalAnisotropyEnergy.cpp
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

#include "HexagonalAnisotropyEnergy.h"

// forward and further includes
#include "MyMath.h"

HexagonalAnisotropyEnergy::HexagonalAnisotropyEnergy(Threedim* spinArray, double energyParameter1, 
	double energyParameter2, double energyParameter3) : 
	Energy(1, "E_HexAniso ", spinArray, 0)
{
	/**
	* @param[in] spinArray Spin configuration
	* @param[in] energyParameter Energy parameter [meV]
	* @param[in] direction Direction vector for spatial orientation of anisotropy axis is normalized
	*/

	_energyParameter1 = energyParameter1;
	_energyParameter2 = energyParameter2;
	_energyParameter3 = energyParameter3;
}

HexagonalAnisotropyEnergy::~HexagonalAnisotropyEnergy()
{
}

double HexagonalAnisotropyEnergy::single_energy(const int &position) const
{
	double sinTheta = MyMath::norm(MyMath::vector_product(_spinArray[position], {0.,0.,1.}));
	double phi = atan2(_spinArray[position].y, _spinArray[position].x);
	return _energyParameter1*pow(sinTheta,2)+_energyParameter2*pow(sinTheta,4)+
		_energyParameter3*pow(sinTheta,6)*cos(6*phi);
}

Threedim HexagonalAnisotropyEnergy::effective_field(const int &position) const
{
	double cosTheta = _spinArray[position].z;
	double sinTheta = MyMath::norm(MyMath::vector_product(_spinArray[position], { 0.,0.,1. }));
	double cosPhi = _spinArray[position].x / sinTheta;
	double sinPhi = _spinArray[position].y / sinTheta;
	
	Threedim field{ 0.,0.,1. };
	field.z = _energyParameter1*2*cosTheta + _energyParameter2*4*(cosTheta-pow(cosTheta,3));
	field.x = -6*pow(sinTheta,5)*cosPhi*(pow(cosPhi,4) - 10*pow(cosPhi,2)*pow(sinPhi,2) + 5*pow(sinPhi,4));
	field.y = 6*pow(sinTheta,5)*sinPhi*(pow(sinPhi, 4) - 10*pow(cosPhi,2)*pow(sinPhi,2) + 5*pow(cosPhi,4));

	return field;
}