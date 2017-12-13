/*
* Functions.cpp
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

#include "Functions.h"

#include <gsl/gsl_complex.h>

#include <qstring.h>

#include <iomanip>

Functions::Functions()
{
}

Functions::~Functions()
{
}

std::string Functions::time_stamp(void)
{
	/**
	* This function returns a std::string containing the current time and date.
	*
	* @return std::string containing the time and date.
	*/

	time_t result = time(NULL);
	char str[26];
	ctime_s(str, sizeof str, &result);
	std::string tmpString(str);
	std::stringstream ss;
	ss.str(tmpString);
	std::string item;
	std::getline(ss, item);
	return item;
}

void Functions::get_spin_model_params(SpinMeshParams &params, std::string fname)
{
	/**
	* Read parameters defining size and shape of arrow model for spin display in graphical interface 
	* from previous session. If no file with specified file name exists, the file will be created.
	*
	* @param[in] params Parameters defining size and shape of arrow model
	* @param[in] fname File name containing the parameters of arrow model
	*/

	std::fstream sstream;
	std::string _parameter;
	char _cstring[256];
	char* _spiece;
	char separators[] = " ";
	char* next_token1 = NULL;
	sstream.open(fname, std::fstream::in);
	if (!sstream.good())
	{
		std::fstream newstream;
		newstream.open(fname, std::fstream::out);
		newstream << "n " << params.n << " #" << std::endl;
		newstream << "r2divr1 " << params.r2divr1 << " #" << std::endl;
		newstream << "l1divl2 " << params.l1divl2 << " #" << std::endl;
		newstream << "r2divl2 " << params.r2divl2 << " #" << std::endl;
		newstream << "scale " << params.scale << " #" << std::endl;
		newstream.close();
	}
	else
	{
		while (!sstream.eof())
		{
			sstream.getline(_cstring, sizeof(_cstring));
			_parameter = _cstring;
			/*
			* look for comments and empty rows
			*/
			if (int(_parameter.find("//")) == 0 || int(_parameter.find("*")) == 0
				|| int(_parameter.find("#")) == 0)
			{
			}
			else if (int(_parameter.find(" ")) == -1)
			{ //if no blank spaces occur its empty
			}
			else
			{
				_spiece = strtok_s(_cstring, separators, &next_token1);
				if (strcmp(_spiece, "n") == 0)
				{
					_spiece = strtok_s(NULL, separators, &next_token1);
					_parameter = _spiece;
					params.n = atoi(_spiece);
				}
				if (strcmp(_spiece, "r2divr1") == 0)
				{
					_spiece = strtok_s(NULL, separators, &next_token1);
					_parameter = _spiece;
					params.r2divr1 = atof(_spiece);
				}
				if (strcmp(_spiece, "l1divl2") == 0)
				{
					_spiece = strtok_s(NULL, separators, &next_token1);
					_parameter = _spiece;
					params.l1divl2 = atof(_spiece);
				}
				if (strcmp(_spiece, "r2divl2") == 0)
				{
					_spiece = strtok_s(NULL, separators, &next_token1);
					_parameter = _spiece;
					params.r2divl2 = atof(_spiece);
				}
				if (strcmp(_spiece, "scale") == 0)
				{
					_spiece = strtok_s(NULL, separators, &next_token1);
					_parameter = _spiece;
					params.scale = atof(_spiece);
				}
			}
		}
	}
}

void Functions::save_spin_model_params(SpinMeshParams params, std::string fname)
{
	/**
	* Save arrow model parameters for spin visualization in graphical interface.
	*
	* @param[in] params Parameters defining arrow model.
	* @param[in] fname Name of file to save parameters in.
	*/

	std::fstream sstream;
	sstream.open(fname, std::fstream::out);
	sstream << "n " << params.n << " #" << std::endl;
	sstream << "r2divr1 " << params.r2divr1 << " #" << std::endl;
	sstream << "l1divl2 " << params.l1divl2 << " #" << std::endl;
	sstream << "r2divl2 " << params.r2divl2 << " #" << std::endl;
	sstream << "scale " << params.scale << " #" << std::endl;
	sstream.close();
}

std::string Functions::get_id(std::string workfolder)
{
	/**
	* get unique id using the helper file "simulation_number" in working folder. If no such file exist, the
	* id will be 1 and a corresponding "simulation_number" file is created
	*
	* @param[in] workfolder Working directory
	* @return string containing unique simulation identity number
	*/

	std::string fname = workfolder + "simulation_number";

	int id = 1;
	std::string idStr = "";

	std::fstream sstream;
	std::string _parameter;
	char _cstring[256];
	char* _spiece;
	char separators[] = " ";
	char* next_token1 = NULL;
	sstream.open(fname, std::fstream::in);
	if (!sstream.good())
	{
		std::fstream newstream;
		newstream.open(fname, std::fstream::out);
		idStr.append("0001");
		newstream << "ID " << idStr << " #" << std::endl;
		newstream.close();
	}
	else
	{
		while (!sstream.eof())
		{
			sstream.getline(_cstring, sizeof(_cstring));
			_parameter = _cstring;
			/*
			* look for comments and empty rows
			*/
			if (int(_parameter.find("//")) == 0 || int(_parameter.find("*")) == 0
				|| int(_parameter.find("#")) == 0)
			{
			}
			else if (int(_parameter.find(" ")) == -1)
			{ //if no blank spaces occur its empty
			}
			else
			{
				_spiece = strtok_s(_cstring, separators, &next_token1);
				if (strcmp(_spiece, "ID") == 0)
				{
					_spiece = strtok_s(NULL, separators, &next_token1);
					_parameter = _spiece;
					id = atoi(_spiece);
					++id;
				}
			}
		}
		sstream.clear();
		sstream.close();
		sstream.open(fname, std::fstream::out);
		std::stringstream stream;
		stream << id;
		if (id < 10)
		{
			idStr = "000";
			idStr.append(stream.str());
		}
		else
		{
			if (id < 100)
			{
				idStr = "00";
				idStr.append(stream.str());
			}
			else
			{
				if (id < 1000)
				{
					idStr = "0";
					idStr.append(stream.str());
				}
				else
				{
					idStr.append(stream.str());
				}
			}
		}
		sstream << "ID " << idStr << " #" << std::endl;
	}

	sstream.close();
	return idStr;
}

std::string Functions::folder_name(const Configuration* config)
{
	/**
	* Create a distinct folder name from parameters specified in config. 
	*
	* @param[in] config Configuration parameters
	* @return string with a distinct folder name
	*/

	std::stringstream stream;
	std::stringstream tmpStream;

	stream << "_";

	switch (config->_simulationType)
	{
	case landauLifshitzGilbert:
		stream  << "LLG";
		break;
	case metropolis:
		stream << "MC";
		break;
	}

	/*if (config->_latticePath.empty())
	{
		switch (config->_latticeType)
		{
		case simpleCubic:
			stream << "simple_cubic";
			break;
		case bodyCenteredCubic:
			stream << "body_centered_cubic";
			break;
		case faceCenteredCubic:
			stream << "face_centered_cubic";
			break;

		case triangularHexagonal:
			if (config->_boundaryConditions == helical) {
				stream << "2D_triangular_hexagonal_helical";
			}
			else {
				stream << "2D_triangular_hexagonal_open";
			}
			break;
		case triangularTriangular:
			stream << "2D_triangular_triangular";
			break;
		case triangularHalfDisk:
			stream << "2D_triangular_half_disk";
			break;
		case triangularDisk:
			stream << "2D_triangular_disk";
			break;
		case triangularArrowHead:
			stream << "2D_triangular_arrow_head";
			break;
		case triangularStripe:
			stream << "2D_triangular_stripe";
			break;
		}
	}
	else
	{
		stream << "lattice_read_in";
	}*/

	return stream.str();
}

std::string Functions::get_name(double value)
{
	/**
	* Transform value into a specific form of string representation. Needed to get a nice representation of
	* magnetic field and temperature in file names for example.
	* e.g. T = 0.1 -> "00.1" or B = -1.4 -> "-01.4" results in better ordering of files
	*
	* @param[in] value Value to get string representation of
	* @return string representation of value
	*/

	std::stringstream stream;
	if (fabs(value) < 10)
	{
		if (value >= 0)
		{
			stream << "0" << value;
		}
		else
		{
			stream << "-0" << -value;
		}
	}
	else
	{
		stream << value;
	}
	return stream.str();
}

std::string Functions::get_three_digit_name(int value)
{
	/**
	* See get_name(double). This function expects a positive value.
	*
	* @param[in] value A positive integer value.
	*/

	std::stringstream stream;
	if (value < 10)
	{
		stream << "00" << value;
	}
	else if (value < 100)
	{
		stream << "0" << value;
	}
	else
	{
		stream << value;
	}
	return stream.str();
}

void Functions::save(Threedim* array, int size, std::string fname)
{
	/**
	* save values of array to file; first column provides line index
	*
	* @param[in] array array with three dimensional vectors
	* @param[in] size number of vectors
	* @param[in] fname file name for output
	*/

	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	for (int i = 0; i < size; ++i)
	{
		filestr << i << " " << array[i].x << " " << array[i].y << " " << array[i].z;
		if (i < size - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(int * array1, Threedim * array2, int size, std::string fname)
{
	/**
	* save values of two arrays to file
	*
	* @param[in] array1 array with integer values
	* @param[in] array2 array with three dimensional vectors
	* @param[in] size number of vectors
	* @param[in] fname file name for output
	*/

	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << std::setprecision(15);
	for (int i = 0; i < size; ++i)
	{
		filestr << array1[i] << " " << array2[i].x << " " << array2[i].y << " " << array2[i].z;
		if (i < size - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(TopologicalChargeCell* array, int size, std::string fname)
{
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	for (int idx = 0; idx < size; ++idx)
	{
		filestr << idx << " " << array[idx].i << " " << array[idx].j << " " << array[idx].k;
		if (idx < size - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(Threedim* array1, Threedim* array2, int size, std::string fname)
{
	/**
	* save two arrays to one file; first column line index
	*
	* @param[in] array1 array with three dimensional vectors
	* @param[in] array2 array with three dimensional vectors
	* @param[in] size number of vectors in each array
	* @param[in] fname file name for output
	*/

	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	for (int i = 0; i < size - 1; ++i)
	{
		filestr << i << " " << array1[i].x << " " << array1[i].y << " " << array1[i].z << " "
			<< array2[i].x << " " << array2[i].y << " " << array2[i].z;
		if (i < size - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(int* array, int size, std::string fname)
{
	/**
	* save values of array to file; first column provides line index
	*
	* @param[in] array array with integer values
	* @param[in] size number of integer values
	* @param[in] fname file name for output
	*/

	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	for (int i = 0; i < size - 1; ++i)
	{
		filestr << i << " " << array[i];
		if (i < size - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(std::string fname, std::string body)
{
	/**
	* save string to file
	*
	* @param[in] fname file name for output
	* @param[in] body string to be stored in file
	*/

	std::fstream filestr;
	filestr.open(fname, std::fstream::app);
	filestr << body;
	filestr.close();
}

void Functions::save(gsl_vector_complex* eval, int size, std::string fname)
{
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << "real imag" << std::endl;
	for (int i = 0; i < size; i++)
	{
		gsl_complex eval_i = gsl_vector_complex_get(eval, i);

		filestr << GSL_REAL(eval_i) << " " << GSL_IMAG(eval_i);

		if (i < size - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(gsl_matrix_complex* evec, int rows, int columns, std::string fname)
{
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << "real imag" << std::endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; ++j)
		{
			gsl_vector_complex_view evec_j = gsl_matrix_complex_column(evec, j);
			gsl_complex z = gsl_vector_complex_get(&evec_j.vector, i);

			if (j > 0)
			{
				filestr << " ";
			}
			filestr << GSL_REAL(z) << " " << GSL_IMAG(z);
		}
		if (i < rows - 1)
		{
			filestr << std::endl;
		}
	}
	filestr.close();
}

void Functions::save(std::vector<Eigen::Triplet<double>> &sparseMatrix, std::string fname)
{
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << std::setprecision(15);
	for (int i = 0; i < sparseMatrix.size(); i++)
	{
		filestr << sparseMatrix[i].row() << " " << sparseMatrix[i].col() << " " << sparseMatrix[i].value() << std::endl;
	}
	filestr.close();
}

void Functions::save(Eigen::VectorXcd & evalues, Eigen::MatrixXcd & evectors, std::string path)
{
	std::fstream filestr;
	filestr.open(path+"RedEvals", std::fstream::out);
	for (int i = 0; i < evalues.rows(); ++i)
	{
		if (evalues[i].imag() > 0)
		{
			filestr << evalues[i].real() << " " << evalues[i].imag() << " " << std::endl;
		}
	}
	filestr.close();

	filestr.open(path+"RedEvecs", std::fstream::out);
	for (int j = 0; j < evectors.rows(); ++j)
	{
		for (int i = 0; i < evalues.rows(); ++i)
		{
			if (evalues[i].imag() > 0)
			{
				filestr << evectors(j,i).real() << " " << evectors(j, i).imag() << " ";
			}
		}
		filestr << std::endl;
	}
	filestr.close();
}

void Functions::save(Eigen::VectorXcd & evalues, std::string fname)
{
	std::fstream filestr;
	filestr.open(fname, std::fstream::out);
	filestr << std::setprecision(15);
	for (int i = 0; i < evalues.rows(); ++i)
	{
		filestr << evalues[i] << std::endl;
	}
	filestr.close();
}

int Functions::read_eigenvalues(std::string fname, std::vector<double> &eigenvalues)
{
	/**
	* Read eigenvalues from a text file. The text file is expected to contain two columns.
	* First column contains real part and second column imaginary part of eigenvalues.
	*
	* @param[in] fname Name of file containing eigenvalues.
	* @param[out] eigenvalues The imaginary parts of the eigenvalues. 
	*/

	// open file
	std::ifstream infile(fname);
	std::string line;
	int numberRows = Functions::get_num_lines(fname);
	// temporary helper value for read in
	std::string parameter;

	if (!infile)
	{
		return 0;
	}

	eigenvalues.clear();
	int numberEigenvalues = 0;
	while (std::getline(infile, line))
	{
		if (line.find("\n") != 0)
		{
			std::istringstream isstream(line);
			parameter.clear();
			isstream >> parameter;
			parameter.clear();
			isstream >> parameter;

			eigenvalues.push_back(std::stof(parameter));
			numberEigenvalues += 1;
		}
	}
	// close file
	infile.close();
	
	return numberEigenvalues;
}

gsl_matrix_complex* Functions::read_eigenvectors(std::string fname)
{
	/**
	* Read eigenvectors from text file. The first two columns shall be the real and imaginary
	* parts of the values of the first eigenvector and so on with the next columns. This means 
	* that the total number of columns shall be an even number.
	*
	* @param[in] fname The name of the file containing the eigenvectors.
	*/

	// open file
	std::ifstream infile(fname);
	std::string line;

	if (!infile)
	{
		return NULL;
	}

	int numberRows = Functions::get_num_lines(fname);
	int numberEvectors = 0;
	gsl_matrix_complex* evecs = NULL;

	int count = 0; // index of currently processed line of file
	// temporary helper value for read in
	std::string real;
	std::string imag;

	while (std::getline(infile, line))
	{
		if (count == 0)
		{
			QString qline = QString::fromStdString(line);
			numberEvectors = qline.count(" ")/2;
			evecs = gsl_matrix_complex_alloc(numberRows, numberEvectors);
		}
		std::istringstream isstream(line);

		for (int i = 0; i < numberEvectors; ++i)
		{

			real.clear();
			isstream >> real;
			imag.clear();
			isstream >> imag;

			gsl_complex value;
			GSL_SET_COMPLEX(&value, std::stof(real), std::stof(imag));
			gsl_matrix_complex_set(evecs, count, i, value);
		}
		++count;
	}
	// close file
	infile.close();
	
	return evecs;
}

void Functions::write_README(std::string workfolder, std::string simFolder, const Configuration* config)
{
	/**
	* write new entry to README file; contains time stamp; simulation folder name; simulation parameters
	*
	* @param[in] workfolder working folder
	* @param[in] simFolder name of simulation folder
	* @param[in] config simulation parameters
	*/

	std::string fname = workfolder + "README";
	std::fstream filestr;
	filestr.open(fname, std::fstream::in);
	if (!filestr.good())
	{
		filestr.close();
		filestr.open(fname, std::fstream::out);
		filestr << "--------- README file for MonteCrystal ---------" << std::endl << std::endl;
		filestr.close();
	}
	else {
		filestr.close();
	}
	filestr.open(fname, std::fstream::out | std::fstream::app);
	filestr << "SIMULATION on: " << time_stamp();
	filestr << "    Folder Name: " << simFolder << std::endl;
	filestr << "Configuration Parameters: " << std::endl << config->_allParameters << std::endl << std::endl;
	filestr.close();
}

int Functions::get_num_lines(std::string fname)
{
	/**
	* Obtain number of lines of a text file
	*
	* @param[in] fname file name 
	*/

	int number_of_lines = 0;
	std::string line;
	std::ifstream myfile(fname);

	while (std::getline(myfile, line))
	{
		++number_of_lines;
	}

	std::cout << std::endl << "Number of lines in file " << fname << ": " << number_of_lines << std::endl;
	return number_of_lines;
}