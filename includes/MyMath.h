/*
* MyMath.h
*
* 
*
*/

#ifndef MYMATH_H_
#define MYMATH_H_

#include <cmath>
#include <vector>
#include <math.h>

#include "typedefs.h"

#include <gsl/gsl_matrix.h>

/// Mathematical tool set

class MyMath {
public:
	MyMath();
	virtual ~MyMath();
    static int signum(double x);
	static double round(double value, int digits);
	static double norm(const Threedim &vec);
	static Threedim normalize(const Threedim &vec);
	static Threedim add(const Threedim &vec1, const Threedim &vec2);
	static Threedim mult(const Threedim &vec, const double &mult);
	static double dot_product(const Threedim &vec1, const Threedim &vec2);
	static Threedim vector_product(const Threedim &vec1, const Threedim &vec2);
	static Threedim difference(const Threedim &vec1, const Threedim &vec2);
	static double cosine_vectors(const Threedim &vec1, const Threedim &vec2);

	static Threedim matrix_vector_product(gsl_matrix* matrix, const Threedim &vec);
	static gsl_matrix* get_rotation_matrix(Threedim* array1, const int &index);

	static Twodim two_point_equation(const double &x1, const double &y1, const double &x2, const double &y2);		
	static double gauss(const double &sigma, const double &mu, const double &x);
	static double mean_value(double* values, const int &num);
	static double variance(double* values, const int &num);
	static double periodic_function1(const double &x);
	static double rectangular_function(const double &t0, const double &T, const int &n, const double &t);

	static void min_max_threedim(Threedim* threedimArray, const int &size, Threedim &min, Threedim &max);
	static void min_max_double(double* a, const int &size, double &min, double &max);
	
	static std::vector<double> linspace(const double &a, const double &b, const int &n);

	static int contains_value(int* array, const int &size, const int &value);
	static void remove_value(int* &array, int &size, const int &value);
	static void add_value(int* &array, int &size, const int &value);

	static double topological_charge(const Threedim &x1, const Threedim &x2, const Threedim &x3);
};

#endif /* MYMATH_H_ */
