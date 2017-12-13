/*
* Functions.h
*
*  Created on: 10.01.2013
*      
*/

#include <Eigen\Eigen>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <string>
#include <sstream>
#include <memory>
#include <time.h>  

#include "Configuration.h"
#include "typedefs.h"

#include <gsl/gsl_matrix.h>

/// Methods for output of simulation information and results

class Functions
{
public:
	Functions();
	virtual ~Functions();
	static std::string time_stamp(void);
	static void get_spin_model_params(SpinMeshParams &params, std::string fname);
	static void save_spin_model_params(SpinMeshParams params, std::string fname);
	static std::string get_id(std::string workfolder);
	static std::string folder_name(const Configuration* config);
	static std::string get_name(double value);
	static std::string get_three_digit_name(int value);
	
	static void save(Threedim* array, int size, std::string fname);
	static void save(int* array1, Threedim* array2, int size, std::string fname);
	static void save(TopologicalChargeCell* array, int size, std::string fname);
	static void save(Threedim* array1, Threedim* array2, int size, std::string fname);
	static void save(int* array, int size, std::string fname);
	static void save(std::string fname, std::string body);
	static void save(gsl_vector_complex* eval, int size, std::string fname);
	static void save(gsl_matrix_complex* evec, int rows, int columns, std::string fname);
	static void save(std::vector<Eigen::Triplet<double>> &sparseMatrix, std::string fname);
	static void save(Eigen::VectorXcd &evalues, Eigen::MatrixXcd &evectors, std::string path);
	static void save(Eigen::VectorXcd &evalues, std::string fname);

	static int read_eigenvalues(std::string fname, std::vector<double> &eigenvalues);
	static gsl_matrix_complex* read_eigenvectors(std::string fname);

	static void write_README(std::string workfolder, std::string simFolder, const Configuration* config);
	static int get_num_lines(std::string fname);
};

#endif /* FUNCTIONS_H_ */
