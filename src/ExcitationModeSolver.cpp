/*
* ExcitationModeSolver.cpp
*
* Copyright 2017 Julian Hagemeister
*
* This file is part of MonteCrystal.
*
* MonteCrystal is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MonteCrystal is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with MonteCrystal.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include <Eigen\SparseCore>
#include <spectra\MatOp\SparseGenRealShiftSolve.h>
#include <spectra\GenEigsRealShiftSolver.h>

#include "ExcitationModeSolver.h"

#include "SimulationProgram.h"

#include "SpinOrientation.h"
#include "Hamiltonian.h"
#include "Energy.h"
#include "ExchangeInteraction.h"
#include "DMInteraction.h"
#include "ZeemanEnergy.h"

#include "MyMath.h"
#include "Functions.h"



ExcitationModeSolver::ExcitationModeSolver(SpinOrientation* spinOrientation, 
	QSharedPointer<Hamiltonian> hamilton)
{
	/**
	* All information for calculation of eigenmodes is provided as parameters in constructor.
	*
	* @param[in] spinOrientation The information about the spin configuration.
	* @param[in] hamilton The Hamiltonian providing information about the energy of the system.
	*/

	_spinArray = spinOrientation->get_spin_array();
	_numberAtoms = spinOrientation->get_number_atoms();
	_hamilton = hamilton;
	
	_energies = _hamilton->get_energies();

	_fnExEnergyIndex = -1;
	_fnDMEnergyIndex = -1;
	_AnisotropyIndex = -1;
	_ZeemanEnergyIndex = -1;
}

ExcitationModeSolver::~ExcitationModeSolver()
{
}

void ExcitationModeSolver::save_sparse_matrix(std::string fname)
{
	/**
	* Save sparse matrix to text file.
 	*/

	Functions::save(_tripletList, fname);
}

void ExcitationModeSolver::save_eigenmodes(std::string fname)
{
	/**
	* Save eigenvalues and eigenvectors to text files.
	*/

	Functions::save(_evalues, fname + "Eigenvalues");
	Functions::save(_evalues, _evectors, fname);
}

void ExcitationModeSolver::setup_matrix(void)
{
	/**
	* Setup sparse matrix needed for the calculation of the eigenmodes.
	*/

	find_energy_indexes();

	Threedim ZeemanField{ 0,0,0 };
	if (_ZeemanEnergyIndex > -0.5)
	{
		ZeemanField = static_cast<ZeemanEnergy*>(_energies[_ZeemanEnergyIndex].get())->get_direction();
		ZeemanField = MyMath::mult(ZeemanField, _energies[_ZeemanEnergyIndex]->get_energy_parameter());
	}

	double kAniso = 0;
	if (_AnisotropyIndex > -0.5)
	{
		// CAUTION: minus sign due to different convention for anisotropy energy
		kAniso -= _energies[_AnisotropyIndex]->get_energy_parameter();
	}

	double J1 = 0;
	int fnbors = -1;
	int* fnborArray = NULL;
	if (_fnExEnergyIndex > -0.5)
	{
		J1 -= _energies[_fnExEnergyIndex]->get_energy_parameter();
		fnbors = static_cast<ExchangeInteraction*>(_energies[_fnExEnergyIndex].get())->get_nbors();
		fnborArray = static_cast<ExchangeInteraction*>(_energies[_fnExEnergyIndex].get())->get_neighbor_array();
	}

	double D1 = 0;
	Threedim* dmVectors = NULL;
	if (_fnDMEnergyIndex > -0.5)
	{
		D1 -= _energies[_fnDMEnergyIndex]->get_energy_parameter();
		fnbors = static_cast<DMInteraction*>(_energies[_fnDMEnergyIndex].get())->get_nbors();
		fnborArray = static_cast<DMInteraction*>(_energies[_fnDMEnergyIndex].get())->get_neighbor_array();
		dmVectors = static_cast<DMInteraction*>(_energies[_fnDMEnergyIndex].get())->get_dm_vectors();
	}

	for (int i = 0; i < _numberAtoms; ++i)
	{
		gsl_matrix* matrixI = MyMath::get_rotation_matrix(_spinArray, i);

		double BzTilde = gsl_matrix_get(matrixI, 2, 0) * ZeemanField.x
			+ gsl_matrix_get(matrixI, 2, 1) * ZeemanField.y
			+ gsl_matrix_get(matrixI, 2, 2) * ZeemanField.z;

		double KxxTilde = pow(gsl_matrix_get(matrixI, 0, 2), 2) * kAniso;
		double KzzTilde = pow(gsl_matrix_get(matrixI, 2, 2), 2) * kAniso;
		double KyyTilde = pow(gsl_matrix_get(matrixI, 1, 2), 2) * kAniso;
		double KxyTilde = gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixI, 1, 2) * kAniso;

		double sumNeighborInteraction = 0;
		for (int k = 0; k < fnbors; ++k)
		{
			int neighbor = fnborArray[i*fnbors + k];
			if (neighbor > -0.5)
			{
				gsl_matrix* matrixK = MyMath::get_rotation_matrix(_spinArray, neighbor);

				gsl_matrix* matrixJik = gsl_matrix_calloc(3, 3);

				if (dmVectors != NULL)
				{
					double value = D1*dmVectors[i*fnbors + k].z;
					gsl_matrix_set(matrixJik, 0, 1, value);
					value = -D1*dmVectors[i*fnbors + k].y;
					gsl_matrix_set(matrixJik, 0, 2, value);
					value = D1*dmVectors[i*fnbors + k].x;
					gsl_matrix_set(matrixJik, 1, 2, value);
					value = -D1*dmVectors[i*fnbors + k].z;
					gsl_matrix_set(matrixJik, 1, 0, value);
					value = D1*dmVectors[i*fnbors + k].y;
					gsl_matrix_set(matrixJik, 2, 0, value);
					value = -D1*dmVectors[i*fnbors + k].x;
					gsl_matrix_set(matrixJik, 2, 1, value);
				}

				gsl_matrix_set(matrixJik, 0, 0, J1);
				gsl_matrix_set(matrixJik, 1, 1, J1);
				gsl_matrix_set(matrixJik, 2, 2, J1);

				double JTilde_ik_zz = (gsl_matrix_get(matrixI, 2, 0) * gsl_matrix_get(matrixK, 2, 1) * gsl_matrix_get(matrixJik, 0, 1)
					+ gsl_matrix_get(matrixI, 2, 0) * gsl_matrix_get(matrixK, 2, 2) * gsl_matrix_get(matrixJik, 0, 2)
					+ gsl_matrix_get(matrixI, 2, 1) * gsl_matrix_get(matrixK, 2, 2) * gsl_matrix_get(matrixJik, 1, 2)
					+ gsl_matrix_get(matrixI, 2, 1) * gsl_matrix_get(matrixK, 2, 0) * gsl_matrix_get(matrixJik, 1, 0)
					+ gsl_matrix_get(matrixI, 2, 2) * gsl_matrix_get(matrixK, 2, 0) * gsl_matrix_get(matrixJik, 2, 0)
					+ gsl_matrix_get(matrixI, 2, 2) * gsl_matrix_get(matrixK, 2, 1) * gsl_matrix_get(matrixJik, 2, 1)
					+ gsl_matrix_get(matrixI, 2, 0) * gsl_matrix_get(matrixK, 2, 0) * gsl_matrix_get(matrixJik, 0, 0)
					+ gsl_matrix_get(matrixI, 2, 1) * gsl_matrix_get(matrixK, 2, 1) * gsl_matrix_get(matrixJik, 1, 1)
					+ gsl_matrix_get(matrixI, 2, 2) * gsl_matrix_get(matrixK, 2, 2) * gsl_matrix_get(matrixJik, 2, 2));

				double JTilde_ik_xx = (gsl_matrix_get(matrixI, 0, 0) * gsl_matrix_get(matrixK, 0, 1) * gsl_matrix_get(matrixJik, 0, 1)
					+ gsl_matrix_get(matrixI, 0, 0) * gsl_matrix_get(matrixK, 0, 2) * gsl_matrix_get(matrixJik, 0, 2)
					+ gsl_matrix_get(matrixI, 0, 1) * gsl_matrix_get(matrixK, 0, 2) * gsl_matrix_get(matrixJik, 1, 2)
					+ gsl_matrix_get(matrixI, 0, 1) * gsl_matrix_get(matrixK, 0, 0) * gsl_matrix_get(matrixJik, 1, 0)
					+ gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixK, 0, 0) * gsl_matrix_get(matrixJik, 2, 0)
					+ gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixK, 0, 1) * gsl_matrix_get(matrixJik, 2, 1)
					+ gsl_matrix_get(matrixI, 0, 0) * gsl_matrix_get(matrixK, 0, 0) * gsl_matrix_get(matrixJik, 0, 0)
					+ gsl_matrix_get(matrixI, 0, 1) * gsl_matrix_get(matrixK, 0, 1) * gsl_matrix_get(matrixJik, 1, 1)
					+ gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixK, 0, 2) * gsl_matrix_get(matrixJik, 2, 2));

				double JTilde_ik_yy = (gsl_matrix_get(matrixI, 1, 0) * gsl_matrix_get(matrixK, 1, 1) * gsl_matrix_get(matrixJik, 0, 1)
					+ gsl_matrix_get(matrixI, 1, 0) * gsl_matrix_get(matrixK, 1, 2) * gsl_matrix_get(matrixJik, 0, 2)
					+ gsl_matrix_get(matrixI, 1, 1) * gsl_matrix_get(matrixK, 1, 2) * gsl_matrix_get(matrixJik, 1, 2)
					+ gsl_matrix_get(matrixI, 1, 1) * gsl_matrix_get(matrixK, 1, 0) * gsl_matrix_get(matrixJik, 1, 0)
					+ gsl_matrix_get(matrixI, 1, 2) * gsl_matrix_get(matrixK, 1, 0) * gsl_matrix_get(matrixJik, 2, 0)
					+ gsl_matrix_get(matrixI, 1, 2) * gsl_matrix_get(matrixK, 1, 1) * gsl_matrix_get(matrixJik, 2, 1)
					+ gsl_matrix_get(matrixI, 1, 0) * gsl_matrix_get(matrixK, 1, 0) * gsl_matrix_get(matrixJik, 0, 0)
					+ gsl_matrix_get(matrixI, 1, 1) * gsl_matrix_get(matrixK, 1, 1) * gsl_matrix_get(matrixJik, 1, 1)
					+ gsl_matrix_get(matrixI, 1, 2) * gsl_matrix_get(matrixK, 1, 2) * gsl_matrix_get(matrixJik, 2, 2));

				double JTilde_ik_xy = (gsl_matrix_get(matrixI, 0, 0) * gsl_matrix_get(matrixK, 1, 1) * gsl_matrix_get(matrixJik, 0, 1)
					+ gsl_matrix_get(matrixI, 0, 0) * gsl_matrix_get(matrixK, 1, 2) * gsl_matrix_get(matrixJik, 0, 2)
					+ gsl_matrix_get(matrixI, 0, 1) * gsl_matrix_get(matrixK, 1, 2) * gsl_matrix_get(matrixJik, 1, 2)
					+ gsl_matrix_get(matrixI, 0, 1) * gsl_matrix_get(matrixK, 1, 0) * gsl_matrix_get(matrixJik, 1, 0)
					+ gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixK, 1, 0) * gsl_matrix_get(matrixJik, 2, 0)
					+ gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixK, 1, 1) * gsl_matrix_get(matrixJik, 2, 1)
					+ gsl_matrix_get(matrixI, 0, 0) * gsl_matrix_get(matrixK, 1, 0) * gsl_matrix_get(matrixJik, 0, 0)
					+ gsl_matrix_get(matrixI, 0, 1) * gsl_matrix_get(matrixK, 1, 1) * gsl_matrix_get(matrixJik, 1, 1)
					+ gsl_matrix_get(matrixI, 0, 2) * gsl_matrix_get(matrixK, 1, 2) * gsl_matrix_get(matrixJik, 2, 2));

				sumNeighborInteraction += JTilde_ik_zz;

				// fill off-diagonal elements:
				//element of matrix A
				add_sparse_matrix_element(i, _numberAtoms + neighbor, JTilde_ik_xx);
				//element of matrix C
				add_sparse_matrix_element(_numberAtoms + i, neighbor, -JTilde_ik_yy);
				// element of matrix B
				add_sparse_matrix_element(i, neighbor, JTilde_ik_xy);
				add_sparse_matrix_element(_numberAtoms + neighbor, _numberAtoms + i, -JTilde_ik_xy);

				gsl_matrix_free(matrixK);
				gsl_matrix_free(matrixJik);
			}
		}

		// fill diagonal elements:
		// element of matrix A
		double a = -sumNeighborInteraction + 2 * KxxTilde - 2 * KzzTilde + BzTilde;
		add_sparse_matrix_element(i, _numberAtoms + i, a);
		// element of matrix C
		double c = -sumNeighborInteraction + 2 * KyyTilde - 2 * KzzTilde + BzTilde;
		add_sparse_matrix_element(_numberAtoms + i, i, -c);
		// element of matrix B
		double b = 2 * KxyTilde;
		add_sparse_matrix_element(i, i, b);
		add_sparse_matrix_element(_numberAtoms + i, _numberAtoms + i, -b);

		gsl_matrix_free(matrixI);
	}
}

void ExcitationModeSolver::diagonalize(int numberEigenstates)
{
	/**
	* Diagonalization of sparse matrix to obtain eigenvalues and eigenvectors of eigenmodes.
	*
	* @param[in] numberEigenstates The number of eigenstates to be calculated.
	*/

	int n = 2 * _numberAtoms;
	Eigen::SparseMatrix<double> m(n, n);
	m.setFromTriplets(_tripletList.begin(), _tripletList.end());

	// Construct matrix operation object using the wrapper class
	Spectra::SparseGenRealShiftSolve<double> op(m);
	// Construct eigen solver object with shift 0
	// This will find eigenvalues that are closest to 0
	int nev = 2*numberEigenstates; // number of eigenvalues
	int ncv = 10 * nev; // convergence speed. should be ncv>=2*nev+1 according to documentation
	Spectra::GenEigsRealShiftSolver< double, Spectra::LARGEST_MAGN, Spectra::SparseGenRealShiftSolve<double> > eigs(&op, nev, ncv, 0.0);
	eigs.init();
	eigs.compute();
	if (eigs.info() == Spectra::SUCCESSFUL)
	{
		_evalues = eigs.eigenvalues();
		_evectors = eigs.eigenvectors();
	}
}


void ExcitationModeSolver::find_energy_indexes(void)
{
	/**
	* The eigenmode solver only works for selected energy contributions. 
	* These are currently NEAREST NEIGHBOR DM and exchange energy and anisotropy and Zeeman energy.
	* The code will fail if higher order DM and exchange energies were defined!
	*/

	for (int i = 0; i < _energies.size(); ++i)
	{
		if (_energies[i]->get_string_id().compare(0,3, "E_D") == 0)
		{
			_fnDMEnergyIndex = i;
		}
		if (_energies[i]->get_string_id().compare(0, 3, "E_J") == 0)
		{
			_fnExEnergyIndex = i;
		}
		if (_energies[i]->get_string_id().compare(0, 7, "E_Uniax") == 0)
		{
			_AnisotropyIndex = i;
		}
		if (_energies[i]->get_string_id().compare(0, 8, "E_Zeeman") == 0)
		{
			_ZeemanEnergyIndex = i;
		}
	}
}

void ExcitationModeSolver::add_sparse_matrix_element(int i, int j, double value)
{
	_tripletList.push_back(Eigen::Triplet<double>(i, j, value));
}