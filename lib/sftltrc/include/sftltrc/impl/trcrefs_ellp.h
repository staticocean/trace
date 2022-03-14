
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrefs_ellp - ellipsoid refernce frame

#ifndef __TRCREFS_ELLP__
#define __TRCREFS_ELLP__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcrefs.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_ellp
{
	t_f64 				a;
	t_f64 				b;
	t_f64 				c;
	t_f64 				e;
	t_f64 				f;
	
	t_f64 				ee;
	t_f64 				invaa;
	t_f64 				l;
	t_f64 				p1mee;
	t_f64 				p1meedaa;
	t_f64 				Hmin;
	t_f64 				ll4;
	t_f64 				ll;
	t_f64 				invcbrt2;

}	s_trcrefs_ellp;

typedef struct trcrefs_ellp_attr
{
	t_f64 				a;
	t_f64 				f;

}	s_trcrefs_ellp_attr;

//------------------------------------------------------------------------------

void trcrefs_ellp_init (s_trcrefs_ellp *ellp, s_trcrefs_ellp_attr *attr)
{
    ellp->a         = attr->a;
    ellp->f         = attr->f;
	ellp->b         = ellp->a * (1 - ellp->f);
	ellp->ee		= 1 - ellp->b*ellp->b / (ellp->a*ellp->a );
	ellp->e 		= vld_sqrt(ellp->ee);
	ellp->invaa		= 1.0 / (ellp->a * ellp->a);
	ellp->l		 	= (ellp->e * ellp->e) / 2.0;
	ellp->p1mee		= 1.0 - (ellp->e * ellp->e);
	ellp->p1meedaa	= (1.0 - (ellp->e * ellp->e)) / (ellp->a * ellp->a);
	ellp->Hmin		= (ellp->e * ellp->e * ellp->e * ellp->e * ellp->e * ellp->e
					  *ellp->e * ellp->e * ellp->e * ellp->e * ellp->e * ellp->e) / 4.0;
	ellp->ll4		= 4.0 * (ellp->l * ellp->l);
	ellp->ll		= ellp->l * ellp->l;
	ellp->invcbrt2	= 1.0 / (vld_pow(2.0, 1.0 / 3.0));
}

//------------------------------------------------------------------------------

void trcrefs_ellp_ecef (s_trcrefs_ellp *ellp, t_f64 *ecef, t_f64 *lla)
{
	t_f64 n = ellp->a / vld_sqrt(1 - ellp->ee * vld_sin(lla[0]) * vld_sin(lla[0]));
	
	if (ecef != lla)
	{
		ecef[0] = (n + lla[2]) * vld_cos(lla[0]) * vld_cos(lla[1]);
		ecef[1] = (n * (1 - ellp->ee) + lla[2]) * vld_sin(lla[0]);
		ecef[2] = -(n + lla[2]) * vld_cos(lla[0]) * vld_sin(lla[1]);
	}
	
	else
	{
		t_f64 lla_[3] = { lla[0], lla[1], lla[2] };
		
		ecef[0] = (n + lla_[2]) * vld_cos(lla_[0]) * vld_cos(lla_[1]);
		ecef[1] = (n * (1 - ellp->ee) + lla_[2]) * vld_sin(lla_[0]);
		ecef[2] = -(n + lla_[2]) * vld_cos(lla_[0]) * vld_sin(lla_[1]);
	}
}

//------------------------------------------------------------------------------

t_s8 trcrefs_ellp_lla (s_trcrefs_ellp *ellp, t_f64 *lla, t_f64 *ecef)
{
	const t_f64 inv3 = +3.33333333333333333333e-0001;
	const t_f64 inv6 = +1.66666666666666666667e-0001;
	
	t_f64 x, y, z;
	// The variables below correspond to symbols used in the paper
	// "Accurate Conversion of Earth-Centered, Earth-Fixed Coordinates
	// to Geodetic Coordinates"
	t_f64 beta;
	t_f64 C;
	t_f64 dFdt;
	t_f64 dt;
	t_f64 dw;
	t_f64 dz;
	t_f64 F;
	t_f64 G;
	t_f64 H;
	t_f64 i;
	t_f64 k;
	t_f64 m;
	t_f64 n;
	t_f64 p;
	t_f64 P;
	t_f64 t;
	t_f64 u;
	t_f64 v;
	t_f64 w;
	// Intermediate variables
	t_f64 j;
	t_f64 ww; // w^2
	t_f64 mpn; // m+n
	t_f64 g;
	t_f64 tt; // t^2
	t_f64 ttt; // t^3
	t_f64 tttt; // t^4
	t_f64 zu; // z * u
	t_f64 wv; // w * v
	t_f64 invuv; // 1 / (u * v)
	t_f64 da;
	t_f64 t1, t2, t3, t4, t5, t6, t7;
	
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

t_s8 trcrefs_ellp_glla (s_trcrefs_ellp *ellp, t_f64 *lla, t_f64 *ecef)
{
    trcrefs_ellp_lla(ellp, lla, ecef);

    t_f64 rn = ellp->a / vld_sqrt(1 - ellp->e*ellp->e*vld_sin(lla[0])*vld_sin(lla[0]));
    lla[0] = atan(1 - ellp->ee * rn / (rn + lla[2]));

    return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trcrefs_ellp_ecefrot (s_trcrefs_ellp *ellp, t_f64 *ecef, t_f64 *c_tn)
{
	t_f64 ctn_tnp[9];
	
	t_f64 *x = &ctn_tnp[0];
	t_f64 *y = &ctn_tnp[3];
	t_f64 *z = &ctn_tnp[6];
	
	t_f64 lla[3];
	trcrefs_ellp_lla(ellp, lla, ecef);
	y[0] =  cos(lla[1]) * cos(lla[0]);
	y[2] = -sin(lla[1]) * cos(lla[0]);
	y[1] =  sin(lla[0]);
	
	t_f64 north[3] = { 0.0, ellp->a, 0.0 };
	vld3v_subv(x, north, ecef);
	vld3v_muls(x, x, 1.0 / vld3v_norm(x));
	
	t_f64 xy[3];
	vld3v_muls(xy, y, vld3v_dot(x, y));
	vld3v_subv(x, x, xy);
	vld3v_muls(x, x, 1.0 / vld3v_norm(x));
	
	vld3v_cross(z, x, y);
	vld3v_muls(z, z, 1.0 / vld3v_norm(z));
	
	vld3m_tnp(c_tn, ctn_tnp);
	
	return 0x00;
}

//------------------------------------------------------------------------------

void trcrefs_ellp_nwhvel (s_trcrefs_ellp *ellp, t_f64 *lla, t_f64 *vel, t_f64 *nwh)
{
	t_f64 sin_lat = vld_sin(lla[0]);
	t_f64 temp = 1 - ellp->ee * sin_lat*sin_lat;
	
	t_f64 M = ellp->a * ellp->p1mee / vld_pow(temp, 1.5);
	t_f64 N = ellp->a / vld_pow(temp, 0.5);
	
	nwh[0] = vel[0] * M;
	nwh[1] = vel[1] * N * vld_cos(lla[0]);
	nwh[2] = vel[2];
}

//------------------------------------------------------------------------------

static s_trcref trcrefs_wgs84 = {
		.desc   = "wgs84",
		.a 		=  6.37813700000000000000e+0006,
		.f  	=  3.35281066474748071998e-0003,
		};

static s_trcref trcrefs_pz90 = {
		.desc   = "pz90",
		.a 		=  6378136,
		.f  	=  3.35280374301947673491022158624e-0003,
		};

static s_trcref trcrefs_pz90_11 = {
		.desc   = "pz90_11",
		.a 		=  6378136,
		.f  	=  0.0033528037345079697573169141833205,
		};

//------------------------------------------------------------------------------

#endif /* __TRCREFS_ELLP__ */
