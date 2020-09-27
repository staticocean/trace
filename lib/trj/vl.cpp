
#include "vl.h"

//------------------------------------------------------------------------------

vlf_t vl_atan2(vlf_t x, vlf_t y) { return atan2(x, y); }
vlf_t vl_sin(vlf_t x) { return sin(x); }
vlf_t vl_cos(vlf_t x) { return cos(x); }
vlf_t vl_sqrt(vlf_t x) { return sqrt(x); }
vlf_t vl_pow(vlf_t x, vlf_t power) { return pow(x, power); }
vlf_t vl_crt(vlf_t x) { return x < 0 ? -pow(-x, 1.0f/3.0f) : pow(x, 1.0f/3.0f); }
//------------------------------------------------------------------------------

void vl_vcopy(vlf_t *res, vlf_t *vec)
{
	res[0] = vec[0];
	res[1] = vec[1];
	res[2] = vec[2];

	return;
}

//------------------------------------------------------------------------------

void vl_mcopy(vlf_t *res, vlf_t *mat)
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

vlf_t vl_dist(vlf_t *vec_0, vlf_t *vec_1)
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

vlf_t vl_dist2(vlf_t *vec_0, vlf_t *vec_1)
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

vlf_t vl_vnorm(vlf_t *vec)
{
	return vl_sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

//------------------------------------------------------------------------------

vlf_t vl_vdot(vlf_t *vec_0, vlf_t *vec_1)
{
	return (vec_0[0]*vec_1[0] + vec_0[1]*vec_1[1] + vec_0[2]*vec_1[2]);
}

//------------------------------------------------------------------------------

void vl_cross(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[1]*vec_1[2] - vec_0[2]*vec_1[1];
	res[1] = vec_0[2]*vec_1[0] - vec_0[0]*vec_1[2];
	res[2] = vec_0[0]*vec_1[1] - vec_0[1]*vec_1[0];

	return;
}

//------------------------------------------------------------------------------

void vl_skew(vlf_t *res, vlf_t *vec)
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

void vl_unskew(vlf_t *res, vlf_t *mat)
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

void vl_rm_align(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
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

void vl_vprint(vlf_t *vec)
{
	printf("%lf %lf %lf \n", vec[0], vec[1], vec[2]);
    
    return;
}

//------------------------------------------------------------------------------

void vl_mprint(vlf_t *vec)
{
	printf("%lf %lf %lf \n", vec[0 + 0], vec[0 + 1], vec[0 + 2]);
	printf("%lf %lf %lf \n", vec[3 + 0], vec[3 + 1], vec[3 + 2]);
	printf("%lf %lf %lf \n", vec[6 + 0], vec[6 + 1], vec[6 + 2]);
    
    printf("\n");

    return;
}

//------------------------------------------------------------------------------

void vl_vzero (vlf_t *vec)
{
    vec[0] = 0; vec[1] = 0; vec[2] = 0;
	
	return;
}

//------------------------------------------------------------------------------

void vl_mzero (vlf_t *mat)
{
    mat[0] = 0; mat[1] = 0; mat[2] = 0;
    mat[3] = 0; mat[4] = 0; mat[5] = 0;
    mat[6] = 0; mat[7] = 0; mat[8] = 0;
    
    return;
}

//------------------------------------------------------------------------------

void vl_row (vlf_t *res, vlf_t *mat, uint8_t row)
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

void vl_col (vlf_t *res, vlf_t *mat, uint8_t col)
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

void vl_mmul_s(vlf_t *res, vlf_t *mat, vlf_t scalar)
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

void vl_mmul_m(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
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

void vl_mmul_v(vlf_t *res, vlf_t *mat, vlf_t *vec)
{
    res[0] = mat[0]*vec[0] + mat[1]*vec[1] + mat[2]*vec[2];
    res[1] = mat[3]*vec[0] + mat[4]*vec[1] + mat[5]*vec[2];
    res[2] = mat[6]*vec[0] + mat[7]*vec[1] + mat[8]*vec[2];
    
    return;
}

//------------------------------------------------------------------------------

void vl_tnp(vlf_t *res, vlf_t *mat)
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

vlf_t vl_det(vlf_t *mat)
{
	vlf_t d0 = mat[1*3+1]*mat[2*3+2] - mat[1*3+2]*mat[2*3+1];
	vlf_t d1 = mat[1*3+0]*mat[2*3+2] - mat[1*3+2]*mat[2*3+0];
	vlf_t d2 = mat[1*3+0]*mat[2*3+1] - mat[1*3+1]*mat[2*3+0];

	return d0*mat[0] - d1*mat[1] + d2*mat[2];
}

//------------------------------------------------------------------------------

void vl_msum(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
    uint32_t i;
    
    for (i = 0; i < 9; ++i)
    {
        res[i] = mat_0[i] + mat_1[i];
    }
    
    return;
}

void vl_msub(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
    uint32_t i;
    
    for (i = 0; i < 9; ++i)
    {
        res[i] = mat_0[i] - mat_1[i];
    }
    
    return;
}

//------------------------------------------------------------------------------

void vl_vsum(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
    res[0] = vec_0[0] + vec_1[0];
    res[1] = vec_0[1] + vec_1[1];
    res[2] = vec_0[2] + vec_1[2];
    
    return;
}

void vl_vsub(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
    res[0] = vec_0[0] - vec_1[0];
    res[1] = vec_0[1] - vec_1[1];
    res[2] = vec_0[2] - vec_1[2];
    
    return;
}

//------------------------------------------------------------------------------

void vl_vmul_s(vlf_t *res, vlf_t *vec, vlf_t scalar)
{
    res[0] = vec[0] * scalar;
    res[1] = vec[1] * scalar;
    res[2] = vec[2] * scalar;
    
    return;
}

void vl_vmul_v(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
    res[0] = vec_0[0] * vec_1[0];
    res[1] = vec_0[1] * vec_1[1];
    res[2] = vec_0[2] * vec_1[2];
    
    return;
}

void vl_inv(uint32_t n, vlf_t *inv, vlf_t *mat)
{
//	vlf_t ratio,a;
//
//	int i, j, k;
//
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			if (i == j) inv[n*i+j] = 1.0;
//			else inv[n*i+j] = 0.0;
//		}
//	}
//
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			if (i != j)
//			{
//				ratio = mat[n*j+i] / mat[n*i+i];
//
//				for (k = 0; k < n; ++k)
//				{
//					inv[n*j+k] -= ratio * inv[n*i+k];
//				}
//			}
//		}
//	}
//
//	for(i = 0; i < n; i++)
//	{
//		a = mat[n*i+i];
//
//		for(j = 0; j < n; j++)
//		{
//			inv[n*i+j] /= a;
//		}
//	}
//
//	return;
	
	vlf_t temp;
	uint32_t i, j, k;

	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
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
	
	return;
	
//	int i, j, k;
//
//	for (i = 1; i <= n; i++)
//		for (j = 1; j <= 2 * n; j++)
//			if (j == (i + n))
//				a[i][j] = 1;
//
//	/************** partial pivoting **************/
//	for (i = n; i > 1; i--)
//	{
//		if (a[i - 1][1] < a[i][1])
//			for (j = 1; j <= n * 2; j++)
//			{
//				d = a[i][j];
//				a[i][j] = a[i - 1][j];
//				a[i - 1][j] = d;
//			}
//	}
//	cout << "pivoted output: " << endl;
//	for (i = 1; i <= n; i++)
//	{
//		for (j = 1; j <= n * 2; j++)
//			cout << a[i][j] << "    ";
//		cout << endl;
//	}
//	/********** reducing to diagonal  matrix ***********/
//
//	for (i = 1; i <= n; i++)
//	{
//		for (j = 1; j <= n * 2; j++)
//			if (j != i)
//			{
//				d = a[j][i] / a[i][i];
//				for (k = 1; k <= n * 2; k++)
//					a[j][k] -= a[i][k] * d;
//			}
//	}
//	/************** reducing to unit matrix *************/
//	for (i = 1; i <= n; i++)
//	{
//		d = a[i][i];
//		for (j = 1; j <= n * 2; j++)
//			a[i][j] = a[i][j] / d;
//	}
//
	
	
	
	return;
}

//------------------------------------------------------------------------------








