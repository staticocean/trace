
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_orb - trace

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trctraj.h>
#include <sftltrc/trcspl.h>
#include <sftltrc/trcellp.h>

//------------------------------------------------------------------------------

typedef struct trctraj_orb
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	t_u32 	ref_hash;
	
	t_u8 	sync_en;
	
	t_f64       radius;
	t_f64       rate;
	t_f64       tilt[9];
	
	t_f64       s_rate;
	t_f64       s_tilt[9];
	
} 	s_trctraj_orb;

typedef struct trctraj_orb_init
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	t_u32 	ref_hash;
	
	t_u8 	sync_en;
	
	t_f64       radius;
	t_f64       rate;
	t_f64       tilt[9];
	
	t_f64       s_rate;
	t_f64       s_tilt[9];
	
} 	s_trctraj_orb_init;

//------------------------------------------------------------------------------

inline t_u8 trctraj_orb_init(s_trctraj_orb *self, s_trctraj_orb_init attr)
{
	self->eng    	= attr.eng;
	
	self->ref = attr.ref;
	self->ref_hash = self->ref->hash;
	
	self->sync_en  = attr.sync_en;
	
	self->radius = attr.radius;
	self->rate   = attr.rate;
	
	vld3m_copy(self->tilt, attr.tilt);
	
	self->s_rate = attr.s_rate;
	vld3m_copy(self->s_tilt, attr.s_tilt);
	
	return 0x00;
}

inline t_u8 trctraj_orb_save(s_trctraj_orb *self, s_trctraj_orb_init *attr, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trctraj_orb_load(s_trctraj_orb *self, s_trctraj_orb_init *attr, t_u8 **v_file)
{
	self->eng = attr->eng;
	self->ref = trceng_find_obj(self->eng, self->ref_hash);
	
	return 0x00;
}

inline t_u8 trctraj_orb_compile(s_trctraj_orb *self)
{
	return 0x00;
}

inline t_u8 trctraj_orb_pos(s_trctraj_orb *self, t_f64 time, t_f64 *pos)
{
	switch (self->sync_en)
	{
		default:
		case 0x00:
		{
			t_f64 angle = self->rate * time;
			t_f64 pos_t[3];
			t_f64 pos_n[3];
			
			pos_t[0] = vld_sin(angle) * self->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vld_cos(angle) * self->radius;
			
			vld3m_mulv(pos_n, self->tilt, pos_t);
			
			vl3_vsum(pos, pos_n, self->ref->pos[0]);
			
			return 0x00;
		}
		
		case 0x01:
		{
			t_f64 angle = self->rate * time;
			t_f64 pos_t[3];
			t_f64 pos_n[3];
			
			pos_t[0] = vld_sin(angle) * self->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vld_cos(angle) * self->radius;
			
			vld3m_mulv(pos_n, self->tilt, pos_t);
			vld3m_mulv(pos, self->ref->rot[0], pos_n);
			
			vl3_vsum(pos, pos, self->ref->pos[0]);
			
			return 0x00;
		}
	}
	
	return 0x01;
}

inline t_u8 trctraj_orb_rot(s_trctraj_orb *self, t_f64 time, t_f64 *rot)
{
	switch (self->sync_en)
	{
		default:
		case 0x00:
		{
			t_f64 angle = self->s_rate * time;
			
			t_f64 rot_t[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vl3_rot(rot_t, (s_vl_hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vld3m_mulm(rot, self->s_tilt, rot_t);
			
			return 0x00;
			
			break;
		}
		
		case 0x01:
		{
			t_f64 angle = self->s_rate * time;
			
			t_f64 rot_t[9];
			t_f64 rot_n[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vl3_rot(rot_t, (s_vl_hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vld3m_mulm(rot_n, self->s_tilt, rot_t);
			vld3m_mulm(rot, self->ref->rot[0], rot_n);
			
			return 0x00;
			
			break;
		}
	}
	
	return 0x01;
}

//------------------------------------------------------------------------------

// API
//t_u8 (*init) 		(void *data, void *config)
//t_u8 (*free) 		(void *data);
//t_u8 (*compile) 	(void *data);
//t_u8 (*rot) 		(void *data, t_f64 time, t_f64 *pos);
//t_u8 (*pos) 		(void *data, t_f64 time, t_f64 *rot);

inline t_u8 trctraj_orb_init_ (void **data, void *config)
{
	*data = (s_trctraj_orb*) malloc(sizeof(s_trctraj_orb));
	
	s_trctraj_orb *traj = (s_trctraj_orb*) *data;
	s_trctraj_orb_init *init = (s_trctraj_orb_init*) config;
	
	return trctraj_orb_init(traj, *init);
}

inline t_u8 trctraj_orb_free_ (void **data)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) *data;
	
	free(traj);
	
	return 0x00;
}

inline t_u8 trctraj_orb_save_ (void *data, void *config, t_u8 **v_file)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	s_trctraj_orb_init *attr = (s_trctraj_orb_init*) config;
	
	return trctraj_orb_save(traj, attr, v_file);
}

inline t_u8 trctraj_orb_load_ (void *data, void *config, t_u8 **v_file)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	s_trctraj_orb_init *attr = (s_trctraj_orb_init*) config;
	return trctraj_orb_load(traj, attr, v_file);
}

inline t_u8 trctraj_orb_compile_(void *data)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	return trctraj_orb_compile(traj);
}

inline t_u8 trctraj_orb_pos_(void *data, t_f64 time, t_f64 *pos)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	return trctraj_orb_pos(traj, time, pos);
}

inline t_u8 trctraj_orb_rot_(void *data, t_f64 time, t_f64 *rot)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	return trctraj_orb_rot(traj, time, rot);
}

inline t_u8 trctraj_orb_info_(void *data, s_trctraj_info *info)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = vld_2pi / traj->rate;
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __trctraj__ */
