
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __VL__
#define __VL__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#include "trj_types.h"

//------------------------------------------------------------------------------

typedef float64_t vlf_t;

//------------------------------------------------------------------------------

#define vl_rad(deg) ((vlf_t) (((vlf_t) (deg)) * (M_PI / 180)))
#define vl_deg(rad) ((vlf_t) (((vlf_t) (rad)) * (180 / M_PI)))
#define vl_pi (3.14159265358979323846264338327950288)
#define vl_2pi (vl_pi*2)

//------------------------------------------------------------------------------

vlf_t vl_atan2(vlf_t x, vlf_t y);
vlf_t vl_sin(vlf_t x);
vlf_t vl_cos(vlf_t x);
vlf_t vl_sqrt(vlf_t x);
vlf_t vl_pow(vlf_t x, vlf_t power);
vlf_t vl_crt(vlf_t x);

vlf_t vl_vnorm(vlf_t *vec);
void vl_vsum(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1);
void vl_vmul_s(vlf_t *res, vlf_t *vec, vlf_t scalar);
void vl_vsub(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1);
void vl_vcopy(vlf_t *res, vlf_t *vec);
void vl_mcopy(vlf_t *res, vlf_t *mat);
vlf_t vl_sqrt(vlf_t x);
vlf_t vl_dist(vlf_t *vec_0, vlf_t *vec_1);
vlf_t vl_norm(vlf_t *vec);
vlf_t vl_vdot(vlf_t *vec_0, vlf_t *vec_1);
void vl_cross(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1);
void vl_skew(vlf_t *res, vlf_t *vec);
void vl_rm_align(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1);
void vl_vprint(vlf_t *vec);
void vl_mprint(vlf_t *vec);
void vl_vzero (vlf_t *vec);
void vl_mzero (vlf_t *mat);
void vl_row (vlf_t *res, vlf_t *mat, uint8_t row);
void vl_col (vlf_t *res, vlf_t *mat, uint8_t col);
void vl_mmul_s(vlf_t *res, vlf_t *mat, vlf_t scalar);
void vl_mmul_m(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1);
void vl_tnp(vlf_t *res, vlf_t *mat);
vlf_t vl_det(vlf_t *mat);
void vl_msum(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1);
void vl_msub(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1);
void vl_unskew(vlf_t *res, vlf_t *mat);
void vl_mmul_v(vlf_t *res, vlf_t *mat, vlf_t *vec);
void vl_inv(uint32_t n, vlf_t *inv, vlf_t *mat);

//------------------------------------------------------------------------------

#endif /* __VL__ */




























