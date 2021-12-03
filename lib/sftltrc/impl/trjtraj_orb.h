
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_orb - trace

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vl.h>

#include <sftltrc/trctraj.h>
#include <sftltrc/trcspl.h>
#include <sftltrc/trcellp.h>

//------------------------------------------------------------------------------

typedef struct trctraj_orb
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	u32_t 	ref_hash;
	
	u8_t 	sync_en;
	
	f64_t       radius;
	f64_t       rate;
	f64_t       tilt[9];
	
	f64_t       s_rate;
	f64_t       s_tilt[9];
	
} 	s_trctraj_orb;

typedef struct trctraj_orb_init
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	u32_t 	ref_hash;
	
	u8_t 	sync_en;
	
	f64_t       radius;
	f64_t       rate;
	f64_t       tilt[9];
	
	f64_t       s_rate;
	f64_t       s_tilt[9];
	
} 	s_trctraj_orb_init;

//------------------------------------------------------------------------------

inline u8_t trctraj_orb_init(s_trctraj_orb *self, s_trctraj_orb_init attr)
{
	self->eng    	= attr.eng;
	
	self->ref = attr.ref;
	self->ref_hash = self->ref->hash;
	
	self->sync_en  = attr.sync_en;
	
	self->radius = attr.radius;
	self->rate   = attr.rate;
	
	vl3_mcopy(self->tilt, attr.tilt);
	
	self->s_rate = attr.s_rate;
	vl3_mcopy(self->s_tilt, attr.s_tilt);
	
	return 0x00;
}

inline u8_t trctraj_orb_save(s_trctraj_orb *self, s_trctraj_orb_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trctraj_orb_load(s_trctraj_orb *self, s_trctraj_orb_init *attr, u8_t **v_file)
{
	self->eng = attr->eng;
	self->ref = trceng_find_obj(self->eng, self->ref_hash);
	
	return 0x00;
}

inline u8_t trctraj_orb_compile(s_trctraj_orb *self)
{
	return 0x00;
}

inline u8_t trctraj_orb_pos(s_trctraj_orb *self, f64_t time, f64_t *pos)
{
	switch (self->sync_en)
	{
		default:
		case 0x00:
		{
			f64_t angle = self->rate * time;
			f64_t pos_t[3];
			f64_t pos_n[3];
			
			pos_t[0] = vl_sin(angle) * self->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vl_cos(angle) * self->radius;
			
			vl3_mmul_v(pos_n, self->tilt, pos_t);
			
			vl3_vsum(pos, pos_n, self->ref->pos[0]);
			
			return 0x00;
		}
		
		case 0x01:
		{
			f64_t angle = self->rate * time;
			f64_t pos_t[3];
			f64_t pos_n[3];
			
			pos_t[0] = vl_sin(angle) * self->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vl_cos(angle) * self->radius;
			
			vl3_mmul_v(pos_n, self->tilt, pos_t);
			vl3_mmul_v(pos, self->ref->rot[0], pos_n);
			
			vl3_vsum(pos, pos, self->ref->pos[0]);
			
			return 0x00;
		}
	}
	
	return 0x01;
}

inline u8_t trctraj_orb_rot(s_trctraj_orb *self, f64_t time, f64_t *rot)
{
	switch (self->sync_en)
	{
		default:
		case 0x00:
		{
			f64_t angle = self->s_rate * time;
			
			f64_t rot_t[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vl3_rot(rot_t, (s_vl_hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vl3_mmul_m(rot, self->s_tilt, rot_t);
			
			return 0x00;
			
			break;
		}
		
		case 0x01:
		{
			f64_t angle = self->s_rate * time;
			
			f64_t rot_t[9];
			f64_t rot_n[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vl3_rot(rot_t, (s_vl_hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vl3_mmul_m(rot_n, self->s_tilt, rot_t);
			vl3_mmul_m(rot, self->ref->rot[0], rot_n);
			
			return 0x00;
			
			break;
		}
	}
	
	return 0x01;
}

//------------------------------------------------------------------------------

// API
//u8_t (*init) 		(void *data, void *config)
//u8_t (*free) 		(void *data);
//u8_t (*compile) 	(void *data);
//u8_t (*rot) 		(void *data, f64_t time, f64_t *pos);
//u8_t (*pos) 		(void *data, f64_t time, f64_t *rot);

inline u8_t trctraj_orb_init_ (void **data, void *config)
{
	*data = (s_trctraj_orb*) malloc(sizeof(s_trctraj_orb));
	
	s_trctraj_orb *traj = (s_trctraj_orb*) *data;
	s_trctraj_orb_init *init = (s_trctraj_orb_init*) config;
	
	return trctraj_orb_init(traj, *init);
}

inline u8_t trctraj_orb_free_ (void **data)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) *data;
	
	free(traj);
	
	return 0x00;
}

inline u8_t trctraj_orb_save_ (void *data, void *config, u8_t **v_file)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	s_trctraj_orb_init *attr = (s_trctraj_orb_init*) config;
	
	return trctraj_orb_save(traj, attr, v_file);
}

inline u8_t trctraj_orb_load_ (void *data, void *config, u8_t **v_file)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	s_trctraj_orb_init *attr = (s_trctraj_orb_init*) config;
	return trctraj_orb_load(traj, attr, v_file);
}

inline u8_t trctraj_orb_compile_(void *data)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	return trctraj_orb_compile(traj);
}

inline u8_t trctraj_orb_pos_(void *data, f64_t time, f64_t *pos)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	return trctraj_orb_pos(traj, time, pos);
}

inline u8_t trctraj_orb_rot_(void *data, f64_t time, f64_t *rot)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	return trctraj_orb_rot(traj, time, rot);
}

inline u8_t trctraj_orb_info_(void *data, s_trctraj_info *info)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = vl_2pi / traj->rate;
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __trctraj__ */
