
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrefs_ellp - ellipsoid refernce frame

#ifndef __TRCREFS_ELLP__
#define __TRCREFS_ELLP__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vl.h>

#include <sftlrefs/trcrefs.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_ellp
{
	s_trcrefs_ellp 		super;
	
	f64_t 				a;
	f64_t 				b;
	f64_t 				c;
	f64_t 				e;
	f64_t 				f;
	
	f64_t 				ee;
	f64_t 				invaa;
	f64_t 				l;
	f64_t 				p1mee;
	f64_t 				p1meedaa;
	f64_t 				Hmin;
	f64_t 				ll4;
	f64_t 				ll;
	f64_t 				invcbrt2;

}	s_trcrefs_ellp;

typedef struct trcrefs_ellp_attr
{
	s_trcrefs_ellp_attr super;
	
	f64_t 				a;
	f64_t 				f;

}	s_trcrefs_ellp_attr;

//------------------------------------------------------------------------------

void trcrefs_init (s_trcref *ellp)
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
	
	ellp->hash 		= crc32_iso_str(ellp->desc);
}

//------------------------------------------------------------------------------

void trcrefs_ecef (s_trcref *ellp, f64_t *ecef, f64_t *lla)
{
	f64_t n = ellp->a / vl_sqrt(1 - ellp->ee * vl_sin(lla[0]) * vl_sin(lla[0]));
	
	if (ecef != lla)
	{
		ecef[0] = (n + lla[2]) * vl_cos(lla[0]) * vl_cos(lla[1]);
		ecef[1] = (n * (1 - ellp->ee) + lla[2]) * vl_sin(lla[0]);
		ecef[2] = -(n + lla[2]) * vl_cos(lla[0]) * vl_sin(lla[1]);
	}
	
	else
	{
		f64_t lla_[3] = { lla[0], lla[1], lla[2] };
		
		ecef[0] = (n + lla_[2]) * vl_cos(lla_[0]) * vl_cos(lla_[1]);
		ecef[1] = (n * (1 - ellp->ee) + lla_[2]) * vl_sin(lla_[0]);
		ecef[2] = -(n + lla_[2]) * vl_cos(lla_[0]) * vl_sin(lla_[1]);
	}
}

//------------------------------------------------------------------------------

s8_t trcrefs_lla (s_trcref *ellp, f64_t *lla, f64_t *ecef)
{
	const f64_t inv3 = +3.33333333333333333333e-0001;
	const f64_t inv6 = +1.66666666666666666667e-0001;
	
	f64_t x, y, z;
	// The variables below correspond to symbols used in the paper
	// "Accurate Conversion of Earth-Centered, Earth-Fixed Coordinates
	// to Geodetic Coordinates"
	f64_t beta;
	f64_t C;
	f64_t dFdt;
	f64_t dt;
	f64_t dw;
	f64_t dz;
	f64_t F;
	f64_t G;
	f64_t H;
	f64_t i;
	f64_t k;
	f64_t m;
	f64_t n;
	f64_t p;
	f64_t P;
	f64_t t;
	f64_t u;
	f64_t v;
	f64_t w;
	// Intermediate variables
	f64_t j;
	f64_t ww; // w^2
	f64_t mpn; // m+n
	f64_t g;
	f64_t tt; // t^2
	f64_t ttt; // t^3
	f64_t tttt; // t^4
	f64_t zu; // z * u
	f64_t wv; // w * v
	f64_t invuv; // 1 / (u * v)
	f64_t da;
	f64_t t1, t2, t3, t4, t5, t6, t7;
	
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

s8_t trcrefs_glla (s_trcref *ellp, f64_t *lla, f64_t *ecef)
{
    trcrefs_lla(ellp, lla, ecef);

    f64_t rn = ellp->a / vl_sqrt(1 - ellp->e*ellp->e*vl_sin(lla[0])*vl_sin(lla[0]));
    lla[0] = atan(1 - ellp->ee * rn / (rn + lla[2]));

    return 0x00;
}

//------------------------------------------------------------------------------

s8_t trcrefs_ecefrot (s_trcref *ellp, f64_t *ecef, f64_t *c_tn)
{
	f64_t ctn_tnp[9];
	
	f64_t *x = &ctn_tnp[0];
	f64_t *y = &ctn_tnp[3];
	f64_t *z = &ctn_tnp[6];
	
	f64_t lla[3];
	trcrefs_lla(ellp, lla, ecef);
	y[0] =  cos(lla[1]) * cos(lla[0]);
	y[2] = -sin(lla[1]) * cos(lla[0]);
	y[1] =  sin(lla[0]);
	
	f64_t north[3] = { 0.0, ellp->a, 0.0 };
	vl3v_subv(x, north, ecef);
	vl3v_muls(x, x, 1.0 / vl3v_norm(x));
	
	f64_t xy[3];
	vl3v_muls(xy, y, vl3v_dot(x, y));
	vl3v_subv(x, x, xy);
	vl3v_muls(x, x, 1.0 / vl3v_norm(x));
	
	vl3v_cross(z, x, y);
	vl3v_muls(z, z, 1.0 / vl3v_norm(z));
	
	vl3m_tnp(c_tn, ctn_tnp);
	
	return 0x00;
}

//------------------------------------------------------------------------------

void trcrefs_nwhvel (s_trcref *ellp, f64_t *lla, f64_t *vel, f64_t *nwh)
{
	f64_t sin_lat = vl_sin(lla[0]);
	f64_t temp = 1 - ellp->ee * sin_lat*sin_lat;
	
	f64_t M = ellp->a * ellp->p1mee / vl_pow(temp, 1.5);
	f64_t N = ellp->a / vl_pow(temp, 0.5);
	
	nwh[0] = vel[0] * M;
	nwh[1] = vel[1] * N * vl_cos(lla[0]);
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
