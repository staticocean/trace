
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_orb - trace

#ifndef __TRCTRAJ_ORB__
#define __TRCTRAJ_ORB__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trctraj.h>
#include <sftltrc/trcrefs.h>

//------------------------------------------------------------------------------

typedef struct trctraj_orb
{
    s_trctraj       super;

	s_trcobj 	   *ref;

	t_s8 	        sync_en;
	
	t_f64           radius;
	t_f64           rate;
	t_f64           tilt[9];
	
	t_f64           s_rate;
	t_f64           s_tilt[9];
	
} 	s_trctraj_orb;

typedef struct trctraj_orb_attr
{
    s_trctraj_attr  super;

	s_trcobj 	   *ref;

	t_u8 	        sync_en;
	
	t_f64           radius;
	t_f64           rate;
	t_f64           tilt[9];
	
	t_f64           s_rate;
	t_f64           s_tilt[9];
	
} 	s_trctraj_orb_attr;

//------------------------------------------------------------------------------

static
t_s8 __trctraj_orb_init__ (void *__traj__, void *__attr__)
{
    s_trctraj_orb 	   *traj = (s_trctraj_orb*     ) __traj__;
    s_trctraj_orb_attr *attr = (s_trctraj_orb_attr*) __attr__;

    traj->ref = attr->ref;

    traj->sync_en  = attr->sync_en;

    traj->radius = attr->radius;
    traj->rate   = attr->rate;
	
	vld3m_copy(traj->tilt, attr->tilt);

    traj->s_rate = attr->s_rate;
	vld3m_copy(traj->s_tilt, attr->s_tilt);
	
	return 0x00;
}

static
t_s8 __trctraj_orb_free__ (void *__traj__)
{
    return 0x00;
}

static
t_s8 __trctraj_orb_save__ (void *__traj__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

static
t_s8 __trctraj_orb_load__ (void *__traj__, s_trcspl *spl, t_u8 **v_file)
{
    s_trctraj_orb *traj = (s_trctraj_orb*) __traj__;

//    traj->ref = trceng_find_obj(self->eng, self->ref_hash);
	
	return 0x00;
}

static
t_s8 __trctraj_orb_compile__ (void *__traj__)
{
	return 0x00;
}

static
t_s8 __trctraj_orb_pos__ (void *__traj__, t_f64 time, t_f64 *pos)
{
    s_trctraj_orb *traj = (s_trctraj_orb*) __traj__;

	switch (traj->sync_en)
	{
		default:
		case 0x00:
		{
			t_f64 angle = traj->rate * time;
			t_f64 pos_t[3];
			t_f64 pos_n[3];
			
			pos_t[0] = vld_sin(angle) * traj->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vld_cos(angle) * traj->radius;
			
			vld3m_mulv(pos_n, traj->tilt, pos_t);
			
			vld3v_addv(pos, pos_n, traj->ref->pos[0]);
			
			return 0x00;
		}
		
		case 0x01:
		{
			t_f64 angle = traj->rate * time;
			t_f64 pos_t[3];
			t_f64 pos_n[3];
			
			pos_t[0] = vld_sin(angle) * traj->radius;
			pos_t[1] = 0.0;
			pos_t[2] = vld_cos(angle) * traj->radius;
			
			vld3m_mulv(pos_n, traj->tilt, pos_t);
			vld3m_mulv(pos, traj->ref->rot[0], pos_n);
			
			vld3v_addv(pos, pos, traj->ref->pos[0]);
			
			return 0x00;
		}
	}
	
	return 0x01;
}

static
t_s8 __trctraj_orb_rot__ (void *__traj__, t_f64 time, t_f64 *rot)
{
    s_trctraj_orb *traj = (s_trctraj_orb*) __traj__;

    switch (traj->sync_en)
	{
		default:
		case 0x00:
		{
			t_f64 angle = traj->s_rate * time;
			
			t_f64 rot_t[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vld3m_rot(rot_t, (s_vl3hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vld3m_mulm(rot, traj->s_tilt, rot_t);
			
			return 0x00;
			
			break;
		}
		
		case 0x01:
		{
			t_f64 angle = traj->s_rate * time;
			
			t_f64 rot_t[9];
			t_f64 rot_n[9];
			
			// NEGATIVE HEADING!!! because heading is CW and all angles are CCW
			vld3m_rot(rot_t, (s_vl3hpr) {
					.heading = -angle, .pitch = 0.0, .roll = 0.0
			});
			
			vld3m_mulm(rot_n, traj->s_tilt, rot_t);
			vld3m_mulm(rot, traj->ref->rot[0], rot_n);
			
			return 0x00;
			
			break;
		}
	}
	
	return 0x01;
}

//inline t_u8 trctraj_orb_info_(void *data, s_trctraj_info *info)
//{
//	s_trctraj_orb *traj = (s_trctraj_orb*) data;
//
//	info->preview_time[0] = 0.0;
//	info->preview_time[1] = vld_2pi / traj->rate;
//
//	return 0x00;
//}

//------------------------------------------------------------------------------

s_trctraj_intf __trctraj_orb__ = {

        .desc    = "trctraj_orb",

        .data_sz = sizeof(s_trctraj_orb),
        .attr_sz = sizeof(s_trctraj_orb_attr),

        .init    = __trctraj_orb_init__,
        .free    = __trctraj_orb_free__,
        .save    = __trctraj_orb_save__,
        .load    = __trctraj_orb_load__,
        .compile = __trctraj_orb_compile__,
        .pos     = __trctraj_orb_pos__,
        .rot     = __trctraj_orb_rot__,
};

//------------------------------------------------------------------------------

#endif /* __TRCTRAJ_ORB__ */
