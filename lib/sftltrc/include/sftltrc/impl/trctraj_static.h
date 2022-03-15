
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_static - trace

#ifndef __TRCTRAJ_STATIC__
#define __TRCTRAJ_STATIC__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trctraj.h>
#include <sftltrc/trcrefs.h>

//------------------------------------------------------------------------------

typedef struct trctraj_static
{
	s_trctraj 		super;
	
	s_trcobj 	   *ref;
	
	t_s8 			ellp_en;
	s_trcrefs 	   *ellp;
	
	t_f64 			pos[3];
	t_f64 			pos_cache[3];
	t_f64 			rot[9];
	t_f64 			rot_cache[9];
	
} 	s_trctraj_static;

typedef struct trctraj_static_attr
{
	s_trctraj_attr 	super;
	
	s_trcobj 		*ref;
	
	t_u8 			ellp_en;
	s_trcrefs  	   *ellp;
	
	t_f64 			pos[3];
	t_f64 			rot[9];
	
} 	s_trctraj_static_attr;

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_init__ (void *__traj__, void *__attr__)
{
	s_trctraj_static 	  *traj = (s_trctraj_static*     ) __traj__;
	s_trctraj_static_attr *attr = (s_trctraj_static_attr*) __attr__;

	traj->ref = attr->ref;

	traj->ellp_en = attr->ellp_en;
	traj->ellp = attr->ellp;
	
	vld3v_copy(traj->pos, attr->pos);
	vld3m_copy(traj->rot, attr->rot);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_free__ (void *__traj__)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_save__ (void *__traj__, s_trcspl *spl, t_u8 **v_file)
{
    s_trctraj_static *traj = (s_trctraj_static*) __traj__;

    return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_load__ (void *__traj__, s_trcspl *spl, t_u8 **v_file)
{
	s_trctraj_static *traj = (s_trctraj_static*) __traj__;
	
	//	traj->eng = attr->eng;
	//	traj->ref = trceng_find_obj(traj->eng, traj->ref_hash);
	//	traj->ellp = trceng_find_ellp(traj->eng, traj->ellp_hash);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_compile__ (void *__traj__)
{
    s_trctraj_static *traj = (s_trctraj_static*) __traj__;

	if (traj->ellp_en != 0x00)
	{
        trcrefs_pos_abs(traj->ellp, traj->pos_cache, traj->pos);
		
		t_f64 ecef_ctn[9];
        trcrefs_rot_abs(traj->ellp, traj->pos_cache, ecef_ctn);
		
		vld3m_mulm(traj->rot_cache, ecef_ctn, traj->rot);
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_pos__ (void *__traj__, t_f64 time, t_f64 *pos)
{
    s_trctraj_static *traj = (s_trctraj_static*) __traj__;

	if (traj->ellp_en == 0x00)
	{ vld3v_copy(pos, traj->pos); }
	
	else
	{ vld3v_copy(pos, traj->pos_cache); }
	
	vld3m_mulv(pos, &traj->ref->rot[0][0], pos);
	vld3v_addv(pos, pos, &traj->ref->pos[0][0]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_rot__ (void *__traj__, t_f64 time, t_f64 *rot)
{
    s_trctraj_static *traj = (s_trctraj_static*) __traj__;

	if (traj->ellp_en == 0x00)
	{ vld3m_copy(rot, traj->rot); }
	
	else
	{ vld3m_copy(rot, traj->rot_cache); }
	
	vld3m_mulm(rot, &traj->ref->rot[0][0], rot);
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCTRAJ_STATIC__ */
