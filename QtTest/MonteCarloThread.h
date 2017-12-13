/*
* Model.h
*
*  Created on: 31.10.2013
*      Author: Julian Hagemeister
*/

#ifndef MONTECARLOTHREAD_H
#define MONTECARLOTHREAD_H

#include "Experiment.h"

#include <qthread.h>

class MonteCarloThread : public QThread 
{
Q_OBJECT
public:
	MonteCarloThread();
	void set_experiment(std::shared_ptr<Experiment> experiment);
protected:
	void run();
private:
	std::shared_ptr<Experiment> _experiment;
};

#endif // MONTECARLOTHREAD_H
