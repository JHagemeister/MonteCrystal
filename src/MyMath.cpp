/*
* MyMath.cpp
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

#include "MyMath.h"

#include <iostream>

MyMath::MyMath()
{
}

MyMath::~MyMath()
{
}

int MyMath::signum(double x)
{
	/**
	* Determine sign of a value.
	*
	* @param[in] x Value for which the sign is to be determined
	* @return -1 for x < 0; +1 for x > 0; 0 if x == 0
	*/

	if (x > 0)
	{
		return 1;
	}
	if (x < 0)
	{
		return -1;
	}
	return 0;
}

double MyMath::round(double value, int digits)
{
	/**
	* Determine value with reduced number of digits.
	*
	* @param[in] value shall be reduced to digits number of digits
	* @param[in] digits number of digits
	* @return value with reduced number of digits
	*/

	double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 }; // mgl. verlängern
	return floor(value * v[digits] + 0.5) / v[digits];
}

double MyMath::norm(const Threedim &vec)
{
	/**
	* Calculate norm of a vector.
	*
	* @param[in] vec three component vector
	*
	*@return vector norm
	*/

	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Threedim MyMath::normalize(const Threedim &vec)
{
	/**
	* Normalize a vector.
	*
	* @param[in] vec three component vector
	*
	*@return normalized vector
	*/

	Threedim normVec;
	double normValue = norm(vec);
	if (normValue < PRECISION)
	{
		std::cout << "Caution: Attempt to normalize vector with norm zero. Nothing done." << std::endl;
		normValue = 1;
	}
	normVec.x = vec.x / normValue;
	normVec.y = vec.y / normValue;
	normVec.z = vec.z / normValue;
	return normVec;
}

Threedim MyMath::add(const Threedim &vec1, const Threedim &vec2)
{
	/**
	* Add two vectors.
	*
	* @param[in] vec1 three component vector
	* @param[in] vec2 three component vector
	*
	*@return result of vector addition
	*/

	Threedim vec = {0,0,0};
	vec.x = vec1.x + vec2.x;
	vec.y = vec1.y + vec2.y;
	vec.z = vec1.z + vec2.z;
	return vec;
}

Threedim MyMath::mult(const Threedim &vec, const double &mult)
{
	/**
	* Multiplication of a vector with a scalar.
	*
	* @param[in] vec three component vector
	* @param[in] mult scalar value for multiplication
	*
	*@return vector multiplied with scalar
	*/

	Threedim vector = {0,0,0};
	vector.x = vec.x * mult;
	vector.y = vec.y * mult;
	vector.z = vec.z * mult;
	return vector;
}

double MyMath::dot_product(const Threedim &vec1, const Threedim &vec2)
{
	/**
	* Calculate the dot/scalar product of two vectors.
	*
	* @param[in] vec1 three component vector
	* @param[in] vec2 three component vector
	*
	*@return scalar product
	*/

	double vecproduct = 0;
	vecproduct = (vec1.x) * (vec2.x) + (vec1.y) * (vec2.y) + (vec1.z) * (vec2.z);
	return vecproduct;
}

Threedim MyMath::vector_product(const Threedim &vec1, const Threedim &vec2)
{
	/**
	* Calculate the vector product of two vectors.
	*
	* @param[in] vec1 three component vector
	* @param[in] vec2 three component vector
	*
	*@return vector product
	*/

	Threedim prod = {0,0,0};
	prod.x = vec1.y * vec2.z - vec1.z * vec2.y;
	prod.y = vec1.z * vec2.x - vec1.x * vec2.z;
	prod.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return prod;
}

Threedim MyMath::difference(const Threedim &vec1, const Threedim &vec2)
{
	/**
	* Calculate the vector difference vec1 - vec2.
	*
	* @param[in] vec1 three component vector
	* @param[in] vec2 three component vector
	*
	*@return vector difference
	*/

	Threedim diff = {0,0,0};
	diff.x = vec1.x - vec2.x;
	diff.y = vec1.y - vec2.y;
	diff.z = vec1.z - vec2.z;
	return diff;
}

double MyMath::cosine_vectors(const Threedim &vec1, const Threedim &vec2)
{
	/**
	* Calculate the cosine of the angle between two vectos.
	*
	* @param[in] vec1 three component vector
	* @param[in] vec2 three component vector
	*
	*@return cosine of angle between vectors
	*/

	return dot_product(vec1, vec2) / (norm(vec1) * norm(vec2));
}

Threedim MyMath::matrix_vector_product(gsl_matrix* matrix, const Threedim &vec)
{
	/**
	* matrix * vector.
	*
	* param[in] matrix
	* param[in] vec
	* @return matrix*vector
	*/
	Threedim result{ 0,0,0 };
	result.x = gsl_matrix_get(matrix, 0, 0)*vec.x + gsl_matrix_get(matrix, 0, 1)*vec.y 
		+ gsl_matrix_get(matrix, 0, 2)*vec.z;
	result.y = gsl_matrix_get(matrix, 1, 0)*vec.x + gsl_matrix_get(matrix, 1, 1)*vec.y
		+ gsl_matrix_get(matrix, 1, 2)*vec.z;
	result.z = gsl_matrix_get(matrix, 2, 0)*vec.x + gsl_matrix_get(matrix, 2, 1)*vec.y
		+ gsl_matrix_get(matrix, 2, 2)*vec.z;
	return result;
}

gsl_matrix * MyMath::get_rotation_matrix(Threedim * array1, const int &index)
{
	/**
	* Get rotation matrix for transformation to coordinate system in which
	* z-direction is parallel to vector(/spin). Needed for calculation and display 
	* of eigenmodes.
	*/

	gsl_matrix* matrix = gsl_matrix_calloc(3, 3);
	gsl_matrix_set(matrix, 0, 0, 1);
	gsl_matrix_set(matrix, 1, 1, 1);
	gsl_matrix_set(matrix, 2, 2, 1);

	Threedim ez{0,0,1};
	Threedim rotDir = MyMath::vector_product(array1[index], ez);
	double sinT = MyMath::norm(rotDir);
	if (sinT > 0.000000000001)
	{
		rotDir = MyMath::normalize(rotDir);
		double cosT = MyMath::dot_product(array1[index], ez);

		double omc = (1 - cosT);

		double value = pow(rotDir.x, 2)*omc + cosT;
		gsl_matrix_set(matrix, 0, 0,  value);

		value = rotDir.x*rotDir.y*omc - rotDir.z*sinT;
		gsl_matrix_set(matrix, 0, 1, value);

		value = rotDir.x*rotDir.z*omc + rotDir.y*sinT;
		gsl_matrix_set(matrix, 0, 2, value);

		value = rotDir.y*rotDir.x*omc + rotDir.z*sinT;
		gsl_matrix_set(matrix, 1, 0, value);

		value = pow(rotDir.y,2)*omc + cosT;
		gsl_matrix_set(matrix, 1, 1, value);

		value = rotDir.y*rotDir.z*omc - rotDir.x*sinT;
		gsl_matrix_set(matrix, 1, 2, value);

		value = rotDir.z*rotDir.x*omc - rotDir.y*sinT;
		gsl_matrix_set(matrix, 2, 0, value);

		value = rotDir.z*rotDir.y*omc + rotDir.x*sinT;
		gsl_matrix_set(matrix, 2, 1, value);

		value = pow(rotDir.z,2)*omc + cosT;
		gsl_matrix_set(matrix, 2, 2, value);
	}

	return matrix;
}

Twodim MyMath::two_point_equation(const double &x1, const double &y1, const double &x2, const double &y2)
{
	/**
	* Determines the slope and the offset of a linear function through points
	* (x1,y1) and (x2,y2)
	*
	* @param[in] x1 x-value of first point
	* @param[in] y1 y-value of first point
	* @param[in] x2 x-value of second point
	* @param[in] y2 y-value of second point
	*/

	Twodim solution = {0,0};
	solution.x = (y2 - y1) / (x2 - x1);
	solution.y = y1 - solution.x * x1;
	return solution;
}

double MyMath::gauss(const double &sigma, const double &mu, const double &x)
{
	/**
	* Gauss function with expectation value mu and variance sigma**2
	*
	* @param[in] sigma standard deviation
	* @param[in] mu expectation value
	* @param[in] x variable value
	*
	*@return value of gauss curve at x
	*/

	double f = 1 / (sigma * sqrt(2 * Pi));
	double s = (x - mu) / sigma;
	return f * exp(-0.5 * pow(s, 2));
}

double MyMath::mean_value(double* values, const int &num)
{
	/**
	* Evaluate mean value.
	*
	* @param[in] values array with values
	* @param[in] num array length
	*
	*@return mean value
	*/

	double mean = 0;
	for (int i = 0; i < num; ++i)
	{
		mean += values[i];
	}
	return mean / num;
}

double MyMath::variance(double* values, const int &num)
{
	/**
	* Evaluate variance.
	*
	* @param[in] values array with values
	* @param[in] num array length
	*
	*@return variance
	*/

	double variance = 0;
	double shift = values[num - 1]; // choose arbitrarily last value for shift
	double squares = 0;
	double sum = 0;
	for (int i = 0; i < num; ++i)
	{
		sum += values[i] - shift;
		squares += pow(values[i] - shift, 2);
	}
	variance = (squares - pow(sum,2)/num) / (num - 1);
	return variance;
}

double MyMath::periodic_function1(const double &x)
{
	/**
	*  2 Pi -periodic function. approximating a periodic rectangle function.
	*
	*  approximates 1 for (0 , 2Pi/6)
	*  approximates 0 for (2Pi/6 , 2Pi)
	*/

	return (1 / 6.0 + sqrt(3.0) / (2 * Pi) * cos(x)
		+ sqrt(3.0) / (4 * Pi) * cos(2 * x)
		- sqrt(3.0) / (8 * Pi) * cos(4 * x)
		- sqrt(3.0) / (10 * Pi) * cos(5 * x)
		+ 1.0 / (2 * Pi) * sin(x)
		+ 3.0 / (4 * Pi) * sin(2 * x)
		+ 2.0 / (3 * Pi) * sin(3 * x)
		+ 3.0 / (8 * Pi) * sin(4 * x)
		+ 1.0 / (10 * Pi) * sin(5 * x));
}

double MyMath::rectangular_function(const double &t0, const double &T, const int &n, const double &t) 
{
	/**
	* The function is 1 from -t0/2 to + t0/2; otherwise zero and T periodic.
	*
	* @param[in] t0 function 1 in interval 2*t0
	* @param[in] T function period
	* @param[in] n order by which rectangular function is approximated with sine functions
	* @param[in] t function argument
	*
	* @return function value at t
	*/

	double f = t0 / T;
	for (int i = 1; i < n + 1; ++i) {
		f += (2 / (i*Pi)) * sin(i*Pi*t0 / T) * cos(2 * Pi*i*t / T);
	}
	return f;
}

void MyMath::min_max_threedim(Threedim* threedimArray, const int &size, Threedim &min, Threedim &max)
{
	/**
	* Determines x_min, y_min, z_min and x_max, y_max, z_max from a array of three dimensional vectors
	*
	* @param[in] threedimArray Array of three dimension vectors
	* @param[in] size Number of vectors
	* @param[out] min (x_min, y_min, z_min) by reference
	* @param[out] max (x_max, y_max, z_max) by reference
	*/

	min = threedimArray[0];
	max = threedimArray[0];

	for (int i = 1; i < size; ++i)
	{
		if (threedimArray[i].x < min.x)
		{
			min.x = threedimArray[i].x;
		}
		if (threedimArray[i].x > max.x)
		{
			max.x = threedimArray[i].x;
		}
		if (threedimArray[i].y < min.y)
		{
			min.y = threedimArray[i].y;
		}
		if (threedimArray[i].y > max.y)
		{
			max.y = threedimArray[i].y;
		}
		if (threedimArray[i].z < min.z)
		{
			min.z = threedimArray[i].z;
		}
		if (threedimArray[i].z > max.z)
		{
			max.z = threedimArray[i].z;
		}
	}
}

void MyMath::min_max_double(double * a, const int &size, double &min, double &max)
{
	/**
	* Determines minimal and maximal number in an array of double values.
	*/

	min = a[0];
	max = a[0];
	for (int i = 0; i < size; i++)
	{
		if (a[i] < min)
		{
			min = a[i];
		}
		if (a[i] > max)
		{
			max = a[i];
		}
	}
}

std::vector<double> MyMath::linspace(const double &a, const double &b, const int &n)
{
	/**
	* Intended to be similar to Python numpy::linspace function. Creates array of values between a and b
	*
	* @param[in] a Start value
	* @param[in] b End value
	* @param[in] n Number of values
	*
	* @return n equidistant values between a and b (including a and b)
	*/
	std::vector<double> array;
	
	if (n < 2 )
	{
		array.push_back(a);
	}
	else {
		double value = a;
		double step = (b - a) / (n - 1);

		while (abs(value - b) > 0.0000001)
		{
			array.push_back(value);
			value += step;
		}
		array.push_back(b);
	}
	return array;
}

int MyMath::contains_value(int* array, const int &size, const int &value)
{
	/**
	* Check whether array contains a value
	* 
	* @param[in] array array of values
	* @param[in] size number of values in array
	* @param[in] value check whether array contains value
	*
	* @ return 0 if array does not contain value; 1 if array contains value
	*/

	int found = FALSE;
	for (int i = 0; i < size; i++)
	{
		if (array[i] == value)
		{
			found = TRUE;
		}
	}
	return found;
}

void MyMath::remove_value(int* &array, int &size, const int &value)
{
	/**
	* If array contains value, removes value from array
	* 
	* @param[in,out] array array of values
	* @param[in,out] size number of values in array
	* @param[in] value value to be removed
	*/

	int* pointer = NULL;
	if (contains_value(array, size, value) == TRUE)
	{
		pointer = new int[size - 1];
		int count = 0;
		for (int i = 0; i < size; i++)
		{
			if (array[i] != value)
			{
				pointer[count] = array[i];
				count += 1;
			}
		}
		size -= 1;
		delete[] array;
		array = pointer;
	}
}

void MyMath::add_value(int* &array, int &size, const int &value)
{
	/**
	* If array does not contain value, add value to array
	*
	* @param[in,out] array array of values
	* @param[in,out] size number of values in array
	* @param[in] value value to be added
	*/

	int* pointer = NULL;
	if (contains_value(array, size, value) == FALSE)
	{
		pointer = new int[size + 1];
		int count = 0;
		for (int i = 0; i < size; i++)
		{
				pointer[count] = array[i];
				count += 1;
		}
		pointer[size] = value;
		size += 1;
		delete[] array;
		array = pointer;
	}
}

double MyMath::topological_charge(const Threedim &x1, const Threedim &x2, const Threedim &x3)
{
	/**
	* Calculate projection of area spanned by three vectors onto unit sphere.
	*
	* @param[in] x1 First vector
	* @param[in] x2 Second vector
	* @param[in] x3 Third vector
	* @return are on unit sphere
	*/

	double N = 0;
	double D = 0;

	N = MyMath::dot_product(x1, MyMath::vector_product(x2, x3));
	D = 1 + MyMath::dot_product(x1, x2) + MyMath::dot_product(x1, x3)
		+ MyMath::dot_product(x2, x3);
	return 2 * atan2(N, D) / (4 * Pi);
}
