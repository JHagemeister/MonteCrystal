/*
* Converger1.h
*
*  Created on: 11.04.2017
*      
*/

#ifndef CONVERGER1_H_
#define CONVERGER1_H_

#include "SimulationMethod.h"

#include <QSharedPointer>

#include "typedefs.h"

class SpinOrientation;
class Hamiltonian;
class RanGen;

/// Converger - spins set into direction of effective field

class Converger1 : public SimulationMethod
{
public:
	Converger1(SpinOrientation* spinOrientation, int simulationSteps, double temperature,
		QSharedPointer<Hamiltonian> hamilton, std::shared_ptr<RanGen> ranGen, 
		SimulationProgram* simulationProgram);
	virtual ~Converger1();
	virtual double simulation_step(void);
};

#endif /* CONVERGER1_H_ */