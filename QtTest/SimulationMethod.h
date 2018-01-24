/*
* SimulationMethod.h
*
*  Created on: 19.03.2012
*      
*/

#ifndef SIMULATIONMETHOD_H_
#define SIMULATIONMETHOD_H_

#include <memory>
#include <string>

#include <QSharedPointer>

// own
#include "typedefs.h"

class SpinOrientation;
class Hamiltonian;
class RanGen;
class Measurement;
class SimulationProgram;
class Lattice;

/// Basis class for simulation methods as Metropolis algorithm or Landau-Lifshitz-Gilbert (LLG) equation

class SimulationMethod
{
public:
	SimulationMethod(SpinOrientation* spinOrientation, int simulationSteps, double temperature,	
		QSharedPointer<Hamiltonian> hamilton, std::shared_ptr<RanGen> ranGen, SimulationProgram* simulationProgram = NULL);
	virtual ~SimulationMethod();

	/// definition of simulation step depends on algorithm and is implemented in derived class
	virtual double simulation_step(void) = 0;
	/// sets equal temperature at each spin
	virtual void set_temperature(double temperature);
	/// sets temperature gradient along spin system
	virtual void set_temperature_gradient(double temperatureMin, double temperatureMax, Threedim direction,
		Lattice* lattice);

	/// Perform _simulationSteps numbers of simulation steps at constant energy parameters 
	void run_simulation(int uiUpdateWidth, std::shared_ptr<Measurement> measurement, int outputWidth, 
		int movieStart, int movieEnd, 
		int movieWidth, std::string fname);
	/// Perform simulationSteps numbers of simulation steps at constant energy parameters 
	void relaxate(int simulationSteps);

protected:
	SpinOrientation* _spinOrientation; ///< spin configuration
	int _simulationSteps; ///< number of simulation steps
	double* _temperature; ///< temperature at each spin
	QSharedPointer<Hamiltonian> _hamilton; ///< Hamiltonian to calculate energies
	std::shared_ptr<RanGen> _ranGen; ///< pseudo random number generator

	SimulationProgram* _simulationProgram;
	
	int _numberActiveSites; ///< number of spin sites active for update during simulation
	int* _activeSites; ///< indexes of active sites
	int _numberInactiveSites; ///< number of sites inactive for update during simulation
	int* _inactiveSites; ///< number of inactive sites

	int _boolConvergenceCriterion;
};

#endif /* SIMULATIONMETHOD_H_ */
