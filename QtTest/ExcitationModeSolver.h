#include <Eigen\Eigen>


#ifndef EXCITATIONMODESOLVER_H_
#define EXCITATIONMODESOLVER_H_

#include <qsharedpointer.h>
#include <memory>

#include "typedefs.h"

class SimulationProgram;

class SpinOrientation;
class Hamiltonian;
class Energy;

class ExcitationModeSolver 
{
public:
	ExcitationModeSolver(SpinOrientation* spinOrientation, QSharedPointer<Hamiltonian> hamilton);
	virtual ~ExcitationModeSolver(void);
	void setup_matrix(void);
	void diagonalize(int numberEigenstates);

	void save_sparse_matrix(std::string fname);
	void save_eigenmodes(std::string fname);

private:
	void find_energy_indexes(void);

	void add_sparse_matrix_element(int i, int j, double value);

	std::vector<Eigen::Triplet<double>> _tripletList; ///< elements of sparse matrix

	Eigen::MatrixXcd _evectors;
	Eigen::VectorXcd _evalues;

	Threedim* _spinArray;
	int _numberAtoms;
	QSharedPointer<Hamiltonian> _hamilton;
	std::vector<std::shared_ptr<Energy>> _energies; ///< energy objects	
	int _fnExEnergyIndex; ///< index identifying first neighbor exchange energy object in _energies
	int _fnDMEnergyIndex; ///< index identifying first neighbor DM energy object in _energies
	int _AnisotropyIndex; ///< index identifying anisotropy energy object in _energies
	int _ZeemanEnergyIndex; ///< index identifying Zeeman energy object in _energies
};

#endif // EXCITATIONMODESOLVER
