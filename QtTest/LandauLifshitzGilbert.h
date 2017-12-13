/*
* MonteCarloSimulation.h
*
*  Created on: 11.04.2017
*      
*/

#ifndef LANDAULIFSHITZGILBERT_H_
#define LANDAULIFSHITZGILBERT_H_

#include "SimulationMethod.h"

#include <qsharedpointer.h>

#include "typedefs.h"

class SpinOrientation;
class Hamiltonian;
class RanGen;

/// Landau-Lifshitz-Gilbert equation

class LandauLifshitzGilbert: public SimulationMethod
{
public:
	LandauLifshitzGilbert(SpinOrientation* spinOrientation, int simulationSteps, double temperature, 
		QSharedPointer<Hamiltonian> hamilton, std::shared_ptr<RanGen> ranGen, 
		double timeWidth, double dampingParameter, double magneticMoment, 
		SimulationProgram* simulationProgram);
	virtual ~LandauLifshitzGilbert();
	virtual double simulation_step(void);
	virtual void set_temperature(double temperature);
	virtual void set_temperature_gradient(double temperatureMin, double temperatureMax,
		Threedim direction, Lattice* lattice);

protected:
	double _timeWidth; ///< time width in [s] (see constructor)
	double _sqrtTimeWidth; ///< square root of time width [s**(1/2)]
	double _dampingParameter; ///< Gilbert damping parameter; 1 for fastest damping process
	double _magneticMoment; ///< magnetic moment in multiples of muBohr
	double _inverseMagneticMoment; ///< inverse magnetic moment (SI units)

	double _reducedGyromagneticRatio;
	double* _thermalFieldVariance; ///< Factor for thermal field; individual for each spin due to temperature

private:
	void standard_normal_variables(Threedim &vec1, Threedim &vec2);
	void set_thermal_field_variance(void);
	void set_reduced_gyromagnetic_ratio(void);
};

#endif /* LANDAULIFSHITZGILBERT_H_ */

