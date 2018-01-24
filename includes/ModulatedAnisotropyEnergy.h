/*
* ModulatedAnisotropyEnergy.h
*
*  Created on: 17.07.2015
*      
*/

#ifndef MODULATEDANISOTROPYENERGY_H_
#define MODULATEDANISOTROPYENERGY_H_

#include "Energy.h"

#include "typedefs.h"

class Lattice;

class ModulatedAnisotropyEnergy : public Energy
{
public:
	ModulatedAnisotropyEnergy(Threedim* spinArray, double energyParameter, Threedim direction, 
		Lattice* lattice, double lambda, double width, int modulationNumber);
	virtual ~ModulatedAnisotropyEnergy();
	double single_energy(int position);
	virtual double interaction_energy_between_two_spins(int position1, int position2);

	virtual Threedim effective_field(int position);
	
protected:
	void setup_anisotropy_array(int modulationNumber);
	void setup_anisotropy_array_method1();
	void setup_anisotropy_array_method2();
	void setup_anisotropy_array_method3();
	void setup_anisotropy_array_method4();
	void setup_anisotropy_array_method5();
	void setup_anisotropy_array_method6();
	void setup_anisotropy_array_method7();
	void setup_anisotropy_array_method8();

	Threedim _direction;
	double _lambda;
	double _width;

	int _numberAtoms;
	int _centerSite;
	Threedim* _latticeCoordArray;

	Threedim* _directionArray;
	double* _anisotropyArray;
	double* _anisotropyArrayDefectLine;
};

#endif /* MODULATEDANISOTROPYENERGY_H_ */
