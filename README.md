# MonteCrystal

MonteCrystal is a cross platform simulation tool for magnetic materials with a Qt based 
graphical user interface.

The following simulation algorithms are included:

1) Metropolis Monte Carlo
2) thermal Landau Lifshitz Gilbert equation spin dynamics
3) excitation mode calculation by diagonalization

All are based on an atomistic classical spin model. The user can choose between different 
lattice structures or create a lattice structure and load it from a text file. Each lattice
site is populated by a classical Heisenberg Spin - O(3) model. The user can specify various
common interactions between the spins as Heisenber exchange and the Dzyaloshinskii-Moriya
interaction.


I have tested the compilation of the source code on a Windows 10 machine and Ubuntu 16.04 
running in VirtualBox.

Installation guide:

- Install Qt - 5.10  (I have tested with that one)
- download and build gsl (the program needs to be linked against the library)
- donload and build Eigen (the program needs to be linked against this library, too)
- download spectra (only header files needed for include)
- download CImg (only header files needed for include)
- download glm (only header files needed for include)

donwload the MonteCrystal project from github and run cmake on it. Make sure that the Qt
installation was found. Adjust the include paths and library paths for your system setup,
so that the above mentioned third party parts can be found.

After compilation: 
Copy the lighting.vs and lighting.frag files into the program folder. 
On a windows machine: Copy from Qt the platforms folder into the program folder.


If you have any questions about the software or the compilation of it, I will try my best 
to answer any questions posted on the github page.

I hope you enjoy the software!
