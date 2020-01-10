/*
* EnergyObservable.h
*
*  Created on: 20.11.2012
*      
*/

#ifndef ENERGYOBSERVABLE_H_
#define ENERGYOBSERVABLE_H_

#include <QSharedPointer>

#include "Observable.h"

class Hamiltonian;

/// Energy & heat capacity

class EnergyObservable : public Observable
{
public:
	EnergyObservable(int numberMeasurements, QSharedPointer<Hamiltonian> hamilton,	int numberAtoms,
		             bool eachSpin);
	virtual ~EnergyObservable();
	virtual std::string get_steps_header(void) const;
	virtual std::string get_mean_header(void) const;
	virtual void take_value(void);
	virtual std::string get_step_value(const int &index) const;
	virtual std::string get_mean_value(const double &temperature) const;
	virtual void clear_storage();

protected:
	QSharedPointer<Hamiltonian> _hamilton; ///< Hamiltonian for energy calculation
	int _numberEnergies; ///< number of energy objects in Hamiltonian
	int _numberAtoms; ///< number of lattice sites
	bool _eachSpin;

	double** _singleEnergies; ///< storage room for measurement values resolved to energy objects
	double*** _singleEnergiesperspin; ///< storage room for measurement values resolved to energy objects and spins
};

#endif /* ENERGYOBSERVABLE_H_ */
