#ifndef _CVADDON_MATH_H
#define _CVADDON_MATH_H


////////////////////////////////////////////////////////////
//                CvAddon Math Functions
////////////////////////////////////////////////////////////
// By Wai Ho Li
////////////////////////////////////////////////////////////
// Math functions including:
// - Matrix operations
// - CvRect resize
// - MAX, MIN etc
////////////////////////////////////////////////////////////
// TODO
// ---
// DOC issues:
// Check and reject bad hessians? (abs(shift) > 1 etc)
////////////////////////////////////////////////////////////

#include <cv.h>
#include <iostream>
using std::endl;
using std::cerr;

#define CV_MAT_VAL(mat, type, row, col) \
	( ((type*)( (mat)->data.ptr + (mat)->step* (row) ))[ (col) ] )


// Orders two scalars such that their elements are placed into a 
// lower and upper resulting scalar
// eg. (1,2,3); (4,0,6) ==> lower:(1,0,3), upper:(4,2,6)
inline void lowerUpper(const CvScalar& a, const CvScalar& b, CvScalar& lower, CvScalar& upper)
{
	int i;
	for(i = 0; i < 4; ++i) {
		if(a.val[i] > b.val[i]) {
			lower.val[i] = b.val[i];
			upper.val[i] = a.val[i];
		}
		else {
			lower.val[i] = a.val[i];
			upper.val[i] = b.val[i];
		}
	}
}

// Resizes CvRect based on two ratios
inline void cvAddonResizeRect(CvRect &rect, const float& widthRatio, const float &heightRatio)
{
	rect.x += (1.0f - widthRatio)/2 * rect.width;
	rect.width *= widthRatio;
	rect.y += (1.0f - heightRatio)/2 * rect.height;
	rect.height *= heightRatio;
}

// Finds local extrema in 3x3 array of values relative
// to center element's location
// <dif> should be a 2x1 vector of floats
// <mat> should be a 2x2 matrix of floats
// <shift> should be a 2x1 vector of floats.
// ** Variables are ordered x,y
// Returns pointer to 2x1 results array
inline float *cvAddonFindExtrema3x3_2D(const float arr[9], CvMat *vec, CvMat *mat, CvMat *shift)
{
	float *diff = (vec->data.fl);
	float *hessian = (mat->data.fl);

#define val(x, y) (arr[3*(y+1) + (x+1)])

	diff[0] = -0.5f * (val(1, 0) - val(-1, 0));
	diff[1] = -0.5f * (val(0, 1) - val(0, -1));
	
	// Hessian
	hessian[0] = val(1,0) + val(-1,0) - 2.0f * val(0,0);
	hessian[1] = hessian[2] = 0.25f * ( val(1,1) + val(-1,-1) - val(-1,1) - val(+1,-1) );
	hessian[3] = val(0,1) + val(0,-1) - 2.0f * val(0,0);

#undef val
	
	cvSolve(mat, vec, shift, CV_LU);

	return shift->data.fl;
}

#endif