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
	static double norm(Threedim vec);
	static Threedim normalize(Threedim vec);
	static Threedim add(Threedim vec1, Threedim vec2);
	static Threedim mult(Threedim vec, double mult);
	static double dot_product(Threedim vec1, Threedim vec2);
	static Threedim vector_product(Threedim vec1, Threedim vec2);
	static Threedim difference(Threedim vec1, Threedim vec2);
	static double cosine_vectors(Threedim vec1, Threedim vec2);

	static Threedim matrix_vector_product(gsl_matrix* matrix,Threedim vec);
	static gsl_matrix* get_rotation_matrix(Threedim* array1, int index);

	static Twodim two_point_equation(double x1, double y1, double x2, double y2);		
	static double gauss(double sigma, double mu, double x);
	static double mean_value(double* values, int num);
	static double variance(double* values, int num);
	static double periodic_function1(double x);
	static double rectangular_function(double t0, double T, int n, double t);

	static void min_max_threedim(Threedim* threedimArray, int size, Threedim &min, Threedim &max);
	static void min_max_double(double* a, int size, double &min, double &max);
	
	static std::vector<double> linspace(double a, double b, int n);

	static int contains_value(int* array, int size, int value);
	static void remove_value(int* &array, int &size, int value);
	static void add_value(int* &array, int &size, int value);

	static double topological_charge(Threedim x1, Threedim x2, Threedim x3);
};

#endif /* MYMATH_H_ */
