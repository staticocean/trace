
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright ? 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_ELLP__
#define __TRJ_ELLP__

//------------------------------------------------------------------------------

#include "vl.h"
#include "trj_utils.h"
#include "trj_api.h"

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__

static s_trj_ellp trj_ellp_wgs84;
trj_ellp_wgs84.desc   	= "wgs84";
trj_ellp_wgs84.a 		=  6.37813700000000000000e+0006;
trj_ellp_wgs84.f  		=  3.35281066474748071998e-0003;

#else

static s_trj_ellp trj_ellp_wgs84 = {
		
		.desc   = "wgs84",
		
		.a 		=  6.37813700000000000000e+0006,
		.f  	=  3.35281066474748071998e-0003,
};

#endif


#ifdef __TRJ_ENV__

static s_trj_ellp trj_ellp_pz90;
trj_ellp_wgs84.desc   	= "pz90";
trj_ellp_wgs84.a 		=  6378136;
trj_ellp_wgs84.f  		=  3.35280374301947673491022158624e-0003;

#else

static s_trj_ellp trj_ellp_pz90 = {
		
		.desc   = "pz90",
		
		.a 		=  6378136,
		.f  	=  3.35280374301947673491022158624e-0003,
};

#endif

//------------------------------------------------------------------------------

inline void trj_ellp_init(s_trj_ellp *ellp)
{
	ellp->b         = ellp->a * (1 - ellp->f);
	ellp->ee		= 1 - ellp->b*ellp->b / (ellp->a*ellp->a );
	ellp->e 		= vl_sqrt(ellp->ee);
	ellp->invaa		= 1.0 / (ellp->a * ellp->a);
	ellp->l		 	= (ellp->e * ellp->e) / 2.0;
	ellp->p1mee		= 1.0 - (ellp->e * ellp->e);
	ellp->p1meedaa	= (1.0 - (ellp->e * ellp->e)) / (ellp->a * ellp->a);
	ellp->Hmin		= (ellp->e * ellp->e * ellp->e * ellp->e * ellp->e * ellp->e
					  *ellp->e * ellp->e * ellp->e * ellp->e * ellp->e * ellp->e) / 4.0;
	ellp->ll4		= 4.0 * (ellp->l * ellp->l);
	ellp->ll		= ellp->l * ellp->l;
	ellp->invcbrt2	= 1.0 / (vl_pow(2.0, 1.0 / 3.0));
	
	ellp->hash 		= vl_crc32(ellp->desc);
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_ellp_ecef(s_trj_ellp *ellp, vlf_t *ecef, vlf_t *lla)
{
	vlf_t n = ellp->a / vl_sqrt(1 - ellp->ee * vl_sin(lla[0]) * vl_sin(lla[0]));
	
	if (ecef != lla)
	{
		ecef[0] = (n + lla[2]) * vl_cos(lla[0]) * vl_cos(lla[1]);
		ecef[1] = (n * (1 - ellp->ee) + lla[2]) * vl_sin(lla[0]);
		ecef[2] = -(n + lla[2]) * vl_cos(lla[0]) * vl_sin(lla[1]);
	}
	
	else
	{
		vlf_t lla_[3] = { lla[0], lla[1], lla[2] };
		
		ecef[0] = (n + lla_[2]) * vl_cos(lla_[0]) * vl_cos(lla_[1]);
		ecef[1] = (n * (1 - ellp->ee) + lla_[2]) * vl_sin(lla_[0]);
		ecef[2] = -(n + lla_[2]) * vl_cos(lla_[0]) * vl_sin(lla_[1]);
	}
	
	return;
}

//------------------------------------------------------------------------------

inline uint8_t trj_ellp_lla(s_trj_ellp *ellp, vlf_t *lla, vlf_t *ecef)
{
	const vlf_t inv3 = +3.33333333333333333333e-0001;
	const vlf_t inv6 = +1.66666666666666666667e-0001;
	
	vlf_t x, y, z;
	// The variables below correspond to symbols used in the paper
	// "Accurate Conversion of Earth-Centered, Earth-Fixed Coordinates
	// to Geodetic Coordinates"
	vlf_t beta;
	vlf_t C;
	vlf_t dFdt;
	vlf_t dt;
	vlf_t dw;
	vlf_t dz;
	vlf_t F;
	vlf_t G;
	vlf_t H;
	vlf_t i;
	vlf_t k;
	vlf_t m;
	vlf_t n;
	vlf_t p;
	vlf_t P;
	vlf_t t;
	vlf_t u;
	vlf_t v;
	vlf_t w;
	// Intermediate variables
	vlf_t j;
	vlf_t ww; // w^2
	vlf_t mpn; // m+n
	vlf_t g;
	vlf_t tt; // t^2
	vlf_t ttt; // t^3
	vlf_t tttt; // t^4
	vlf_t zu; // z * u
	vlf_t wv; // w * v
	vlf_t invuv; // 1 / (u * v)
	vlf_t da;
	vlf_t t1, t2, t3, t4, t5, t6, t7;
	
	x =  ecef[0];
	y = -ecef[2];
	z =  ecef[1];
	
	ww = x * x + y * y;
	m = ww * ellp->invaa;
	n = z * z * ellp->p1meedaa;
	mpn = m + n;
	p = inv6 * (mpn - ellp->ll4);
	G = m * n * ellp->ll;
	H = 2 * p * p * p + G;
	
	if (H < ellp->Hmin)
	{
		return 0x01;
	}
	
	C = pow(H + G + 2 * sqrt(H * G), inv3) * ellp->invcbrt2;
	i = -ellp->ll - 0.5 * mpn;
	P = p * p;
	beta = inv3 * i - C - P / C;
	k = ellp->ll * (ellp->ll - mpn);
	// Compute left part of t
	t1 = beta * beta - k;
	t2 = sqrt(t1);
	t3 = t2 - 0.5 * (beta + i);
	t4 = sqrt(t3);
	// Compute right part of t
	t5 = 0.5 * (beta - i);
	// t5 may accidentally drop just below zero due to numeric turbulence
	// This only occurs at latitudes close to +- 45.3 degrees
	t5 = fabs(t5);
	t6 = sqrt(t5);
	t7 = (m < n) ? t6 : -t6;
	// Add left and right parts
	t = t4 + t7;
	// Use Newton-Raphson's method to compute t correction
	j = ellp->l * (m - n);
	g = 2 * j;
	tt = t * t;
	ttt = tt * t;
	tttt = tt * tt;
	F = tttt + 2 * i * tt + g * t + k;
	dFdt = 4 * ttt + 4 * i * t + g;
	dt = -F / dFdt;
	// compute latitude (range -PI/2..PI/2)
	u = t + dt + ellp->l;
	v = t + dt - ellp->l;
	w = sqrt(ww);
	zu = z * u;
	wv = w * v;
	lla[0] = atan2(zu, wv);
	// compute altitude
	invuv = 1 / (u * v);
	dw = w - wv * invuv;
	dz = z - zu * ellp->p1mee * invuv;
	da = sqrt(dw * dw + dz * dz);
	lla[2] = (u < 1) ? -da : da;
	// compute longitude (range -PI..PI)
	lla[1] = atan2(y, x);
	
	return 0x00;
}

inline uint8_t trj_ellp_glla(s_trj_ellp *ellp, vlf_t *lla, vlf_t *ecef)
{
    trj_ellp_lla(ellp, lla, ecef);

    vlf_t rn = ellp->a / vl_sqrt(1 - ellp->e*ellp->e*vl_sin(lla[0])*vl_sin(lla[0]));
    lla[0] = atan(1 - ellp->ee * rn / (rn + lla[2]));

    return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_ellp_ecefrot(s_trj_ellp *self, vlf_t *ecef, vlf_t *c_tn)
{
	vlf_t ctn_tnp[9];
	
	vlf_t *x = &ctn_tnp[0];
	vlf_t *y = &ctn_tnp[3];
	vlf_t *z = &ctn_tnp[6];
	
	vlf_t lla[3];
	trj_ellp_lla(self, lla, ecef);
	y[0] =  cos(lla[1]) * cos(lla[0]);
	y[2] = -sin(lla[1]) * cos(lla[0]);
	y[1] = sin(lla[0]);
	
	vlf_t north[3] = { 0.0, self->a, 0.0 };
	vl_vsub(x, north, ecef);
	vl_vmul_s(x, x, 1.0 / vl_vnorm(x));
	
	vlf_t xy[3];
	vl_vmul_s(xy, y, vl_vdot(x, y));
	vl_vsub(x, x, xy);
	vl_vmul_s(x, x, 1.0 / vl_vnorm(x));
	
	vl_cross(z, x, y);
	vl_vmul_s(z, z, 1.0 / vl_vnorm(z));
	
	vl_tnp(c_tn, ctn_tnp);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline void trj_ellp_nwhvel(s_trj_ellp *self, vlf_t *lla, vlf_t *vel, vlf_t *nwh)
{
	vlf_t sin_lat = vl_sin(lla[0]);
	vlf_t temp = 1 - self->ee * sin_lat*sin_lat;
	
	vlf_t M = self->a * self->p1mee / vl_pow(temp, 1.5);
	vlf_t N = self->a / vl_pow(temp, 0.5);
	
	nwh[0] = vel[0] * M;
	nwh[1] = vel[1] * N * vl_cos(lla[0]);
	nwh[2] = vel[2];
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_ELLP__ */
