// MonteCrystal.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "Experiment.h"

int main(int argc, char **argv)
{
	string workfolder = "C:/Users/Julian/Documents/TestOutput/";  // folder with config file and DATA folder
	auto config = std::make_shared<Configuration>(workfolder+"Config");
	config->read_parameters(); //read the configuration parameters
	config->show_parameters(); //show configuration parameters
	Experiment* experiment = new Experiment(workfolder, config);
	experiment->main_experiment();

    return 0;
}