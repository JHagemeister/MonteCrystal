/*
* ExcitationVisualization.cpp
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

#include "ExcitationVisualization.h"

#include "MyMath.h"

#include <stdlib.h>

ExcitationVisualization::ExcitationVisualization(Threedim* spinArray, Threedim* spinArrayBackup, 
	int numberAtoms, std::vector<double>* evalues, gsl_matrix_complex* evec, 
	ExcitationFrequencyParameters parameters)
{
	/**
	@param[in] spinArray Pointer to displayed (openGlWidget) spin array.
	@param[in] spinArrayBackup Pointer to array containing copy of spin array.
	@param[in] numberAtoms Number of spins
	@param[in] evalues Eigenvalues of the excitation modes
	@param[in] evec Eigenvectors of the excitation modes
	@param[in] parameters To influence visualization from other parallel thread
	*/

	_spinArray = spinArray;
	_spinArrayBackup = spinArrayBackup;
	_numberAtoms = numberAtoms;
	_parameters = parameters;

	_evalues = evalues;
	_evec = evec;

	update();
}

ExcitationVisualization::~ExcitationVisualization()
{
}

void ExcitationVisualization::update(void)
{
	/**
	This function needs to be called when user wants to switch to visualisation
	of another eigenstate. The necessary eigenvalues and eigenvectors are loaded
	and a scaling value for the eigenvector is determined.
	*/

	// _index is pointer whose value identifies the desired eigenstate
	_evec_i = gsl_matrix_complex_column(_evec, *_parameters.index); // update eigenvector
	_eval_i = (*_evalues)[ *_parameters.index]; // update eigenvalue

	// In the following: Identification of suitable scaling value of eigenvector
	double* values = new double[2 * _numberAtoms]; // room for p and q values -> Levente thesis
	double max = 0; // value identifying maximum spin deterioration from equilibrium direction

	for (int j = 0; j < _numberAtoms; ++j)
	{
		// q-value
		gsl_complex z = gsl_vector_complex_get(&_evec_i.vector, j);
		values[j] = GSL_REAL(z);
		// corresponding p-value
		z = gsl_vector_complex_get(&_evec_i.vector, _numberAtoms + j);
		values[_numberAtoms + j] = GSL_REAL(z);

		double value = pow(values[j], 2) + pow(values[_numberAtoms + j], 2);
		if (value > max)
		{
			max = value;
		}
	}

	double cosTheta = cos(*_parameters.maxAngle * Pi / 180);
	_scalingValue = sqrt(((1 / pow(cosTheta, 2)) - 1) / max);

	delete[] values;
}

void ExcitationVisualization::segmentation(void)
{
	// find center of skyrmion structure
	Threedim center{ 0,0,0 };
	double* segmentationImage = new double[_numberAtoms];

	std::vector<int> points;
	for (int i = 0; i < _numberAtoms; ++i)
	{
		segmentationImage[i] = pow(_spinArray[i].z, 2);
		if (segmentationImage[i] < *_parameters.segmentationThreshold)
		{
			segmentationImage[i] = 0;
			points.push_back(i);
		}
		else
		{
			segmentationImage[i] = 1;
		}
	}
	emit send_points(points);

	delete[] segmentationImage;
}

void ExcitationVisualization::visualize(void)
{
	/**
	Does visualisation of eigenstate.
	*/

	double time = 0; // initialize time at zero
	double omega = _eval_i; // omega is given by the eigenvalue
	double timeStep = 2 * Pi / (fabs(omega) * 100); // do 100 visualization updates during one period 
	
	// frameIndex is needed for png file output. It is somehow not that nicely solved.
	// frameIndex is abitrarily initialized with 101. in this case no files are put
	// out during loop. When the user triggers storage of video squence, the frameIndex
	// is set to 0. it is then incremented by steps of 1 back up to 101 during the loop
	// and while this files are stored.
	int frameIndex = 101;

	// do visualization forever as long as user does not request termination
	while (*_parameters.terminate != TRUE)
	{
#ifdef WIN	
	   _sleep(*_parameters.sleepTime); // slow down visualisation
#elif LINUX
           struct timespec ts = {0,(*_parameters.sleepTime)*1000000};
           nanosleep(&ts,NULL);
#endif
		if (*_parameters.pause != TRUE) // pause visualisation
		{
			if (*_parameters.rescale) // _rescale is set in ExcitationFrequencyWindow to trigger update of parameters
			{
				update(); // update eigenvalue, eigenvector, scaling parameter
				omega = _eval_i; // update omega with eigenvalue
				timeStep = 2 * Pi / (fabs(omega) * 100); // update timestep
				time = 0;
				*_parameters.rescale = 0; // rescaling done, so set to 0
			}

			if (*_parameters.videoSequence) // _videoSequence is set in ExcitationFrequencyWindow to trigger png output
			{
				frameIndex = 0;
				*_parameters.videoSequence = 0; // set to zero because video sequence is now initialized
			}

			// add suitable portions of the eigenvectors to the initial spin state
			for (int i = 0; i < _numberAtoms; ++i)
			{
				// q value
				gsl_complex z1 = gsl_vector_complex_get(&_evec_i.vector, i);
				double value1 = GSL_REAL(z1) * cos(omega*time) - GSL_IMAG(z1) * sin(omega*time);
				// p value
				gsl_complex z2 = gsl_vector_complex_get(&_evec_i.vector, _numberAtoms + i);
				double value2 = GSL_REAL(z2) * cos(omega*time) - GSL_IMAG(z2) * sin(omega*time);
				// get the rotation matrix that was used to rotate spin along z-direction
				gsl_matrix* matrix = MyMath::get_rotation_matrix(_spinArrayBackup, i);
				// transpose matrix to perform back-rotation
				gsl_matrix_transpose(matrix);
				// add scaled q,p-vlaues to (0,0,1) vector prior to back-rotation
				Threedim spin{ value2*_scalingValue, value1*_scalingValue, 1 };
				spin = MyMath::normalize(spin);
				// rotate spin back to global coordinate system
				spin = MyMath::matrix_vector_product(matrix, spin);
				// update spin in array that is used for displaying in openGL-Widget
				_spinArray[i] = spin;

				gsl_matrix_free(matrix);
			}

			segmentation();

			time += timeStep;
			emit send_repaint_request(); // repaint openGL widget

			// store current configuration as png file if ouput was initialized
			if (frameIndex < 100)
			{
				emit send_save_image_request(frameIndex);
				frameIndex += 1;
				// pause visualization loop of eigenstate until image was stored.
				// _pause is reset to False in ExcitationFrequencyWindow after storage.
				*_parameters.pause = TRUE;
			}
			else if (frameIndex == 100)
			{
				// image sequence is finished and notification is send to ExcitationFrequencyWindow
				// There, it is kept track of how many image sequences have been stored in order to
				// allow unique folder names. Also the button for the user to trigger storage of an
				// image sequence is enabled again.
				emit send_video_sequence_finished();
				frameIndex += 1;
			}
		}
	}

	emit send_finished();
}
