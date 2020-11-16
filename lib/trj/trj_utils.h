
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_UTILS__
#define __TRJ_UTILS__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

#include "trj_obj.h"

//------------------------------------------------------------------------------

typedef struct trj_pos_lla
{
    vlf_t lat;
    vlf_t lon;
    vlf_t alt;
    
}   trj_pos_lla_t;

typedef struct trj_pos_ecef
{
    vlf_t vec[3];
    
}   trj_pos_ecef_t;

typedef struct insu_rot_hpr
{
    vlf_t heading;
    vlf_t pitch;
    vlf_t roll;
    
}   s_trj_rot_hpr;

//------------------------------------------------------------------------------

inline void trj_gforce(vlf_t *res, s_trj_obj *obj_0, s_trj_obj *obj_1)
{
	vlf_t dir[3];
	
	vl_vsub(dir, obj_0->pos[0], obj_1->pos[0]);
	
	vlf_t dist = vl_vnorm(dir);
	
	if (dist > 1E-9)
	{
		vl_vmul_s(dir, dir, 1 / dist);
		// gforce = dir * 6.67408E-11 * obj_0->pos_inert * obj_1->pos_inert / (dist * dist);
		vl_vmul_s(res, dir, 6.67408E-11 * obj_0->pos_inert * obj_1->pos_inert / (dist * dist));
	}
	
	else
	{
		vl_vzero(res);
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_ecef_to_lla(trj_pos_lla_t *pos_lla, trj_pos_ecef_t *pos_ecef)
{
	vlf_t x = pos_ecef->vec[0];
	vlf_t y = -pos_ecef->vec[2];
	vlf_t z = pos_ecef->vec[1];
	
	const vlf_t a = 6378137;
//    const vlf_t e = 8.1819190842622e-2;
	
	const vlf_t asq = 6378137.0 * 6378137.0;
	const vlf_t esq = 8.1819190842622e-2 * 8.1819190842622e-2;
	
	vlf_t b   = vl_sqrt(asq * (1 - esq));
	vlf_t bsq = b * b;
	
	vlf_t ep  = vl_sqrt((asq - bsq)/bsq);
	vlf_t p   = vl_sqrt(x*x + y*y);
	vlf_t th  = vl_atan2(a * z, b * p);
	
	pos_lla->lon = vl_atan2(y, x);
	pos_lla->lat = vl_atan2( (z + ep*ep * b * vl_pow(vl_sin(th), 3)), (p - esq * a * vl_pow(vl_cos(th), 3)) );
	vlf_t N = a / ( vl_sqrt(1 - esq * vl_pow(vl_sin(pos_lla->lat), 2)) );
	pos_lla->alt = p / vl_cos(pos_lla->lat) - N;
	
	return;
}

inline void trj_lla_to_ecef(trj_pos_ecef_t *pos_ecef, trj_pos_lla_t *pos_lla)
{
	vlf_t rad_lat = pos_lla->lat;
	vlf_t rad_lon = pos_lla->lon;
	
	const vlf_t a = 6378137.0;
	const vlf_t finv = 298.257223563;
	const vlf_t f = 1 / finv;
	const vlf_t e2 = 1 - (1 - f) * (1 - f);
	vlf_t v = a / vl_sqrt(1 - e2 * vl_sin(rad_lat) * vl_sin(rad_lat));
	
	vlf_t xyz[3];
	
	xyz[0] = (v + pos_lla->alt) * vl_cos(rad_lat) * vl_cos(rad_lon);
	xyz[2] = -1 * (v + pos_lla->alt) * vl_cos(rad_lat) * vl_sin(rad_lon);
	xyz[1] = (v * (1 - e2) + pos_lla->alt) * vl_sin(rad_lat);
	
	vl_vcopy(pos_ecef->vec, xyz);
	
	return;
}

//------------------------------------------------------------------------------


// void ecef_to_ctn()
// {
//     y = pos_ecef.vec;
//     y = y / scipy.linalg.norm(y);

//     x = lla_to_ecef(pos_lla(0.5 * math.pi, 0, 1000)).vec - pos_ecef.vec;
//     x = x / scipy.linalg.norm(x);
//     x = x - y * numpy.dot(x, y);
//     x = x / scipy.linalg.norm(x);

//     z = numpy.cross(x, y);
//     z = z / scipy.linalg.norm(z);

//     c_tn_ = numpy.mat([x, y, z]).dot(rot);
// }

inline void trj_ctn_to_hpr(s_trj_rot_hpr *rot_hpr, vlf_t *c_tn)
{
	vlf_t c_tf[9] =
			{
					1,  0, 0,
					0,  0, 1,
					0, -1, 0
			};
	
	vlf_t c_tf_tnp[9];
	vl_tnp(c_tf_tnp, c_tf);
	
	vlf_t temp_mat_0[9];
	vlf_t temp_mat_1[9];
	
	// c_tn = c_tf.T.dot(c_tn_).dot(c_tf);
	vl_mmul_m(temp_mat_0, c_tn, c_tf);
	vl_mmul_m(temp_mat_1, c_tf_tnp, temp_mat_0);
	
	rot_hpr->heading = -vl_atan2( temp_mat_1[1*3+0], temp_mat_1[0*3+0]);
	rot_hpr->pitch   = -vl_atan2(-temp_mat_1[2*3+0], vl_sqrt(temp_mat_1[2*3+1]*temp_mat_1[2*3+1] + temp_mat_1[2*3+2]*temp_mat_1[2*3+2]));
	rot_hpr->roll    =  vl_atan2( temp_mat_1[2*3+1], temp_mat_1[2*3+2]);
	
	if (rot_hpr->heading < 0)
	{
		rot_hpr->heading = rot_hpr->heading + vl_2pi;
	}
	
	if (rot_hpr->heading >= vl_2pi)
	{
		rot_hpr->heading = rot_hpr->heading - vl_2pi;
	}
	
	return;
}

//------------------------------------------------------------------------------

//inline void trj_pos_ecef(trj_pos_ecef_t *ecef, s_trj_obj *obj)
//{
//	vlf_t temp_vec[3];
//	vlf_t rot_inv[9];
//
//	vl_tnp(rot_inv, obj->ref->rot[0]);
//
//	vl_vsub(temp_vec, obj->pos[0], obj->ref->pos[0]);
//	vl_mmul_v(ecef->vec, rot_inv, temp_vec);
//
//	return;
//}

//------------------------------------------------------------------------------

inline void trj_hpr_to_ctn(vlf_t *c_tn, s_trj_rot_hpr rot_hpr)
{
	vlf_t a = -rot_hpr.heading;
	vlf_t b = -rot_hpr.pitch;
	vlf_t g = rot_hpr.roll;
	
	c_tn[0] = vl_cos(a)*vl_cos(b);
	c_tn[1] = vl_cos(a)*vl_sin(b)*vl_sin(g) - vl_sin(a)*vl_cos(g);
	c_tn[2] = vl_cos(a)*vl_sin(b)*vl_cos(g) + vl_sin(a)*vl_sin(g);
	c_tn[3] = vl_sin(a)*vl_cos(b);
	c_tn[4] = vl_sin(a)*vl_sin(b)*vl_sin(g) + vl_cos(a)*vl_cos(g);
	c_tn[5] = vl_sin(a)*vl_sin(b)*vl_cos(g) - vl_cos(a)*vl_sin(g);
	c_tn[6] = -vl_sin(b);
	c_tn[7] = vl_cos(b)*vl_sin(g);
	c_tn[8] = vl_cos(b)*vl_cos(g);
	
	vlf_t c_tf[9] =
			{
					1,  0, 0,
					0,  0, 1,
					0, -1, 0
			};
	
	vlf_t c_tf_tnp[9];
	vl_tnp(c_tf_tnp, c_tf);
	
	vlf_t temp_mat[9];
	
	// c_tn = c_tf * c_tn * c_tf.I;
	
	vl_mmul_m(temp_mat, c_tn, c_tf_tnp);
	vl_mmul_m(c_tn, c_tf, temp_mat);
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_UTILS__ */
