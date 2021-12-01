
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/vl.h>
#include <sftlstd/bz.h>

#include "trc_api.h"
#include "trcellp.h"

//------------------------------------------------------------------------------

typedef struct trctraj_bz_point
{
	f64_t time;
	
	f64_t pos_p[3];
	f64_t pos_t[3];
	f64_t pos_d[3];
	
	f64_t rot_p[3];
	f64_t rot_t[3];
	f64_t rot_d[3];
	
//	f64_t offset[3];
	
	/*
	 pos_d MUST BE
	 to the right of pos_p (pos_d[0] > pos_p[0])
	 cannot overlap next point
	 */
	
} 	s_trctraj_bz_point;

typedef struct trctraj_bz
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	u32_t 	ref_hash;
	
	s_trctraj_bz_point *pts;
	u32_t 	pts_size;
	u32_t 	pts_offset;
	
	u8_t 	ellp_en;
	s_trcellp 	*ellp;
	u32_t 	ellp_hash;
//
//	f64_t 		p_bias[3];
//	f64_t 		p_gain[3];
//
//	f64_t 		t_bias;
//	f64_t 		t_gain;
	
} 	s_trctraj_bz;

typedef struct trctraj_bz_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
	u8_t ellp_en;
	s_trcellp *ellp;
	
	u32_t pts_size;
	s_trctraj_bz_point *pts;
	
} 	s_trctraj_bz_init;

//------------------------------------------------------------------------------

inline u8_t trctraj_bz_init(s_trctraj_bz *self, s_trctraj_bz_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->pts = attr.pts;
	self->pts_size = attr.pts_size;
	self->pts_offset = 0x00;
	
	self->ellp_en = attr.ellp_en;
	self->ellp = attr.ellp;
	if (self->ellp != NULL)
	{ self->ellp_hash = crc32_iso_str(self->ellp->desc); }
	
//	vl_vset(self->p_bias, 0.0);
//	vl_vset(self->p_gain, 1.0);
//
//	self->t_bias = 0.0;
//	self->t_gain = 1.0;
	
	return 0x00;
}

inline u8_t trctraj_bz_save(s_trctraj_bz *self, s_trctraj_bz_init *attr, u8_t **v_file)
{
	memcpy(*v_file, self->pts, self->pts_size * sizeof(s_trctraj_bz_point));
	*v_file += self->pts_size * sizeof(s_trctraj_bz_point);
	
	return 0x00;
}

inline u8_t trctraj_bz_load(s_trctraj_bz *self, s_trctraj_bz_init *attr, u8_t **v_file)
{
	self->pts = (s_trctraj_bz_point*) malloc(self->pts_size * sizeof(s_trctraj_bz_point));
	memcpy(self->pts, *v_file, self->pts_size * sizeof(s_trctraj_bz_point));

	*v_file += self->pts_size * sizeof(s_trctraj_bz_point);
	
	self->eng = attr->eng;
	
	self->ref  = trceng_find_obj (self->eng, self->ref_hash);
	self->ellp = trceng_find_ellp(self->eng, self->ellp_hash);
	
	if (self->ref  == NULL)
	{
		self->ref_hash = 0x00;
	}
	
	if (self->ellp == NULL)
	{
		self->ellp_hash = 0x00;
		self->ellp_en = 0x00;
	}
	
	return 0x00;
}

inline int trctraj_bz_point_comp (const void *p0_, const void *p1_)
{
	s_trctraj_bz_point *p0 = (s_trctraj_bz_point*) p0_;
	s_trctraj_bz_point *p1 = (s_trctraj_bz_point*) p1_;
	
	if (p0->time > p1->time) return +1;
	if (p0->time < p1->time) return -1;
	
	return 0;
}

inline u8_t trctraj_bz_add(s_trctraj_bz *self, s_trctraj_bz_point point)
{
	self->pts[self->pts_offset] = point;
	self->pts_offset++;
	
	qsort (self->pts, self->pts_offset, sizeof(s_trctraj_bz_point), trctraj_bz_point_comp);
	
	return 0x00;
}

inline u8_t trctraj_bz_rem(s_trctraj_bz *self, u32_t index)
{
	u32_t i;
	
	for (i = index; i < self->pts_offset-1; ++i)
	{
		self->pts[i] = self->pts[i+1];
	}
	
	--self->pts_offset;
	
	return 0x00;
}

inline u8_t trctraj_bz_vel(s_trctraj_bz *self, f64_t time, f64_t *vel)
{
	u32_t offset = 0x00;
	
	if (self->pts_offset == 0x00) { return 0x01; }
	
	if (time < self->pts[0x00].time + 1E-9)
	{
        vl3_vzero(vel);
	}

    else if (time > self->pts[self->pts_offset-1].time - 1E-9)
    {
        vl3_vzero(vel);
    }

	else
    {
        if (time > self->pts[self->pts_offset-1].time) time = self->pts[self->pts_offset-1].time;

        while ((time < self->pts[offset].time)
               || (time > self->pts[offset+1].time))
        {
            ++offset;
        }

        if (fabs(time-self->pts[offset].time) < 1E-9)
        {
            f64_t acc[3];

            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[0]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[0],
                            self->pts[offset].pos_p[0] + self->pts[offset].pos_d[0]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[0],
                            self->pts[offset + 1].pos_p[0] - self->pts[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[0]
                    }
            });

            bz4_d2t(&bz4, time, &acc[0]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[1]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[1],
                            self->pts[offset].pos_p[1] + self->pts[offset].pos_d[1]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[1],
                            self->pts[offset + 1].pos_p[1] - self->pts[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[1]
                    }
            });

            bz4_d2t(&bz4, time, &acc[1]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[2]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[2],
                            self->pts[offset].pos_p[2] + self->pts[offset].pos_d[2]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[2],
                            self->pts[offset + 1].pos_p[2] - self->pts[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[2]
                    }
            });

            bz4_d2t(&bz4, time, &acc[2]);

            vl3_vdiv_v(vel, self->pts[offset].pos_d, self->pts[offset].pos_t);
            vl3_vsumm(vel, vel, acc, time - self->pts[offset].time);
        }

        else if (fabs(time-self->pts[offset+1].time) < 1E-9)
        {
            f64_t acc[3];

            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[0]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[0],
                            self->pts[offset].pos_p[0] + self->pts[offset].pos_d[0]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[0],
                            self->pts[offset + 1].pos_p[0] - self->pts[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[0]
                    }
            });

            bz4_d2t(&bz4, time, &acc[0]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[1]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[1],
                            self->pts[offset].pos_p[1] + self->pts[offset].pos_d[1]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[1],
                            self->pts[offset + 1].pos_p[1] - self->pts[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[1]
                    }
            });

            bz4_d2t(&bz4, time, &acc[1]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[2]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[2],
                            self->pts[offset].pos_p[2] + self->pts[offset].pos_d[2]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[2],
                            self->pts[offset + 1].pos_p[2] - self->pts[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[2]
                    }
            });

            bz4_d2t(&bz4, time, &acc[2]);

            vl3_vdiv_v(vel, self->pts[offset+1].pos_d, self->pts[offset+1].pos_t);
            vl3_vsumm(vel, vel, acc, time - self->pts[offset+1].time);
        }

        else
        {
            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[0]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[0],
                            self->pts[offset].pos_p[0] + self->pts[offset].pos_d[0]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[0],
                            self->pts[offset + 1].pos_p[0] - self->pts[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[0]
                    }
            });

            bz4_d1t(&bz4, time, &vel[0]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[1]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[1],
                            self->pts[offset].pos_p[1] + self->pts[offset].pos_d[1]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[1],
                            self->pts[offset + 1].pos_p[1] - self->pts[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[1]
                    }
            });

            bz4_d1t(&bz4, time, &vel[1]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[2]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[2],
                            self->pts[offset].pos_p[2] + self->pts[offset].pos_d[2]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[2],
                            self->pts[offset + 1].pos_p[2] - self->pts[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[2]
                    }
            });

            bz4_d1t(&bz4, time, &vel[2]);
        }
    }

	if (self->ellp_en != 0x00 && self->ellp != NULL)
	{
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trctraj_bz_pos_raw(s_trctraj_bz *self, f64_t time, f64_t *pos)
{
	u32_t offset = 0x00;
	
	if (self->pts_offset == 0x00) { return 0x01; }

    if (time < self->pts[0x00].time + 1E-9)
    {
        vl3_vcopy(pos, self->pts[0x00].pos_p);
    }

    else if (time > self->pts[self->pts_offset-1].time - 1E-9)
    {
        vl3_vcopy(pos, self->pts[self->pts_offset-1].pos_p);
    }

    else
    {
        if (time > self->pts[self->pts_offset-1].time) { time = self->pts[self->pts_offset-1].time; }

        while ((time < self->pts[offset].time)
               || (time > self->pts[offset+1].time))
        {
            ++offset;
        }

    //
    //	else if (fabs(time - self->pts[offset].time) < 1E-9)
    //	{ vl_vcopy(pos, self->pts[offset].pos_p); }
    //
    //	else if (fabs(time - self->pts[offset+1].time) < 1E-9)
    //	{ vl_vcopy(pos, self->pts[offset+1].pos_p); }
    //
        if (fabs(time-self->pts[offset].time) < 1E-9)
        {
            f64_t vel[3];
            vl3_vcopy(pos, self->pts[offset].pos_p);
            vl3_vdiv_v(vel, self->pts[offset].pos_d, self->pts[offset].pos_t);
            vl3_vsumm(pos, pos, vel, time - self->pts[offset].time);
        }

        else if (fabs(time-self->pts[offset+1].time) < 1E-9)
        {
            f64_t vel[3];
            vl3_vcopy(pos, self->pts[offset+1].pos_p);
            vl3_vdiv_v(vel, self->pts[offset+1].pos_d, self->pts[offset+1].pos_t);
            vl3_vsumm(pos, pos, vel, time - self->pts[offset+1].time);
        }

        else
        {
    //		f64_t t = (time - self->pts[offset].time) / (self->pts[offset+1].time - self->pts[offset].time);
    //		f64_t p[2];

    //		vl_vsub(self->pts[offset].pos_p, self->pts[offset].pos_p, self->p_bias);
    //		vl_vsub(self->pts[offset+1].pos_p, self->pts[offset+1].pos_p, self->p_bias);
    //		self->pts[offset].time -= self->t_bias;
    //		self->pts[offset+1].time -= self->t_bias;
    //
    //		vl_vdiv_v(self->pts[offset].pos_p, self->pts[offset].pos_p, self->p_gain);
    //		vl_vdiv_v(self->pts[offset+1].pos_p, self->pts[offset+1].pos_p, self->p_gain);
    //		self->pts[offset].time /= self->t_gain;
    //		self->pts[offset+1].time /= self->t_gain;
    //		time = (time - self->t_bias) / self->t_gain;

            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[0]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[0],
                            self->pts[offset].pos_p[0] + self->pts[offset].pos_d[0]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[0],
                            self->pts[offset + 1].pos_p[0] - self->pts[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[0]
                    }
            });

            bz4_d0t(&bz4, time, &pos[0]);
    //		bz4_d0(&bz4, t, p);
    //		pos[0] = p[1];

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[1]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[1],
                            self->pts[offset].pos_p[1] + self->pts[offset].pos_d[1]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[1],
                            self->pts[offset + 1].pos_p[1] - self->pts[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[1]
                    }
            });

            bz4_d0t(&bz4, time, &pos[1]);
    //		bz4_d0(&bz4, t, p);
    //		pos[1] = p[1];

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            self->pts[offset].time,
                            self->pts[offset].pos_p[2]
                    },

                    .p1 = {
                            self->pts[offset].time + self->pts[offset].pos_t[2],
                            self->pts[offset].pos_p[2] + self->pts[offset].pos_d[2]
                    },

                    .p2 = {
                            self->pts[offset + 1].time - self->pts[offset + 1].pos_t[2],
                            self->pts[offset + 1].pos_p[2] - self->pts[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            self->pts[offset + 1].time,
                            self->pts[offset + 1].pos_p[2]
                    }
            });

            bz4_d0t(&bz4, time, &pos[2]);
    //		bz4_d0(&bz4, t, p);
    //		pos[2] = p[1];

    //		vl_vmul_v(self->pts[offset].pos_p, self->pts[offset].pos_p, self->p_gain);
    //		vl_vmul_v(self->pts[offset+1].pos_p, self->pts[offset+1].pos_p, self->p_gain);
    //		vl_vmul_v(pos, pos, self->p_gain);
    //		self->pts[offset].time *= self->t_gain;
    //		self->pts[offset+1].time *= self->t_gain;
    //
    //		vl_vsum(self->pts[offset].pos_p, self->pts[offset].pos_p, self->p_bias);
    //		vl_vsum(self->pts[offset+1].pos_p, self->pts[offset+1].pos_p, self->p_bias);
    //		vl_vsum(pos, pos, self->p_bias);
    //		self->pts[offset].time += self->t_bias;
    //		self->pts[offset+1].time += self->t_bias;
    //		time = time * self->t_gain + self->t_bias;

    //		vl_vsum(pos, pos, self->pts[offset].offset);
        }
    }

	return 0x00;
}

inline u8_t trctraj_bz_pos_local(s_trctraj_bz *self, f64_t time, f64_t *pos)
{
	trctraj_bz_pos_raw(self, time, pos);
	
	if (self->ellp_en != 0x00 && self->ellp != NULL)
	{
		trcellp_ecef(self->ellp, pos, pos);
	}
	
	return 0x00;
}

inline u8_t trctraj_bz_pos(s_trctraj_bz *self, f64_t time, f64_t *pos)
{
	trctraj_bz_pos_local(self, time, pos);
	
	f64_t pos_[3];
	vl3_mmul_v(pos_, &self->ref->rot[0][0], pos);
	vl3_vsum(pos, pos_, &self->ref->pos[0][0]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trctraj_bz_rot (s_trctraj_bz *self, f64_t time, f64_t *rot)
{
	if (self->ellp_en == 0x00)
	{
	
	}
	
	else
	{
        f64_t vel_lla[3];
        f64_t pos_ecef[3];
        f64_t pos_lla[3];

        trctraj_bz_pos_local(self, time, pos_ecef);
        trctraj_bz_vel(self, time, vel_lla);

        f64_t nwh[3];
        trcellp_lla(self->ellp, pos_lla, pos_ecef);
		trcellp_nwhvel(self->ellp, pos_lla, vel_lla, nwh);

		f64_t rot_nwh_tnp[9] = {
			nwh[0], nwh[2], nwh[1],
			0.0, 1.0, 0.0,
			0.0, 0.0, 0.0,
		};
		
		f64_t *x = &rot_nwh_tnp[0];
		f64_t *y = &rot_nwh_tnp[3];
		f64_t *z = &rot_nwh_tnp[6];
		
//		vl_mprint(rot_nwh_tnp);

		vl3_vmul_s(x, x, 1.0 / vl3_vnorm(x));
		
//		vl_mprint(rot_nwh_tnp);

		f64_t xy[3];
		vl3_vmul_s(xy, x, vl3_vdot(x,y));
		vl3_vsub(y, y, xy);
		vl3_vmul_s(y, y, 1.0 / vl3_vnorm(y));
		
		vl3_cross(z, x, y);
		vl3_vmul_s(z, z, 1.0 / vl3_vnorm(z));

//		vl_rnorm(rot_nwh_tnp);

		f64_t rot_nwh[9];
		vl3_tnp(rot_nwh, rot_nwh_tnp);

		s_vl_hpr rot_nwh_hpr = vl_hpr(rot_nwh);
		rot_nwh_hpr.roll = 0.0;
		vl3_rot(rot_nwh, rot_nwh_hpr);

//		vl_mprint(rot_nwh);
//		fflush(stdout);

		f64_t ecef_ctn[9];
		trcellp_ecefrot(self->ellp, pos_ecef, ecef_ctn);
		
		f64_t r_ref[9];
		f64_t *r_inr = rot;
		
		vl3_mmul_m(r_ref, ecef_ctn, rot_nwh);
		vl3_mmul_m(r_inr, &self->ref->rot[0][0], r_ref);
	}

	vl3_rnorm(rot);

	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trctraj_bz_compile (s_trctraj_bz *self)
{
	if (self->pts_offset < 0x02) { return 0x00; }
	
	qsort (self->pts, self->pts_offset, sizeof(s_trctraj_bz_point), trctraj_bz_point_comp);
	
	u32_t i;
	u32_t j;
	
	// Normalize points to prevent numerical instabilities
	
//	vl_vzero(self->p_bias);
//	self->t_bias = 0.0;
//
//	for (s32_t i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vsumm(self->p_bias, self->p_bias, self->pts[i].pos_p, 1.0 / (f64_t) self->pts_offset);
//		self->t_bias += self->pts[i].time / (f64_t) self->pts_offset;
//	}
//
//	for (s32_t i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vsub(self->pts[i].pos_p, self->pts[i].pos_p, self->p_bias);
//		self->pts[i].time -= self->t_bias;
//	}
//
//	vl_vset(self->p_gain, 1.0);
//	self->t_gain = 1.0;
//
//	for (s32_t i = 0; i < self->pts_offset; ++i)
//	{
//		if (fabs(self->pts[i].time) > self->t_gain) self->t_gain = fabs(self->pts[i].time);
//		if (fabs(self->pts[i].pos_p[0]) > self->p_gain[0]) self->p_gain[0] = fabs(self->pts[i].pos_p[0]);
//		if (fabs(self->pts[i].pos_p[1]) > self->p_gain[1]) self->p_gain[1] = fabs(self->pts[i].pos_p[1]);
//		if (fabs(self->pts[i].pos_p[2]) > self->p_gain[2]) self->p_gain[2] = fabs(self->pts[i].pos_p[2]);
//	}
//
//	for (s32_t i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vdiv_v(self->pts[i].pos_p, self->pts[i].pos_p, self->p_gain);
//		self->pts[i].time /= self->t_gain;
//	}
	
	// End of normalization
	
	f64_t m_a[self->pts_offset*2][self->pts_offset*2];
//	f64_t *m_a = (f64_t*) malloc(sizeof(f64_t) * 2*self->pts_offset * 2*self->pts_offset);
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		for (s32_t j = 0; j < 2*self->pts_offset; ++j)
		{
			m_a[2*i+0][j] = 0.0;
			m_a[2*i+1][j] = 0.0;
		}
		
		m_a[2*i+0][2*i-2] = 1;
		m_a[2*i+0][2*i+2] = 1;
		m_a[2*i+0][2*i+0] = 4;
		m_a[2*i+1][2*i-2+1] = 1;
		m_a[2*i+1][2*i+2+1] = 1;
		m_a[2*i+1][2*i+0+1] = 4;
	}
	
	for (s32_t j = 0; j < 2*self->pts_offset; ++j)
	{
		m_a[0][j] = 0.0;
		m_a[1][j] = 0.0;
		
		m_a[2*self->pts_offset-2][j] = 0.0;
		m_a[2*self->pts_offset-1][j] = 0.0;
	}
	
	m_a[0][0] = 2;
	m_a[0][2] = 1;
	m_a[1][1] = 2;
	m_a[1][3] = 1;
	
	m_a[2*self->pts_offset-2][2*self->pts_offset-4] = 1;
	m_a[2*self->pts_offset-2][2*self->pts_offset-2] = 2;
	m_a[2*self->pts_offset-1][2*self->pts_offset-3] = 1;
	m_a[2*self->pts_offset-1][2*self->pts_offset-1] = 2;

//	f64_t pos_p0[2*self->pts_offset];
//	f64_t pos_d0[2*self->pts_offset];
	f64_t *pos_p0 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *pos_p1 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *pos_p2 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	
	f64_t *pos_d0 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *pos_d1 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *pos_d2 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	
	f64_t *rot_p0 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *rot_p1 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *rot_p2 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	
	f64_t *rot_d0 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *rot_d1 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	f64_t *rot_d2 = (f64_t*) malloc(sizeof(f64_t) * 2 * self->pts_offset);
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		pos_p0[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		pos_p0[2 * i + 1] = 3 * (self->pts[i + 1].pos_p[0] - self->pts[i - 1].pos_p[0]);
		
		pos_p1[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		pos_p1[2 * i + 1] = 3 * (self->pts[i + 1].pos_p[1] - self->pts[i - 1].pos_p[1]);
		
		pos_p2[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		pos_p2[2 * i + 1] = 3 * (self->pts[i + 1].pos_p[2] - self->pts[i - 1].pos_p[2]);
		
		rot_p0[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		rot_p0[2 * i + 1] = 3 * (self->pts[i + 1].rot_p[0] - self->pts[i - 1].rot_p[0]);
		
		rot_p1[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		rot_p1[2 * i + 1] = 3 * (self->pts[i + 1].rot_p[1] - self->pts[i - 1].rot_p[1]);
		
		rot_p2[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		rot_p2[2 * i + 1] = 3 * (self->pts[i + 1].rot_p[2] - self->pts[i - 1].rot_p[2]);
	}
	
	pos_p0[0] = 3 * (self->pts[1].time - self->pts[0].time);
	pos_p0[1] = 3 * (self->pts[1].pos_p[0] - self->pts[0].pos_p[0]);
	
	pos_p1[0] = 3 * (self->pts[1].time - self->pts[0].time);
	pos_p1[1] = 3 * (self->pts[1].pos_p[1] - self->pts[0].pos_p[1]);
	
	pos_p2[0] = 3 * (self->pts[1].time - self->pts[0].time);
	pos_p2[1] = 3 * (self->pts[1].pos_p[2] - self->pts[0].pos_p[2]);
	
	rot_p0[0] = 3 * (self->pts[1].time - self->pts[0].time);
	rot_p0[1] = 3 * (self->pts[1].rot_p[0] - self->pts[0].rot_p[0]);
	
	rot_p1[0] = 3 * (self->pts[1].time - self->pts[0].time);
	rot_p1[1] = 3 * (self->pts[1].rot_p[1] - self->pts[0].rot_p[1]);
	
	rot_p2[0] = 3 * (self->pts[1].time - self->pts[0].time);
	rot_p2[1] = 3 * (self->pts[1].rot_p[2] - self->pts[0].rot_p[2]);
	
	pos_p0[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	pos_p0[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].pos_p[0]
												- self->pts[self->pts_offset-2].pos_p[0]);
	
	pos_p1[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	pos_p1[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].pos_p[1]
												- self->pts[self->pts_offset-2].pos_p[1]);
	
	pos_p2[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	pos_p2[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].pos_p[2]
												- self->pts[self->pts_offset-2].pos_p[2]);
	
	rot_p0[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	rot_p0[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].rot_p[0]
												- self->pts[self->pts_offset-2].rot_p[0]);
	
	rot_p1[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	rot_p1[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].rot_p[1]
												- self->pts[self->pts_offset-2].rot_p[1]);
	
	rot_p2[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	rot_p2[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].rot_p[2]
												- self->pts[self->pts_offset-2].rot_p[2]);
	
	f64_t m_inva[self->pts_offset*2][self->pts_offset*2];
	
	vl3_inv(self->pts_offset*2, &m_inva[0][0], &m_a[0][0]);
	
	for (s32_t i = 0; i < self->pts_offset*2; ++i)
	{
		pos_d0[i] = 0.0;
		pos_d1[i] = 0.0;
		pos_d2[i] = 0.0;
		
		rot_d0[i] = 0.0;
		rot_d1[i] = 0.0;
		rot_d2[i] = 0.0;
		
		for (s32_t j = 0; j < self->pts_offset*2; ++j)
		{
			pos_d0[i] += m_inva[i][j] * pos_p0[j];
			pos_d1[i] += m_inva[i][j] * pos_p1[j];
			pos_d2[i] += m_inva[i][j] * pos_p2[j];
			
			rot_d0[i] += m_inva[i][j] * rot_p0[j];
			rot_d1[i] += m_inva[i][j] * rot_p1[j];
			rot_d2[i] += m_inva[i][j] * rot_p2[j];
		}
	}
	
	for (s32_t i = 0; i < self->pts_offset; ++i)
	{
		self->pts[i].pos_t[0] = pos_d0[2 * i + 0] / 3;
		self->pts[i].pos_d[0] = pos_d0[2 * i + 1] / 3;
		
		self->pts[i].pos_t[1] = pos_d1[2 * i + 0] / 3;
		self->pts[i].pos_d[1] = pos_d1[2 * i + 1] / 3;
		
		self->pts[i].pos_t[2] = pos_d2[2 * i + 0] / 3;
		self->pts[i].pos_d[2] = pos_d2[2 * i + 1] / 3;
		
		self->pts[i].rot_t[0] = rot_d0[2 * i + 0] / 3;
		self->pts[i].rot_d[0] = rot_d0[2 * i + 1] / 3;
		
		self->pts[i].rot_t[1] = rot_d1[2 * i + 0] / 3;
		self->pts[i].rot_d[1] = rot_d1[2 * i + 1] / 3;
		
		self->pts[i].rot_t[2] = rot_d2[2 * i + 0] / 3;
		self->pts[i].rot_d[2] = rot_d2[2 * i + 1] / 3;
	}
	
	free(pos_p0);
	free(pos_d0);
	
	free(pos_p1);
	free(pos_d1);
	
	free(pos_p2);
	free(pos_d2);
	
	free(rot_p0);
	free(rot_d0);
	
	free(rot_p1);
	free(rot_d1);
	
	free(rot_p2);
	free(rot_d2);
//	free(m_a);
	
//	for (s32_t i = 0; i < self->pts_offset; ++i)
//	{ vl_vzero(self->pts[i].offset); }
	
//	for (i = 1; i < self->pts_offset-1; ++i)
//	{
//		f64_t l[3];
//		f64_t r[3];
//
//		f64_t temp[3];
//		vl_vcopy(temp, self->pts[i-1].offset);
//		vl_vzero(self->pts[i-1].offset);
//
//		trctraj_bz_pos_local(self, self->pts[i].time - 1E-9, l);
//		trctraj_bz_pos_local(self, self->pts[i].time + 1E-9, r);
//
//		vl_vcopy(self->pts[i-1].offset, temp);
//
//		vl_vsub(l, self->pts[i].pos_p, l);
//		vl_vsub(r, r, self->pts[i].pos_p);
////		trctraj_bz_vel(self, self->pts[i].time, rv);
////		vl_vmul_s(rv, rv, 2E-9);
////		vl_vmul_s(rv, self->pts[i].pos_d, 2.0 * 1E-9);
//
//		vl_vsub(self->pts[i].offset, r, l);
//	}
//
//	for (i = 1; i < self->pts_offset; ++i)
//	{
//		vl_vsum(self->pts[i].offset, self->pts[i].offset, self->pts[i-1].offset);
//	}

	// return points back to normal
	
//	for (s32_t i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vmul_v(self->pts[i].pos_p, self->pts[i].pos_p, self->p_gain);
//		self->pts[i].time *= self->t_gain;
//
//		vl_vsum(self->pts[i].pos_p, self->pts[i].pos_p, self->p_bias);
//		self->pts[i].time += self->t_bias;
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

// API
//u8_t (*init) 		(void *data, void *config)
//u8_t (*free) 		(void *data);
//u8_t (*compile) 	(void *data);
//u8_t (*rot) 		(void *data, f64_t time, f64_t *pos);
//u8_t (*pos) 		(void *data, f64_t time, f64_t *rot);

inline u8_t trctraj_bz_init_ (void **data, void *config)
{
	*data = (s_trctraj_bz*) malloc(sizeof(s_trctraj_bz));
	
	s_trctraj_bz *traj_bz = (s_trctraj_bz*) *data;
	s_trctraj_bz_init *traj_bz_init = (s_trctraj_bz_init*) config;
	
	traj_bz_init->pts = (s_trctraj_bz_point*) malloc(sizeof(s_trctraj_bz_point) * 100);
	traj_bz_init->pts_size = 100;
	
	return trctraj_bz_init(traj_bz, *traj_bz_init);
}

inline u8_t trctraj_bz_free_ (void **data)
{
	s_trctraj_bz *traj_bz = (s_trctraj_bz*) *data;
	
	free(traj_bz->pts);
	free(traj_bz);
	
	return 0x00;
}

inline u8_t trctraj_bz_save_ (void *data, void *config, u8_t **v_file)
{
	s_trctraj_bz *traj = (s_trctraj_bz*) data;
	s_trctraj_bz_init *attr = (s_trctraj_bz_init*) config;
	return trctraj_bz_save(traj, attr, v_file);
}

inline u8_t trctraj_bz_load_ (void *data, void *config, u8_t **v_file)
{
	s_trctraj_bz *traj = (s_trctraj_bz*) data;
	s_trctraj_bz_init *attr = (s_trctraj_bz_init*) config;
	return trctraj_bz_load(traj, attr, v_file);
}

inline u8_t trctraj_bz_compile_(void *data)
{
	s_trctraj_bz *traj_bz = (s_trctraj_bz*) data;
	
	return trctraj_bz_compile(traj_bz);
}

inline u8_t trctraj_bz_pos_(void *data, f64_t time, f64_t *pos)
{
	s_trctraj_bz *traj_bz = (s_trctraj_bz*) data;
	
	return trctraj_bz_pos(traj_bz, time, pos);
}

inline u8_t trctraj_bz_rot_(void *data, f64_t time, f64_t *rot)
{
	s_trctraj_bz *traj_bz = (s_trctraj_bz*) data;

	return trctraj_bz_rot(traj_bz, time, rot);
}

inline u8_t trctraj_bz_info_(void *data, s_trctraj_info *info)
{
	s_trctraj_bz *traj_bz = (s_trctraj_bz*) data;
	
	if (traj_bz->pts_offset == 0x00)
	{
		info->preview_time[0] = 0.0;
		info->preview_time[1] = 0.0;
	}
	
	if (traj_bz->pts_offset == 0x01)
	{
		info->preview_time[0] = traj_bz->pts[0].time;
		info->preview_time[1] = traj_bz->pts[0].time;
	}
	
	if (traj_bz->pts_offset >= 0x02)
	{
		info->preview_time[0] = traj_bz->pts[0].time;
		info->preview_time[1] = traj_bz->pts[traj_bz->pts_offset-1].time;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __trctraj__ */
