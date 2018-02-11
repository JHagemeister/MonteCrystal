/*
* ModulatedExchangeEnergy.h
*
*  Created on: 20.02.2015
*      
*/

#ifndef MODULATEDEXCHANGEINTERACTION_H_
#define MODULATEDEXCHANGEINTERACTION_H_

#include "Energy.h"

#include "typedefs.h"

class Lattice;

class ModulatedExchangeInteraction : public Energy
{
public:
	ModulatedExchangeInteraction(Threedim* spinArray, Lattice* lattice, double Jx, double Jy, 
								double lambda, double alpha, int modulationNumber);
	virtual ~ModulatedExchangeInteraction();
	double single_energy(const int &position) const;
	virtual Threedim effective_field(const int &position) const;

protected:
	void setup_modulation_array(int modulationNumber);
	void wiggleing_method1(void);
	void wiggleing_method2(void);
	void wiggleing_method3(void);
	void wiggleing_method4(void);
	void wiggleing_method5(void);
	void wiggleing_method6(void);
	void wiggleing_method7(void);
	void wiggleing_method8(void);
	void gradient_method(void);
	void anisotropy_method1(void);
	
	int _nbors;
	int* _neighborArray;
	double _Jx;
	double _Jy;
	double _lambda;
	double _alpha;
	int _numberAtoms;
	int _centerSite;

	double* _modulationArray;
	Threedim* _latticeCoordArray;
	Threedim* _firstNeighborVectorArray;
	
};

#endif /* MODULATEDEXCHANGEINTERACTION_H_ */
