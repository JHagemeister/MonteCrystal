/*
* Metropolis.h
*
*      
*/

#ifndef METROPOLIS_H_
#define METROPOLIS_H_

#include "SimulationMethod.h"

#include <QSharedPointer>

#include "typedefs.h"

class SpinOrientation;
class RanGen;
class Hamiltonian;

/// Metropolis algorithm

class Metropolis : public SimulationMethod
{
public:
	Metropolis(SpinOrientation* spinOrientation, int simulationSteps, double temperature, 
		QSharedPointer<Hamiltonian> hamilton, std::shared_ptr<RanGen> ranGen,
		SimulationProgram* simulationProgram);
	virtual ~Metropolis();
	virtual double simulation_step(void);

private:
	std::vector<int> _randomizedSiteList;
};

#endif /* METROPOLIS_H_ */
