
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __VL__
#define __VL__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __TRJ_ENV__
#include <inttypes.h>
#endif

//------------------------------------------------------------------------------

#define vl_lsep "--------------------------------r\n"

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__
typedef signed char 			int8_t;
typedef unsigned char 			uint8_t;
typedef signed int 				int32_t;
typedef unsigned int  			uint32_t;
#endif

typedef double	 	float64_t;
typedef float      	float32_t;

typedef struct ufp32 { uint32_t data; } ufp32_t;
typedef struct fp32  { int32_t  data; } fp32_t ;

#ifndef __TRJ_ENV__
typedef struct ufp64 { uint64_t data; } ufp64_t;
typedef struct fp64  { int64_t  data; } fp64_t ;
#endif

typedef float64_t vlf_t;

#ifndef __TRJ_ENV__
inline fp64_t    fp64_float32  (float32_t float32_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float32_data) }; }
inline fp64_t    fp64_float64  (float64_t float64_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float64_data) }; }
inline ufp64_t   ufp64_float32 (float32_t float32_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float32_data) }; }
inline ufp64_t   ufp64_float64 (float64_t float64_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float64_data) }; }

inline float32_t float32_fp64  (fp64_t  fp64_data ) { return (float32_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float64_t float64_fp64  (fp64_t  fp64_data ) { return (float64_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float32_t float32_ufp64 (ufp64_t ufp64_data) { return (float32_t) (1E-12 * (float64_t) ufp64_data.data); }
inline float64_t float64_ufp64 (ufp64_t ufp64_data) { return (float64_t) (1E-12 * (float64_t) ufp64_data.data); }
#endif

//------------------------------------------------------------------------------

const vlf_t vl_pi  = 3.14159265358979323846264338327950288;
const vlf_t vl_2pi = 3.14159265358979323846264338327950288 * 2;

//------------------------------------------------------------------------------

inline vlf_t vl_rad(vlf_t deg) { return ((vlf_t) (((vlf_t) (deg)) * (vl_pi / 180))); }
inline vlf_t vl_deg(vlf_t rad) { return ((vlf_t) (((vlf_t) (rad)) * (180 / vl_pi))); }
inline vlf_t vl_atan2(vlf_t x, vlf_t y) { return atan2(x, y); }
inline vlf_t vl_sin(vlf_t x) { return sin(x); }
inline vlf_t vl_cos(vlf_t x) { return cos(x); }
inline vlf_t vl_sqrt(vlf_t x) { return sqrt(x); }
inline vlf_t vl_pow(vlf_t x, vlf_t power) { return pow(x, power); }
inline vlf_t vl_crt(vlf_t x) { return x < 0 ? -pow(-x, 1.0f/3.0f) : pow(x, 1.0f/3.0f); }

//------------------------------------------------------------------------------

inline void vl_vprint(vlf_t *vec)
{
	printf("%f %f %f", vec[0], vec[1], vec[2]);
}

inline void vl_mprint(vlf_t *mat)
{
	vl_vprint(&mat[0]);
	printf("\r\n");
	vl_vprint(&mat[3]);
	printf("\r\n");
	vl_vprint(&mat[6]);
	printf("\r\n");
}

//------------------------------------------------------------------------------

inline vlf_t vl_gauss1(vlf_t x, vlf_t m, vlf_t d)
{
	return exp(-0.5*((x-m)/d)*((x-m)/d)) / (d*2.50662827463);
}

//------------------------------------------------------------------------------

inline void vl_vcopy(vlf_t *res, vlf_t *vec)
{
	res[0] = vec[0];
	res[1] = vec[1];
	res[2] = vec[2];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mcopy(vlf_t *res, vlf_t *mat)
{
	res[0 + 0] = mat[0 + 0];
	res[0 + 1] = mat[0 + 1];
	res[0 + 2] = mat[0 + 2];
	
	res[3 + 0] = mat[3 + 0];
	res[3 + 1] = mat[3 + 1];
	res[3 + 2] = mat[3 + 2];
	
	res[6 + 0] = mat[6 + 0];
	res[6 + 1] = mat[6 + 1];
	res[6 + 2] = mat[6 + 2];
	
	return;
}

//------------------------------------------------------------------------------

inline vlf_t vl_dist(vlf_t *vec_0, vlf_t *vec_1)
{
	uint8_t i;
	
	vlf_t vec_dist = 0;
	vlf_t temp_float;
	
	for (i = 0; i < 3; ++i)
	{
		temp_float = vec_0[0] - vec_1[0];
		vec_dist += temp_float * temp_float;
	}
	
	return vl_sqrt(vec_dist);
}

inline vlf_t vl_dist2(vlf_t *vec_0, vlf_t *vec_1)
{
	uint8_t i;
	
	vlf_t vec_dist = 0;
	vlf_t temp_float;
	
	for (i = 0; i < 3; ++i)
	{
		temp_float = vec_0[0] - vec_1[0];
		vec_dist += temp_float * temp_float;
	}
	
	return vec_dist;
}

//------------------------------------------------------------------------------

inline vlf_t vl_vnorm(vlf_t *vec)
{
	return vl_sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

//------------------------------------------------------------------------------

inline vlf_t vl_vdot(vlf_t *vec_0, vlf_t *vec_1)
{
	return (vec_0[0]*vec_1[0] + vec_0[1]*vec_1[1] + vec_0[2]*vec_1[2]);
}

//------------------------------------------------------------------------------

inline void vl_cross(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[1]*vec_1[2] - vec_0[2]*vec_1[1];
	res[1] = vec_0[2]*vec_1[0] - vec_0[0]*vec_1[2];
	res[2] = vec_0[0]*vec_1[1] - vec_0[1]*vec_1[0];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_skew(vlf_t *res, vlf_t *vec)
{
	res[0 + 0] = 0;
	res[0 + 1] = -vec[2];
	res[0 + 2] = +vec[1];
	
	res[3 + 0] = +vec[2];
	res[3 + 1] = 0;
	res[3 + 2] = -vec[0];
	
	res[6 + 0] = -vec[1];
	res[6 + 1] = +vec[0];
	res[6 + 2] = 0;
	
	return;
}

//------------------------------------------------------------------------------

// def rot_norm(rot):

//     # return rot;
//     # print(rot)

//     u, a, v = scipy.linalg.svd(rot);
//     return numpy.mat(u.dot(v));

//     if abs(1.0 - scipy.linalg.det(rot)) > 1E-6:

//         u, a, v = scipy.linalg.svd(rot);
//         return numpy.mat(u.dot(v));

//     else:

//         return numpy.mat(rot);

//void vl_rm_norm(vlf_t *mat)
//{
//	vl_
//
//	return;
//}


//------------------------------------------------------------------------------

inline void vl_rm_align(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	vlf_t v[3];
	vlf_t s;
	vlf_t c;
	vlf_t k;
	
	vl_cross(v, vec_0, vec_1);
	
	s = vl_vnorm(v);
	c = vl_vdot(vec_0, vec_1);
	
	k = (1 - c) / s;
	
	res[0 + 0] = 1		+ k*(-v[2]*v[2]-v[1]*v[1]);
	res[0 + 1] = -v[2]	+ k*( v[0]*v[1]);
	res[0 + 2] = +v[1]	+ k*( v[0]*v[2]);
	
	res[3 + 0] = +v[2]	+ k*( v[0]*v[1]);
	res[3 + 1] = 1 		+ k*(-v[2]*v[2]-v[0]*v[0]);
	res[3 + 2] = -v[0]	+ k*( v[1]*v[2]);
	
	res[6 + 0] = -v[1]	+ k*( v[0]*v[2]);
	res[6 + 1] = +v[0]	+ k*( v[1]*v[2]);
	res[6 + 2] = 1		+ k*(-v[2]*v[2]-v[0]*v[0]);
	
	return;
}

//------------------------------------------------------------------------------

//inline void vl_vprint(vlf_t *vec)
//{
//	printf("%lf %lf %lf \n", vec[0], vec[1], vec[2]);
//
//	return;
//}
//
////------------------------------------------------------------------------------
//
//inline void vl_mprint(vlf_t *vec)
//{
//	printf("%lf %lf %lf \n", vec[0 + 0], vec[0 + 1], vec[0 + 2]);
//	printf("%lf %lf %lf \n", vec[3 + 0], vec[3 + 1], vec[3 + 2]);
//	printf("%lf %lf %lf \n", vec[6 + 0], vec[6 + 1], vec[6 + 2]);
//
//	printf("\n");
//
//	return;
//}

//------------------------------------------------------------------------------

inline void vl_vzero (vlf_t *vec)
{
	vec[0] = 0; vec[1] = 0; vec[2] = 0;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vset (vlf_t *vec, vlf_t value)
{
	vec[0] = value;
	vec[1] = value;
	vec[2] = value;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mzero (vlf_t *mat)
{
	mat[0] = 0; mat[1] = 0; mat[2] = 0;
	mat[3] = 0; mat[4] = 0; mat[5] = 0;
	mat[6] = 0; mat[7] = 0; mat[8] = 0;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mid (vlf_t *mat)
{
	mat[0] = 1; mat[1] = 0; mat[2] = 0;
	mat[3] = 0; mat[4] = 1; mat[5] = 0;
	mat[6] = 0; mat[7] = 0; mat[8] = 1;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_row (vlf_t *res, vlf_t *mat, uint8_t row)
{
	switch (row)
	{
		case  0: { vl_vcopy(res, &mat[0]); break; }
		case  1: { vl_vcopy(res, &mat[1]); break; }
		case  2: { vl_vcopy(res, &mat[2]); break; }
		default: { vl_vcopy(res, &mat[2]); break; }
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_col (vlf_t *res, vlf_t *mat, uint8_t col)
{
	switch (col)
	{
		case  0: { res[0] = mat[0]; res[1] = mat[3]; res[2] = mat[6]; break; }
		case  1: { res[0] = mat[1]; res[1] = mat[4]; res[2] = mat[7]; break; }
		case  2: { res[0] = mat[2]; res[1] = mat[5]; res[2] = mat[8]; break; }
		default: { res[0] = mat[2]; res[1] = mat[5]; res[2] = mat[8]; break; }
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_s(vlf_t *res, vlf_t *mat, vlf_t scalar)
{
	res[0 + 0] = res[0 + 0] * scalar;
	res[0 + 1] = res[0 + 1] * scalar;
	res[0 + 2] = res[0 + 2] * scalar;
	
	res[3 + 0] = res[3 + 0] * scalar;
	res[3 + 1] = res[3 + 1] * scalar;
	res[3 + 2] = res[3 + 2] * scalar;
	
	res[6 + 0] = res[6 + 0] * scalar;
	res[6 + 1] = res[6 + 1] * scalar;
	res[6 + 2] = res[6 + 2] * scalar;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_m(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;
	
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			res[i*3+j] = 0;
			
			for (k = 0; k < 3; ++k)
			{
				res[i*3+j] += mat_0[i*3+k] * mat_1[k*3+j];
			}
		}
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_v(vlf_t *res, vlf_t *mat, vlf_t *vec)
{
	res[0] = mat[0]*vec[0] + mat[1]*vec[1] + mat[2]*vec[2];
	res[1] = mat[3]*vec[0] + mat[4]*vec[1] + mat[5]*vec[2];
	res[2] = mat[6]*vec[0] + mat[7]*vec[1] + mat[8]*vec[2];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_tnp(vlf_t *res, vlf_t *mat)
{
	uint32_t i;
	uint32_t j;
	
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			res[j*3 + i] = mat[i*3 + j];
		}
	}
	
	return;
}

//------------------------------------------------------------------------------

inline vlf_t vl_det(vlf_t *mat)
{
	vlf_t d0 = mat[1*3+1]*mat[2*3+2] - mat[1*3+2]*mat[2*3+1];
	vlf_t d1 = mat[1*3+0]*mat[2*3+2] - mat[1*3+2]*mat[2*3+0];
	vlf_t d2 = mat[1*3+0]*mat[2*3+1] - mat[1*3+1]*mat[2*3+0];
	
	return d0*mat[0] - d1*mat[1] + d2*mat[2];
}

//------------------------------------------------------------------------------

inline void vl_msum(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint32_t i;
	
	for (i = 0; i < 9; ++i)
	{
		res[i] = mat_0[i] + mat_1[i];
	}
	
	return;
}

inline void vl_msub(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint32_t i;
	
	for (i = 0; i < 9; ++i)
	{
		res[i] = mat_0[i] - mat_1[i];
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vsum(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] + vec_1[0];
	res[1] = vec_0[1] + vec_1[1];
	res[2] = vec_0[2] + vec_1[2];
	
	return;
}

inline void vl_vsub(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] - vec_1[0];
	res[1] = vec_0[1] - vec_1[1];
	res[2] = vec_0[2] - vec_1[2];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vmul_s(vlf_t *res, vlf_t *vec, vlf_t scalar)
{
	res[0] = vec[0] * scalar;
	res[1] = vec[1] * scalar;
	res[2] = vec[2] * scalar;
	
	return;
}

inline void vl_vmul_v(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] * vec_1[0];
	res[1] = vec_0[1] * vec_1[1];
	res[2] = vec_0[2] * vec_1[2];
	
	return;
}

inline void vl_inv(uint32_t n, vlf_t *inv, vlf_t *mat_)
{
	vlf_t temp;
	vlf_t *mat = (vlf_t*) malloc(sizeof(vlf_t) * n * n);
	
	uint32_t i, j, k;
	
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			mat[i*n+j] = mat_[i*n+j];
			
			if (i == j)
			{ inv[n * i + j] = 1.0; }
			else
			{ inv[n * i + j] = 0.0; }
		}
	}
	
	for (k = 0; k < n; ++k)                                  //by some row operations,and the same row operations of
	{                                                       //Unit mat. I gives the inverse of matrix A
		temp = mat[n*k+k];                   //'temp'
		// stores the A[k][k] value so that A[k][k]  will not change
		for (j = 0; j < n; ++j)      //during the operation //A[i] //[j]/=A[k][k]  when i=j=k
		{
			mat[n*k+j] /= temp;
			inv[n*k+j] /= temp;
		}
		//R1=R1-R0*A[1][0] similarly for I
		for (i = 0; i < n; ++i)
		{
			temp = mat[n*i+k];
			
			for (j = 0; j < n; ++j)
			{                                   //R2=R2-R1*A[2][1]
				if(i == k) break;                      //R2=R2/A[2][2]
				mat[n*i+j] -= mat[n*k+j] * temp;          //R0=R0-R2*A[0][2]
				inv[n*i+j] -= inv[n*k+j] * temp;          //R1=R1-R2*A[1][2]
			}
		}
	}
	
	free(mat);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_unskew(vlf_t *res, vlf_t *mat)
{
	vlf_t temp_mat[9];
	vlf_t mat_tnp[9];
	
	vl_tnp(mat_tnp, mat);
	vl_msub(temp_mat, mat, mat_tnp);
	vl_mmul_s(mat, mat, 0.5);
	
	// mat = 0.5 * (mat - mat.T);
	
	res[0] = mat[2*3 + 1];
	res[1] = mat[0*3 + 2];
	res[2] = mat[1*3 + 0];
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __VL__ */




























