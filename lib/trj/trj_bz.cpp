
#include "trj_bz.h"

//------------------------------------------------------------------------------

uint8_t trj_bz_init(s_trj_bz *self, s_trj_bz_init attr)
{
	uint32_t i;
	
	for (i = 0; i < 2; ++i)
	{
		self->p0[i] = attr.p0[i];
		self->p1[i] = attr.p1[i];
		self->p2[i] = attr.p2[i];
	}
	
	return 0x00;
}

uint8_t trj_bz_deriv(s_trj_bz *self, vlf_t t, vlf_t *deriv)
{
	vlf_t temp = 1 - t;
	
	deriv[0] =(2*temp*(self->p1[0] - self->p0[0])
			 + 2*t*(self->p2[0] - self->p1[0]));
	
	return 0x00;
}

uint8_t trj_bz_eval(s_trj_bz *self, vlf_t t, vlf_t *value)
{
	vlf_t a = self->p0[0] - 2 * self->p1[0] + self->p2[0];
	vlf_t b = -2 * (self->p0[0] - self->p1[0]);
	vlf_t c = self->p0[0] - t;
	
	vlf_t sol1 = 0.0;
	vlf_t sol2 = 0.0;
	
	if (fabs(a) > 0)
	{
		// calculate the discriminant
		vlf_t d = (b*b) - (4 * a * c);
		
		// find two solutions
		
		if (d > 0)
		{
			sol1 = (-b - sqrt(d)) / (2 * a);
			sol2 = (-b + sqrt(d)) / (2 * a);
		}
	}
	else
	{
		sol1 = -c / b;
		sol2 = -c / b;
	}
	
	vlf_t x = 0.0;
	
	if (sol1 < 0 || sol1 > 1) x = sol2;
	else if (sol2 < 0 || sol2 > 1) x = sol1;
	else if (sol1 >= sol2) x = sol1;
	else if (sol2 >= sol1) x = sol2;
	
	x = float(x);
	
	vlf_t temp = (1 - x);
	
	*value = (temp * temp * self->p0[1] + 2 * temp * x * self->p1[1] + x * x * self->p2[1]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_bz4_init(s_trj_bz4 *self, s_trj_bz4_init_attr attr)
{
	uint32_t i;
	
	for (i = 0; i < 2; ++i)
	{
		self->p0[i] = attr.p0[i];
		self->p1[i] = attr.p1[i];
		self->p2[i] = attr.p2[i];
		self->p3[i] = attr.p3[i];
	}
	
	return 0x00;
}

uint8_t trj_bz4_d0(s_trj_bz4 *self, vlf_t t, vlf_t *d0)
{
	vlf_t t_ = 1 - t;
	
	d0[0] = t_*t_*t_*self->p0[0] + 3*t_*t_*t*self->p1[0]
			+ 3*t_*t*t*self->p2[0] + t*t*t*self->p3[0];
	
	d0[1] = t_*t_*t_*self->p0[1] + 3*t_*t_*t*self->p1[1]
			+ 3*t_*t*t*self->p2[1] + t*t*t*self->p3[1];
	
	return 0x00;
}

uint8_t trj_bz4_d1(s_trj_bz4 *self, vlf_t t, vlf_t *d1)
{
	vlf_t t_ = 1 - t;
	
	d1[0] =   3 * t_*t_*(self->p1[0] - self->p0[0])
			+ 6 * t_*t *(self->p2[0] - self->p1[0])
			+ 3 * t *t *(self->p3[0] - self->p2[0]);
	
	d1[1] =   3 * t_*t_* (self->p1[1] - self->p0[1])
			+ 6 * t_*t * (self->p2[1] - self->p1[1])
			+ 3 * t *t * (self->p3[1] - self->p2[1]);
	
	return 0x00;
}

uint8_t trj_bz4_eval(s_trj_bz4 *self, vlf_t x, vlf_t *value)
{
	vlf_t roots[4];
	uint8_t root_count = 0x00;
	
	vlf_t pa3 = 3 * self->p0[0];
	vlf_t pb3 = 3 * self->p1[0];
	vlf_t pc3 = 3 * self->p2[0];
	vlf_t a = -self->p0[0]  +   pb3 - pc3 + self->p3[0];
	vlf_t b =  pa3 - 2*pb3 + pc3;
	vlf_t c = -pa3 +   pb3;
	vlf_t d =  self->p0[0] - x;
	// Fun fact: any Bezier curve may (accidentally or on purpose)
	// perfectly model any lower order curve, so we want to test
	// for that: lower order curves are much easier to root-find.
	
	if (fabs(a) <= 1E-9) {
		// this is not a cubic curve.
		if (fabs(b) <= 1E-9) {
			// in fact, this is not a quadratic curve either.
			if (fabs(c) <= 1E-9) {
				// in fact in fact, there are no solutions.
				return 0x01;
			}
			
			root_count = 0x01;
			roots[0] = -d / c;
		}
		else
		{
			// quadratic solution:
			vlf_t q = vl_sqrt(c * c - 4 * b * d);
			vlf_t b2 = 2 * b;
			
			root_count = 0x02;
			roots[0] = (q - c) / b2;
			roots[1] = (-c - q) / b2;
		}
	}
	
	else
	{
		b /= a;
		c /= a;
		d /= a;
		
		vlf_t b3 = b / 3;
		vlf_t p = (3 * c - b * b) / 3;
		vlf_t p3 = p / 3;
		vlf_t q = (2 * b * b * b - 9 * b * c + 27 * d) / 27;
		vlf_t q2 = q / 2;
		vlf_t discriminant = q2 * q2 + p3 * p3 * p3;
		vlf_t u1, v1;
		
		// case 1: three real roots, but finding them involves complex
		// maths. Since we don't have a complex data type, we use trig
		// instead, because complex numbers have nice geometric properties.
		if (discriminant < 0)
		{
			vlf_t mp3 = -p / 3;
			vlf_t r = vl_sqrt(mp3 * mp3 * mp3);
			vlf_t t = -q / (2 * r);
			vlf_t cosphi = t < -1 ? -1 : t > 1 ? 1 : t;
			vlf_t phi = acos(cosphi);
			vlf_t crtr = vl_crt(r);
			vlf_t t1 = 2 * crtr;
			
			root_count = 3;
			
			roots[0] = t1 * cos(phi / 3) - b3;
			roots[1] = t1 * cos((phi + vl_2pi) / 3) - b3;
			roots[2] = t1 * cos((phi + 2 * vl_2pi) / 3) - b3;
		}
			
			// case 2: three real roots, but two form a "double root",
			// and so will have the same resultant value. We only need
			// to return two values in this case.
		else if (discriminant == 0)
		{
			u1 = q2 < 0 ? vl_crt(-q2) : -vl_crt(q2);
			
			root_count = 2;
			
			roots[0] = 2 * u1 - b3;
			roots[1] = -u1 - b3;
		}
			
			// case 3: one real root, 2 complex roots. We don't care about
			// complex results so we just ignore those and directly compute
			// that single real root.
		else
		{
			vlf_t sd = sqrt(discriminant);
			u1 = vl_crt(-q2 + sd);
			v1 = vl_crt(q2 + sd);
			
			root_count = 0x01;
			roots[0] = u1 - v1 - b3;
		}
	}
	
	vlf_t t = 0.0;
	
	for (int i = 0; i < root_count; ++i)
	{
		if (roots[i] < 0 || roots[i] > 1) continue;
		else t = roots[i];
		break;
	}
	
	vlf_t temp[2];
	
	trj_bz4_d0(self, t, temp);
	*value = temp[1];
	
	return 0x00;
}

//------------------------------------------------------------------------------

