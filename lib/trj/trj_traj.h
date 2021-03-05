
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_TRAJ__
#define __TRJ_TRAJ__

//----------------------------------------------------------------

#include <lib_internal/vl.h>
#include <lib_internal/bz.h>

#include "trj_api.h"
#include "trj_ellp.h"

//----------------------------------------------------------------

typedef struct trj_traj_static
{
	s_trj_eng 	*eng;
	
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	uint8_t 	ellp_en;
	s_trj_ellp 	*ellp;
	uint32_t 	ellp_hash;
	
	vlf_t 		pos[3];
	vlf_t 		pos_cache[3];
	vlf_t 		rot[9];
	vlf_t 		rot_cache[9];
	
} 	s_trj_traj_static;

typedef struct trj_traj_static_init
{
	s_trj_eng 	*eng;
	
	s_trj_obj 	*ref;
	
	uint8_t 	ellp_en;
	s_trj_ellp  *ellp;
	
	vlf_t 		pos[3];
	vlf_t 		rot[9];
	
} 	s_trj_traj_static_init;

//----------------------------------------------------------------

inline uint8_t trj_traj_static_init(s_trj_traj_static *self, s_trj_traj_static_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	self->ref_hash = self->ref->hash;
	
	self->ellp_en = attr.ellp_en;
	self->ellp = attr.ellp;
	if (self->ellp != NULL)
	{ self->ellp_hash = self->ellp->hash; }
	
	vl_vcopy(self->pos, attr.pos);
	vl_mcopy(self->rot, attr.rot);
	
	return 0x00;
}

inline uint8_t trj_traj_static_save(s_trj_traj_static *self, s_trj_traj_static_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_traj_static_load(s_trj_traj_static *self, s_trj_traj_static_init *attr, uint8_t **v_file)
{
	self->eng = attr->eng;
	self->ref = trj_eng_find_obj(self->eng, self->ref_hash);
	self->ellp = trj_eng_find_ellp(self->eng, self->ellp_hash);
	
	return 0x00;
}

inline uint8_t trj_traj_static_compile(s_trj_traj_static *self)
{
	if (self->ellp_en != 0x00)
	{
		trj_ellp_ecef(self->ellp, self->pos_cache, self->pos);
		
		vlf_t ecef_ctn[9];
		trj_ellp_ecefrot(self->ellp, self->pos_cache, ecef_ctn);
		
		vl_mmul_m(self->rot_cache, ecef_ctn, self->rot);
	}
	
	return 0x00;
}

inline uint8_t trj_traj_static_pos(s_trj_traj_static *self, vlf_t time, vlf_t *pos)
{
	if (self->ellp_en == 0x00)
	{ vl_vcopy(pos, self->pos); }
	
	else
	{ vl_vcopy(pos, self->pos_cache); }
	
	vl_mmul_v(pos, &self->ref->rot[0][0], pos);
	vl_vsum(pos, pos, &self->ref->pos[0][0]);
	
	return 0x00;
}

inline uint8_t trj_traj_static_rot(s_trj_traj_static *self, vlf_t time, vlf_t *rot)
{
	if (self->ellp_en == 0x00)
	{ vl_mcopy(rot, self->rot); }
	
	else
	{ vl_mcopy(rot, self->rot_cache); }
	
	vl_mmul_m(rot, &self->ref->rot[0][0], rot);
	
	return 0x00;
}

//----------------------------------------------------------------

// API
//uint8_t (*init) 		(void *data, void *config)
//uint8_t (*free) 		(void *data);
//uint8_t (*compile) 	(void *data);
//uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *pos);
//uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *rot);

inline uint8_t trj_traj_static_init_ (void **data, void *config)
{
	*data = (s_trj_traj_static*) malloc(sizeof(s_trj_traj_static));
	
	s_trj_traj_static *traj = (s_trj_traj_static*) *data;
	s_trj_traj_static_init *attr = (s_trj_traj_static_init*) config;
	
	return trj_traj_static_init(traj, *attr);
}

inline uint8_t trj_traj_static_free_ (void **data)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) *data;
	
	free(traj);
	
	return 0x00;
}

inline uint8_t trj_traj_static_save_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) data;
	s_trj_traj_static_init *attr = (s_trj_traj_static_init*) config;
	
	return trj_traj_static_save(traj, attr, v_file);
}

inline uint8_t trj_traj_static_load_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) data;
	s_trj_traj_static_init *attr = (s_trj_traj_static_init*) config;
	
	return trj_traj_static_load(traj, attr, v_file);
}

inline uint8_t trj_traj_static_compile_(void *data)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) data;
	
	return trj_traj_static_compile(traj);
}

inline uint8_t trj_traj_static_pos_(void *data, vlf_t time, vlf_t *pos)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) data;
	
	return trj_traj_static_pos(traj, time, pos);
}

inline uint8_t trj_traj_static_rot_(void *data, vlf_t time, vlf_t *rot)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) data;
	
	return trj_traj_static_rot(traj, time, rot);
}

inline uint8_t trj_traj_static_info_(void *data, s_trj_traj_info *info)
{
	s_trj_traj_static *traj = (s_trj_traj_static*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = 1.0;
	
	return 0x00;
}
//----------------------------------------------------------------

typedef struct trj_traj_orb
{
	s_trj_eng 	*eng;
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	uint8_t 	sync_en;
	
	vlf_t       radius;
	vlf_t       rate;
	vlf_t       tilt[9];
	
	vlf_t       s_rate;
	vlf_t       s_tilt[9];
	
} 	s_trj_traj_orb;

typedef struct trj_traj_orb_init
{
	s_trj_eng 	*eng;
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	uint8_t 	sync_en;
	
	vlf_t       radius;
	vlf_t       rate;
	vlf_t       tilt[9];
	
	vlf_t       s_rate;
	vlf_t       s_tilt[9];
	
} 	s_trj_traj_orb_init;

//----------------------------------------------------------------

inline uint8_t trj_traj_orb_init(s_trj_traj_orb *self, s_trj_traj_orb_init attr)
{
	self->eng    	= attr.eng;
	
	self->ref = attr.ref;
	self->ref_hash = self->ref->hash;
	
	self->sync_en  = attr.sync_en;
	
	self->radius = attr.radius;
	self->rate   = attr.rate;
	
	vl_mcopy(self->tilt, attr.tilt);
	
	self->s_rate = attr.s_rate;
	vl_mcopy(self->s_tilt, attr.s_tilt);
	
	return 0x00;
}

inline uint8_t trj_traj_orb_save(s_trj_traj_orb *self, s_trj_traj_orb_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_traj_orb_load(s_trj_traj_orb *self, s_trj_traj_orb_init *attr, uint8_t **v_file)
{
	self->eng = attr->eng;
	self->ref = trj_eng_find_obj(self->eng, self->ref_hash);
	
	return 0x00;
}

inline uint8_t trj_traj_orb_compile(s_trj_traj_orb *self)
{
	return 0x00;
}

inline uint8_t trj_traj_orb_pos(s_trj_traj_orb *self, vlf_t time, vlf_t *pos)
{
	switch (self->sync_en)
	{
		default:
		case 0x00:
		{
			vlf_t angle = self->rate * time;
			vlf_t pos_t[3];
			vlf_t pos_n[3];
			
			pos_t[0] = vl_sin(angle) * self->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vl_cos(angle) * self->radius;
			
			vl_mmul_v(pos_n, self->tilt, pos_t);
			
			vl_vsum(pos, pos_n, self->ref->pos[0]);
			
			return 0x00;
		}
		
		case 0x01:
		{
			vlf_t angle = self->rate * time;
			vlf_t pos_t[3];
			vlf_t pos_n[3];
			
			pos_t[0] = vl_sin(angle) * self->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vl_cos(angle) * self->radius;
			
			vl_mmul_v(pos_n, self->tilt, pos_t);
			vl_mmul_v(pos, self->ref->rot[0], pos_n);
			
			vl_vsum(pos, pos, self->ref->pos[0]);
			
			return 0x00;
		}
	}
	
	return 0x01;
}

inline uint8_t trj_traj_orb_rot(s_trj_traj_orb *self, vlf_t time, vlf_t *rot)
{
	switch (self->sync_en)
	{
		default:
		case 0x00:
		{
			vlf_t angle = self->s_rate * time;
			
			vlf_t rot_t[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vl_rot(rot_t, (s_vl_hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vl_mmul_m(rot, self->s_tilt, rot_t);
			
			return 0x00;
			
			break;
		}
		
		case 0x01:
		{
			vlf_t angle = self->s_rate * time;
			
			vlf_t rot_t[9];
			vlf_t rot_n[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vl_rot(rot_t, (s_vl_hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vl_mmul_m(rot_n, self->s_tilt, rot_t);
			vl_mmul_m(rot, self->ref->rot[0], rot_n);
			
			return 0x00;
			
			break;
		}
	}
	
	return 0x01;
}

//----------------------------------------------------------------

// API
//uint8_t (*init) 		(void *data, void *config)
//uint8_t (*free) 		(void *data);
//uint8_t (*compile) 	(void *data);
//uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *pos);
//uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *rot);

inline uint8_t trj_traj_orb_init_ (void **data, void *config)
{
	*data = (s_trj_traj_orb*) malloc(sizeof(s_trj_traj_orb));
	
	s_trj_traj_orb *traj = (s_trj_traj_orb*) *data;
	s_trj_traj_orb_init *init = (s_trj_traj_orb_init*) config;
	
	return trj_traj_orb_init(traj, *init);
}

inline uint8_t trj_traj_orb_free_ (void **data)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) *data;
	
	free(traj);
	
	return 0x00;
}

inline uint8_t trj_traj_orb_save_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) data;
	s_trj_traj_orb_init *attr = (s_trj_traj_orb_init*) config;
	
	return trj_traj_orb_save(traj, attr, v_file);
}

inline uint8_t trj_traj_orb_load_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) data;
	s_trj_traj_orb_init *attr = (s_trj_traj_orb_init*) config;
	return trj_traj_orb_load(traj, attr, v_file);
}

inline uint8_t trj_traj_orb_compile_(void *data)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) data;
	
	return trj_traj_orb_compile(traj);
}

inline uint8_t trj_traj_orb_pos_(void *data, vlf_t time, vlf_t *pos)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) data;
	
	return trj_traj_orb_pos(traj, time, pos);
}

inline uint8_t trj_traj_orb_rot_(void *data, vlf_t time, vlf_t *rot)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) data;
	
	return trj_traj_orb_rot(traj, time, rot);
}

inline uint8_t trj_traj_orb_info_(void *data, s_trj_traj_info *info)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = vl_2pi / traj->rate;
	
	return 0x00;
}

//----------------------------------------------------------------

typedef struct trj_traj_bz_point
{
	float64_t time;
	
	float64_t pos_p[3];
	float64_t pos_t[3];
	float64_t pos_d[3];
	
	float64_t rot_p[3];
	float64_t rot_t[3];
	float64_t rot_d[3];
	
//	float64_t offset[3];
	
	/*
	 pos_d MUST BE
	 to the right of pos_p (pos_d[0] > pos_p[0])
	 cannot overlap next point
	 */
	
} 	s_trj_traj_bz_point;

typedef struct trj_traj_bz
{
	s_trj_eng 	*eng;
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	s_trj_traj_bz_point *pts;
	uint32_t 	pts_size;
	uint32_t 	pts_offset;
	
	uint8_t 	ellp_en;
	s_trj_ellp 	*ellp;
	uint32_t 	ellp_hash;
//
//	vlf_t 		p_bias[3];
//	vlf_t 		p_gain[3];
//
//	vlf_t 		t_bias;
//	vlf_t 		t_gain;
	
} 	s_trj_traj_bz;

typedef struct trj_traj_bz_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
	uint8_t ellp_en;
	s_trj_ellp *ellp;
	
	uint32_t pts_size;
	s_trj_traj_bz_point *pts;
	
} 	s_trj_traj_bz_init;

//----------------------------------------------------------------

inline uint8_t trj_traj_bz_init(s_trj_traj_bz *self, s_trj_traj_bz_init attr)
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
	{ self->ellp_hash = vl_crc32(self->ellp->desc); }
	
//	vl_vset(self->p_bias, 0.0);
//	vl_vset(self->p_gain, 1.0);
//
//	self->t_bias = 0.0;
//	self->t_gain = 1.0;
	
	return 0x00;
}

inline uint8_t trj_traj_bz_save(s_trj_traj_bz *self, s_trj_traj_bz_init *attr, uint8_t **v_file)
{
	memcpy(*v_file, self->pts, self->pts_size * sizeof(s_trj_traj_bz_point));
	*v_file += self->pts_size * sizeof(s_trj_traj_bz_point);
	
	return 0x00;
}

inline uint8_t trj_traj_bz_load(s_trj_traj_bz *self, s_trj_traj_bz_init *attr, uint8_t **v_file)
{
	self->pts = (s_trj_traj_bz_point*) malloc(self->pts_size * sizeof(s_trj_traj_bz_point));
	memcpy(self->pts, *v_file, self->pts_size * sizeof(s_trj_traj_bz_point));

	*v_file += self->pts_size * sizeof(s_trj_traj_bz_point);
	
	self->eng = attr->eng;
	
	self->ref  = trj_eng_find_obj (self->eng, self->ref_hash);
	self->ellp = trj_eng_find_ellp(self->eng, self->ellp_hash);
	
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

inline int trj_traj_bz_point_comp (const void *p0_, const void *p1_)
{
	s_trj_traj_bz_point *p0 = (s_trj_traj_bz_point*) p0_;
	s_trj_traj_bz_point *p1 = (s_trj_traj_bz_point*) p1_;
	
	if (p0->time > p1->time) return +1;
	if (p0->time < p1->time) return -1;
	
	return 0;
}

inline uint8_t trj_traj_bz_add(s_trj_traj_bz *self, s_trj_traj_bz_point point)
{
	self->pts[self->pts_offset] = point;
	self->pts_offset++;
	
	qsort (self->pts, self->pts_offset, sizeof(s_trj_traj_bz_point), trj_traj_bz_point_comp);
	
	return 0x00;
}

inline uint8_t trj_traj_bz_rem(s_trj_traj_bz *self, uint32_t index)
{
	uint32_t i;
	
	for (i = index; i < self->pts_offset-1; ++i)
	{
		self->pts[i] = self->pts[i+1];
	}
	
	--self->pts_offset;
	
	return 0x00;
}

inline uint8_t trj_traj_bz_vel(s_trj_traj_bz *self, vlf_t time, vlf_t *vel)
{
	uint32_t offset = 0x00;
	
	if (self->pts_offset == 0x00) { return 0x01; }
	
	if (time < self->pts[0x00].time) time = self->pts[0x00].time;
	if (time > self->pts[self->pts_offset-1].time) time = self->pts[self->pts_offset-1].time;
	
	while ((time < self->pts[offset].time)
		   || (time > self->pts[offset+1].time))
	{
		++offset;
	}
	
	if (fabs(time-self->pts[offset].time) < 1E-9)
	{
		vlf_t acc[3];

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

		vl_vdiv_v(vel, self->pts[offset].pos_d, self->pts[offset].pos_t);
		vl_vsumm(vel, vel, acc, time - self->pts[offset].time);
	}

	else if (fabs(time-self->pts[offset+1].time) < 1E-9)
	{
		vlf_t acc[3];

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

		vl_vdiv_v(vel, self->pts[offset+1].pos_d, self->pts[offset+1].pos_t);
		vl_vsumm(vel, vel, acc, time - self->pts[offset+1].time);
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
	
	if (self->ellp_en != 0x00 && self->ellp != NULL)
	{
	}
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t trj_traj_bz_pos_raw(s_trj_traj_bz *self, vlf_t time, vlf_t *pos)
{
	uint32_t offset = 0x00;
	
	if (self->pts_offset == 0x00) { return 0x01; }
	
	if (time < self->pts[0x00].time) { time = self->pts[0x00].time; }
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
		vlf_t vel[3];
		vl_vcopy(pos, self->pts[offset].pos_p);
		vl_vdiv_v(vel, self->pts[offset].pos_d, self->pts[offset].pos_t);
		vl_vsumm(pos, pos, vel, time - self->pts[offset].time);
	}
	
	else if (fabs(time-self->pts[offset+1].time) < 1E-9)
	{
		vlf_t vel[3];
		vl_vcopy(pos, self->pts[offset+1].pos_p);
		vl_vdiv_v(vel, self->pts[offset+1].pos_d, self->pts[offset+1].pos_t);
		vl_vsumm(pos, pos, vel, time - self->pts[offset+1].time);
	}
	
	else
	{
//		vlf_t t = (time - self->pts[offset].time) / (self->pts[offset+1].time - self->pts[offset].time);
//		vlf_t p[2];

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
	
	return 0x00;
}

inline uint8_t trj_traj_bz_pos_local(s_trj_traj_bz *self, vlf_t time, vlf_t *pos)
{
	trj_traj_bz_pos_raw(self, time, pos);
	
	if (self->ellp_en != 0x00 && self->ellp != NULL)
	{
		trj_ellp_ecef(self->ellp, pos, pos);
	}
	
	return 0x00;
}

inline uint8_t trj_traj_bz_pos(s_trj_traj_bz *self, vlf_t time, vlf_t *pos)
{
	trj_traj_bz_pos_local(self, time, pos);
	
	vlf_t pos_[3];
	vl_mmul_v(pos_, &self->ref->rot[0][0], pos);
	vl_vsum(pos, pos_, &self->ref->pos[0][0]);
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t trj_traj_bz_rot (s_trj_traj_bz *self, vlf_t time, vlf_t *rot)
{
	if (self->ellp_en == 0x00)
	{
	
	}
	
	else
	{
        vlf_t vel_lla[3];
        vlf_t pos_ecef[3];
        vlf_t pos_lla[3];

        trj_traj_bz_pos_local(self, time, pos_ecef);
        trj_traj_bz_vel(self, time, vel_lla);

        vlf_t nwh[3];
        trj_ellp_lla(self->ellp, pos_lla, pos_ecef);
		trj_ellp_nwhvel(self->ellp, pos_lla, vel_lla, nwh);

		vlf_t rot_nwh_tnp[9] = {
			nwh[0], nwh[2], nwh[1],
			0.0, 1.0, 0.0,
			0.0, 0.0, 0.0,
		};
		
		vlf_t *x = &rot_nwh_tnp[0];
		vlf_t *y = &rot_nwh_tnp[3];
		vlf_t *z = &rot_nwh_tnp[6];
		
//		vl_mprint(rot_nwh_tnp);

		vl_vmul_s(x, x, 1.0 / vl_vnorm(x));
		
//		vl_mprint(rot_nwh_tnp);

		vlf_t xy[3];
		vl_vmul_s(xy, x, vl_vdot(x,y));
		vl_vsub(y, y, xy);
		vl_vmul_s(y, y, 1.0 / vl_vnorm(y));
		
		vl_cross(z, x, y);
		vl_vmul_s(z, z, 1.0 / vl_vnorm(z));

//		vl_rnorm(rot_nwh_tnp);

		vlf_t rot_nwh[9];
		vl_tnp(rot_nwh, rot_nwh_tnp);

		s_vl_hpr rot_nwh_hpr;
		vl_hpr(&rot_nwh_hpr, rot_nwh);
		rot_nwh_hpr.roll = 0.0;
		vl_rot(rot_nwh, rot_nwh_hpr);

//		vl_mprint(rot_nwh);
//		fflush(stdout);

		vlf_t ecef_ctn[9];
		trj_ellp_ecefrot(self->ellp, pos_ecef, ecef_ctn);
		
		vlf_t r_ref[9];
		vlf_t *r_inr = rot;
		
		vl_mmul_m(r_ref, ecef_ctn, rot_nwh);
		vl_mmul_m(r_inr, &self->ref->rot[0][0], r_ref);
	}

	vl_rnorm(rot);

	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t trj_traj_bz_compile(s_trj_traj_bz *self)
{
	if (self->pts_offset < 0x02) { return 0x00; }
	
	qsort (self->pts, self->pts_offset, sizeof(s_trj_traj_bz_point), trj_traj_bz_point_comp);
	
	uint32_t i;
	uint32_t j;
	
	// Normalize points to prevent numerical instabilities
	
//	vl_vzero(self->p_bias);
//	self->t_bias = 0.0;
//
//	for (i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vsumm(self->p_bias, self->p_bias, self->pts[i].pos_p, 1.0 / (vlf_t) self->pts_offset);
//		self->t_bias += self->pts[i].time / (vlf_t) self->pts_offset;
//	}
//
//	for (i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vsub(self->pts[i].pos_p, self->pts[i].pos_p, self->p_bias);
//		self->pts[i].time -= self->t_bias;
//	}
//
//	vl_vset(self->p_gain, 1.0);
//	self->t_gain = 1.0;
//
//	for (i = 0; i < self->pts_offset; ++i)
//	{
//		if (fabs(self->pts[i].time) > self->t_gain) self->t_gain = fabs(self->pts[i].time);
//		if (fabs(self->pts[i].pos_p[0]) > self->p_gain[0]) self->p_gain[0] = fabs(self->pts[i].pos_p[0]);
//		if (fabs(self->pts[i].pos_p[1]) > self->p_gain[1]) self->p_gain[1] = fabs(self->pts[i].pos_p[1]);
//		if (fabs(self->pts[i].pos_p[2]) > self->p_gain[2]) self->p_gain[2] = fabs(self->pts[i].pos_p[2]);
//	}
//
//	for (i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vdiv_v(self->pts[i].pos_p, self->pts[i].pos_p, self->p_gain);
//		self->pts[i].time /= self->t_gain;
//	}
	
	// End of normalization
	
	vlf_t m_a[self->pts_offset*2][self->pts_offset*2];
//	vlf_t *m_a = (vlf_t*) malloc(sizeof(vlf_t) * 2*self->pts_offset * 2*self->pts_offset);
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		for (j = 0; j < 2*self->pts_offset; ++j)
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
	
	for (j = 0; j < 2*self->pts_offset; ++j)
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

//	vlf_t pos_p0[2*self->pts_offset];
//	vlf_t pos_d0[2*self->pts_offset];
	vlf_t *pos_p0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_p1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_p2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	vlf_t *pos_d0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_d1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_d2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	vlf_t *rot_p0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_p1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_p2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	vlf_t *rot_d0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_d1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_d2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
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
	
	vlf_t m_inva[self->pts_offset*2][self->pts_offset*2];
	
	vl_inv(self->pts_offset*2, &m_inva[0][0], &m_a[0][0]);
	
	for (i = 0; i < self->pts_offset*2; ++i)
	{
		pos_d0[i] = 0.0;
		pos_d1[i] = 0.0;
		pos_d2[i] = 0.0;
		
		rot_d0[i] = 0.0;
		rot_d1[i] = 0.0;
		rot_d2[i] = 0.0;
		
		for (j = 0; j < self->pts_offset*2; ++j)
		{
			pos_d0[i] += m_inva[i][j] * pos_p0[j];
			pos_d1[i] += m_inva[i][j] * pos_p1[j];
			pos_d2[i] += m_inva[i][j] * pos_p2[j];
			
			rot_d0[i] += m_inva[i][j] * rot_p0[j];
			rot_d1[i] += m_inva[i][j] * rot_p1[j];
			rot_d2[i] += m_inva[i][j] * rot_p2[j];
		}
	}
	
	for (i = 0; i < self->pts_offset; ++i)
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
	
//	for (i = 0; i < self->pts_offset; ++i)
//	{ vl_vzero(self->pts[i].offset); }
	
//	for (i = 1; i < self->pts_offset-1; ++i)
//	{
//		vlf_t l[3];
//		vlf_t r[3];
//
//		vlf_t temp[3];
//		vl_vcopy(temp, self->pts[i-1].offset);
//		vl_vzero(self->pts[i-1].offset);
//
//		trj_traj_bz_pos_local(self, self->pts[i].time - 1E-9, l);
//		trj_traj_bz_pos_local(self, self->pts[i].time + 1E-9, r);
//
//		vl_vcopy(self->pts[i-1].offset, temp);
//
//		vl_vsub(l, self->pts[i].pos_p, l);
//		vl_vsub(r, r, self->pts[i].pos_p);
////		trj_traj_bz_vel(self, self->pts[i].time, rv);
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
	
//	for (i = 0; i < self->pts_offset; ++i)
//	{
//		vl_vmul_v(self->pts[i].pos_p, self->pts[i].pos_p, self->p_gain);
//		self->pts[i].time *= self->t_gain;
//
//		vl_vsum(self->pts[i].pos_p, self->pts[i].pos_p, self->p_bias);
//		self->pts[i].time += self->t_bias;
//	}
	
	return 0x00;
}

//----------------------------------------------------------------

// API
//uint8_t (*init) 		(void *data, void *config)
//uint8_t (*free) 		(void *data);
//uint8_t (*compile) 	(void *data);
//uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *pos);
//uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *rot);

inline uint8_t trj_traj_bz_init_ (void **data, void *config)
{
	*data = (s_trj_traj_bz*) malloc(sizeof(s_trj_traj_bz));
	
	s_trj_traj_bz *traj_bz = (s_trj_traj_bz*) *data;
	s_trj_traj_bz_init *traj_bz_init = (s_trj_traj_bz_init*) config;
	
	traj_bz_init->pts = (s_trj_traj_bz_point*) malloc(sizeof(s_trj_traj_bz_point) * 100);
	traj_bz_init->pts_size = 100;
	
	return trj_traj_bz_init(traj_bz, *traj_bz_init);
}

inline uint8_t trj_traj_bz_free_ (void **data)
{
	s_trj_traj_bz *traj_bz = (s_trj_traj_bz*) *data;
	
	free(traj_bz->pts);
	free(traj_bz);
	
	return 0x00;
}

inline uint8_t trj_traj_bz_save_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_bz *traj = (s_trj_traj_bz*) data;
	s_trj_traj_bz_init *attr = (s_trj_traj_bz_init*) config;
	return trj_traj_bz_save(traj, attr, v_file);
}

inline uint8_t trj_traj_bz_load_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_bz *traj = (s_trj_traj_bz*) data;
	s_trj_traj_bz_init *attr = (s_trj_traj_bz_init*) config;
	return trj_traj_bz_load(traj, attr, v_file);
}

inline uint8_t trj_traj_bz_compile_(void *data)
{
	s_trj_traj_bz *traj_bz = (s_trj_traj_bz*) data;
	
	return trj_traj_bz_compile(traj_bz);
}

inline uint8_t trj_traj_bz_pos_(void *data, vlf_t time, vlf_t *pos)
{
	s_trj_traj_bz *traj_bz = (s_trj_traj_bz*) data;
	
	return trj_traj_bz_pos(traj_bz, time, pos);
}

inline uint8_t trj_traj_bz_rot_(void *data, vlf_t time, vlf_t *rot)
{
	s_trj_traj_bz *traj_bz = (s_trj_traj_bz*) data;

	return trj_traj_bz_rot(traj_bz, time, rot);
}

inline uint8_t trj_traj_bz_info_(void *data, s_trj_traj_info *info)
{
	s_trj_traj_bz *traj_bz = (s_trj_traj_bz*) data;
	
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

//----------------------------------------------------------------

#endif /* __TRJ_TRAJ__ */
