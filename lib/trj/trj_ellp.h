
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_ELLP__
#define __TRJ_ELLP__

//------------------------------------------------------------------------------

#include "vl.h"
#include "trj_utils.h"
#include "trj_api.h"

//------------------------------------------------------------------------------

static s_trj_ellp trj_ellp_wgs84 = {
		
		.id = 0x01,
		.a 		=  6.37813700000000000000e+0006,
		.b 		=  6.35675231424517949745e+0006,
		.e  	=  8.18191908426214947083e-0002,
		.f  	=  3.35281066474748071998e-0003,
};

//------------------------------------------------------------------------------

inline void trj_ellp_init(s_trj_ellp *ellp)
{
	ellp->ee		= ellp->e * ellp->e;
	ellp->invaa		= 1.0 / (ellp->a * ellp->a);
	ellp->l		 	= (ellp->e * ellp->e) / 2.0;
	ellp->p1mee		= 1.0 - (ellp->e * ellp->e);
	ellp->p1meedaa	= (1.0 - (ellp->e * ellp->e)) / (ellp->a * ellp->a);
	ellp->Hmin		= (ellp->e * ellp->e * ellp->e * ellp->e * ellp->e * ellp->e
					  *ellp->e * ellp->e * ellp->e * ellp->e * ellp->e * ellp->e) / 4.0;
	ellp->ll4		= 4.0 * (ellp->l * ellp->l);
	ellp->ll		= ellp->l * ellp->l;
	ellp->invcbrt2	= 1.0 / (vl_pow(2.0, 1.0 / 3.0));
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_ellp_ecef(s_trj_ellp *ellp, vlf_t *ecef, vlf_t *lla)
{
	vlf_t n = ellp->a / vl_sqrt(1 - ellp->ee*vl_sin(lla[0])*vl_sin(lla[0]));
	
	ecef[0] = (n + lla[2] ) * vl_cos(lla[0]) * vl_cos(lla[1]);
	ecef[1] = (n*(1-ellp->ee) + lla[2]) * vl_sin(lla[0]);
	ecef[2] =-(n + lla[2] ) * vl_cos(lla[0]) * vl_sin(lla[1]);
	
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

//------------------------------------------------------------------------------

#endif /* __TRJ_ELLP__ */
