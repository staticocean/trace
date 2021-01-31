
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_CTRL___
#define __TRJ_CTRL___

//------------------------------------------------------------------------------

#include "vl.h"
#include "vl_svd.h"

#include "trj_api.h"
#include "trj_obj.h"
#include "trj_utils.h"

//------------------------------------------------------------------------------

typedef struct __trj_ctrl_gm_k__
{
	uint32_t n;
	uint32_t m;
	
	float64_t C;
	float64_t S;
	float64_t dC;
	float64_t dS;
	
}   __s_trj_ctrl_gm_k__;

typedef struct trj_ctrl_gm
{
	uint32_t order;
	char file_name[256];
	
	uint32_t  data_size;
	__s_trj_ctrl_gm_k__ *data_list;
	
}   s_trj_ctrl_gm;

typedef struct trj_ctrl_gm_init
{
	uint32_t order;
	char file_name[256];
	
}   s_trj_ctrl_gm_init;

//------------------------------------------------------------------------------

inline char *__sgets__(char *s, int n, const char **strp){
	if(**strp == '\0')return NULL;
	int i;
	for(i=0;i<n-1;++i, ++(*strp)){
		s[i] = **strp;
		if(**strp == '\0')
			break;
		if(**strp == '\n'){
			s[i+1]='\0';
			++(*strp);
			break;
		}
	}
	if(i==n-1)
		s[i] = '\0';
	return s;
}

inline uint8_t trj_ctrl_gm_init(s_trj_ctrl_gm *self, s_trj_ctrl_gm_init attr)
{
	strcpy(self->file_name, attr.file_name);
	self->order = attr.order;
	
	FILE *file_handle = fopen(self->file_name, "r");
	
	if (file_handle == NULL)
	{
		printf("default gm file not found");
	}
	
	else
	{
		fseek(file_handle , 0, SEEK_END);
		size_t file_size = ftell (file_handle);
		rewind (file_handle);
		
		uint8_t *file_data = (uint8_t*) malloc(file_size);
		fread(file_data, 0x01, file_size, file_handle);
		
		fclose(file_handle);
		
		uint32_t line_count = 0x00;
		uint32_t offset;
		
		for (offset = 0x00; offset < file_size; ++offset)
		{
			if      (file_data[offset] == '\n') { ++line_count; }
			else if (file_data[offset] == 'D') { file_data[offset] = 'E'; }
		}
		
		self->data_size = line_count;
		self->data_list = (__s_trj_ctrl_gm_k__*) malloc(sizeof(__s_trj_ctrl_gm_k__) * line_count);
		
		line_count = 0x00;
		__s_trj_ctrl_gm_k__ temp;
		
		char buff[512];
		const char **p = (const char**) &file_data;
		
		while(NULL != __sgets__(buff, sizeof(buff), p))
		{
			sscanf(buff, "%d %d %lf %lf %lf %lf %*s",
				   &temp.n, &temp.m, &temp.C, &temp.S, &temp.dC, &temp.dS);
			
			self->data_list[line_count] = temp;
			
			++line_count;
		}
		
		printf("loaded %d lines \n", line_count);
		fflush(stdout);
	}
	
	return 0x00;
}

inline uint8_t trj_ctrl_gm_save(s_trj_ctrl_gm *self, s_trj_ctrl_gm_init *attr, uint8_t **v_file)
{
	memcpy(*v_file, self->data_list, self->data_size * sizeof(__s_trj_ctrl_gm_k__));
	*v_file += self->data_size * sizeof(__s_trj_ctrl_gm_k__);
	
	return 0x00;
}

inline uint8_t trj_ctrl_gm_load(s_trj_ctrl_gm *self, s_trj_ctrl_gm_init *attr, uint8_t **v_file)
{
	self->data_list = (__s_trj_ctrl_gm_k__*) malloc(self->data_size * sizeof(__s_trj_ctrl_gm_k__));
	memcpy(self->data_list, *v_file, self->data_size * sizeof(__s_trj_ctrl_gm_k__));
	*v_file += self->data_size * sizeof(__s_trj_ctrl_gm_k__);
	
	return 0x00;
}

inline uint8_t trj_ctrl_gm_reset(s_trj_ctrl_gm *self, s_trj_obj *obj)
{
	return 0x00;
}

inline int factorial(int n) {
	int result = 1;
	if(n<0)
		return -1;
	for (int i = 1; i <= n; ++i)
		result *= i;
	return result;
}

inline void legendre_order0(vlf_t *L, vlf_t *Ld, uint32_t N, vlf_t x)
{
	L[0] = 1;
	L[1] = x;
	Ld[0] = 0;
	Ld[1] = 1;
	
	uint32_t i;
	uint32_t n;
	
	for (i = 2; i < N; ++i) {
		
		n = i - 2;
		
		L[i] = ((2 * n + 1) * x * L[i - 1] - n * L[i - 2]) / (n + 1);
		Ld[i] = ((2 * n + 1) * (L[i - 1] + x * Ld[i - 1]) - n * Ld[i - 2]) / (n + 1);
	}
	
	return;
}

inline void associated_legendre(vlf_t *P, vlf_t *Pd, uint32_t N, vlf_t x)
{
	// Evaluates all fully normalized associated Legendre polynomials
	// of degree and order less than or equal to N-1, and their first derivatives at x.
	//
	// For n greater than or equal to m:
	// P(m+1,n+1) = P_n^m(x)
	// Pd(m+1, n+1) = d/dx P_n^m(x)
	
	uint32_t i;
	uint32_t j;
	uint32_t m;
	uint32_t n;
	
	vlf_t full_derivs[N][N];
	
	// compute P^n_0
	
	vlf_t L[N];
	vlf_t Ld[N];
	legendre_order0(L, Ld, N,x);
	
	memcpy(&P[0], L, sizeof(vlf_t)*N);
	memcpy(&Pd[0], Ld, sizeof(vlf_t)*N);
	
	// calculate all derivatives up to order N of of P^n_0
	memcpy(&full_derivs[0][0], Ld, sizeof(vlf_t)*N);
	
	for (j = 1; j < N; ++j)
	{
		for (i = 2; i < N; ++i)
		{
			n = i - 2;
			
			full_derivs[j][i] = ((2*n + 1)*(j*full_derivs[j-1][i-1]+ x*full_derivs[j][i-1])
								 - n*full_derivs[j][i-2]) / (n + 1);
		}
	}
	
	// compute P^m_n
	
	for (m = 1; m < N; ++m)
	{
		for (n = m; n < N; ++n)
		{
			vlf_t factor = 1;
			
			P[m*N+n] = factor * vl_pow((1 - x*x), ((m - 1) / 2)) * full_derivs[m - 1][n];
			Pd[m*N+n] = -factor * ((m - 1) / 2) * pow((1 - x*x), (((m - 1) / 2) - 1)) * 2 * x
						+ factor * pow((1 - x*x), ((m - 1) / 2)) * full_derivs[m][n];
		}
	}
	
	// normalize values
	
	for (n = 0; n < N; ++n)
	{
		for (m = 0; m < N; ++m)
		{
			if (n >= m)
			{
				vlf_t factor = (2 * n + 1) * factorial(n - m) / factorial(n + m);
				if (m != 0) { factor = factor * 2; }
				factor = sqrt(factor);
				
				P[m*N+n] *= factor;
				Pd[m*N+n] *= factor;
			}
		}
	}
	
	return;
}

inline uint8_t trj_ctrl_gm_calc(s_trj_ctrl_gm *self, vlf_t *g, vlf_t *ecef)
{
	vlf_t lla[3];
	
	trj_ellp_glla(&trj_ellp_wgs84, lla, ecef);
	
	vlf_t MU = 3.986004415e14;      // Gravitational parameter of Earth    [km^3 s^-2]
	vlf_t A  = 6.3781363e6;          // Mean equitorial radius              [m]
	
	vlf_t el = lla[0];
	vlf_t az = lla[1];
	vlf_t r  = vl_vnorm(ecef);
	
	vlf_t u = MU / r;
	vlf_t g_[3];
	g_[0] = -MU / (r*r);
	
	uint32_t N = self->order;
	
	vlf_t P[N*N];
	vlf_t Pd[N*N];
	
	associated_legendre(P, Pd, N, sin(el));
	
	uint32_t i;
	uint32_t N_ = 0x00;
	
	for (i = 0; i < self->order+1; ++i)
	{
		N_ += i;
	}
	
	for (i = 0; i < N_-3; ++i)
	{
		__s_trj_ctrl_gm_k__ *k = &self->data_list[i];
		
		uint32_t n = k->n;
		uint32_t m = k->m;
		vlf_t    C = k->C;
		vlf_t    S = k->S;
		
		vlf_t Pnm  = P [m*N + n];
		vlf_t Pnmd = Pd[m*N + n];
		
		u = u + vl_pow(A,n) * MU * (Pnm * (C * cos(m * az) + S * sin(m * az))) / vl_pow(r, n+1);
		
		g_[0] -= vl_pow(A,n) * (n + 1) * MU * Pnm * (C * cos(m * az) + S * sin(m * az)) / vl_pow(r, (n + 2));
		g_[1] += vl_pow(A,n) * MU * Pnmd * cos(el) * (C * cos(m * az) + S * sin(m * az)) / vl_pow(r, (n + 2));
		g_[2] += vl_pow(A,n) * MU * m * Pnm * (S * cos(m * az) - C * sin(m * az)) / vl_pow(r, (n + 2) * sin(el));
	}
//
//    vlf_t rot[9] = {
//            sin(el)*cos(az), cos(el)*cos(az), -sin(az),
//            sin(el)*sin(az), cos(el)*sin(az),  cos(az),
//            cos(el),         -sin(el),        0
//    };
//
//    vl_mmul_v(g, rot, g_);
	
	vl_vcopy(g, g_);
	
	return 0x00;
}

inline uint8_t trj_ctrl_gm_update(s_trj_ctrl_gm *self, s_trj_obj *obj)
{
	return 0x00;
}

inline uint8_t trj_ctrl_gm_init_(void **data, void *config)
{
	*data = (s_trj_ctrl_gm*) malloc(sizeof(s_trj_ctrl_gm));
	
	s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) *data;
	s_trj_ctrl_gm_init *init = (s_trj_ctrl_gm_init*) config;
	
	return trj_ctrl_gm_init(ctrl, *init);
}

inline uint8_t trj_ctrl_gm_free_(void **data)
{
	s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) *data;
	
	if (ctrl->data_list != NULL)
	{
		free(ctrl->data_list);
		ctrl->data_list = NULL;
	}
	
	free(ctrl);
	
	return 0x00;
}

inline uint8_t trj_ctrl_gm_save_(void *data, void *config, uint8_t **v_file)
{
	s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) data;
	s_trj_ctrl_gm_init *attr = (s_trj_ctrl_gm_init*) config;
	
	return trj_ctrl_gm_save(ctrl, attr, v_file);
}

inline uint8_t trj_ctrl_gm_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) data;
	s_trj_ctrl_gm_init *attr = (s_trj_ctrl_gm_init*) config;
	
	return trj_ctrl_gm_load(ctrl, attr, v_file);
}

inline uint8_t trj_ctrl_gm_reset_(void *data, void *obj)
{
	s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_gm_reset(ctrl, obj_);
}

inline uint8_t trj_ctrl_gm_update_(void *data, void *obj)
{
	s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_gm_update(ctrl, obj_);
}

//------------------------------------------------------------------------------

#endif /* __INS_CTRL__ */




