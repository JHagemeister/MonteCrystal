/*
* Experiment01Window.h
*
*      
*
* This class defines a window that can be used to specify parameters for a special simulation experiment. It
* is intended that a two-dimensional lattice within the (x,y)-plane is used. The spins at the lattice sites
* with minimum x-component will be rotated with a certain frequency. 
*
*/


#ifndef EXPERIMENT01WINDOW_H
#define EXPERIMENT01WINDOW_H

#include <QDialog>
#include "ui_experiment01.h"

/// Window for parameter specification for Experiment01 simulation.
/**
* This class defines a window that can be used to specify parameters for a special simulation experiment.It
* is intended that a two - dimensional lattice within the(x, y) - plane is used.The spins at the lattice sites
* with minimum x - component will be rotated with a certain frequency.
*/
class Experiment01Window : public QDialog
{
	Q_OBJECT

public:
	Experiment01Window(QWidget *parent = 0);
	~Experiment01Window();

	Experiment01Struct read_parameters(void); ///< read user specified parameters 

private:
	Ui::Experiment01 _ui;
};

#endif // EXPERIMENT01WINDOW_H
