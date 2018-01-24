/*
* ABSMagnetisationObservable.h
*
*  Created on: 24.01.2013
*      
*/

#ifndef ABSOLUTEMAGNETISATIONOBSERVABLE_H_
#define ABSOLUTEMAGNETISATIONOBSERVABLE_H_

#include "typedefs.h"
#include "Observable.h"

class SpinOrientation;

/// Absolute magnetization information: average(|Si|)

class AbsoluteMagnetisationObservable : public Observable
{
public:
	AbsoluteMagnetisationObservable(int numberMeasurements, SpinOrientation* spinOrientation);
	virtual ~AbsoluteMagnetisationObservable();
	virtual std::string get_steps_header(void);
	virtual std::string get_mean_header(void);
	virtual void take_value(void);
	virtual std::string get_step_value(int index);
	virtual std::string get_mean_value(double temperature);
	virtual void clear_storage();

private:
	SpinOrientation* _spinOrientation;
	Threedim* _absoluteMagnetisation;
	int _numberAtoms;
};

#endif /* ABSOLUTEMAGNETISATIONOBSERVABLE_H_ */
