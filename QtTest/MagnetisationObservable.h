/*
* MagnetisationObservable.h
*
*  Created on: 20.11.2012
*      
*/

#ifndef MAGNETISATIONOBSERVABLE_H_
#define MAGNETISATIONOBSERVABLE_H_

#include <memory>
#include <string>
#include <sstream> 

#include "typedefs.h"
#include "Observable.h"

class SpinOrientation;

/// Magnetization & susceptibility

class MagnetisationObservable : public Observable
{
public:
	MagnetisationObservable(int numberMeasurements, SpinOrientation* spinOrientation, int boolEachSpin);
	virtual ~MagnetisationObservable();
	virtual std::string get_steps_header(void);
	virtual std::string get_mean_header(void);
	virtual void take_value(void);
	virtual std::string get_step_value(int index);
	virtual std::string get_mean_value(double temperature);
	virtual void clear_storage();

protected:
	SpinOrientation* _spinOrientation;
	Threedim* _values;
	Threedim* _valuesSpinResolved;
	int _numberAtoms;
	int _boolEachSpin;
};

#endif /* MAGNETISATIONOBSERVABLE_H_ */
