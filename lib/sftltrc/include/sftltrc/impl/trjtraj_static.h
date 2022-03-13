
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_static - trace

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trctraj.h>
#include <sftltrc/trcspl.h>
#include <sftltrc/trcrefs.h>

//------------------------------------------------------------------------------

typedef struct trctraj_static
{
	s_trctraj 		super;
	
	s_trcobj 		*ref;
	t_u32 			ref_hash;
	
	t_u8 			ellp_en;
	s_trcellp 		*ellp;
	t_u32 			ellp_hash;
	
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
	s_trcellp  		*ellp;
	
	t_f64 			pos[3];
	t_f64 			rot[9];
	
} 	s_trctraj_static_attr;

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_init__ (void *__traj__, s_trctraj_static_init *__attr__)
{
	s_trctraj_static 	  *traj = (s_trctraj_static*     ) *__traj__;
	s_trctraj_static_attr *attr = (s_trctraj_static_attr*) __attr__;

	traj->eng = attr->eng;
	
	traj->ref = attr->ref;
	traj->ref_hash = traj->ref->hash;
	
	traj->ellp_en = attr->ellp_en;
	traj->ellp = attr->ellp;
	if (traj->ellp != NULL)
	{ traj->ellp_hash = self->ellp->hash; }
	
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
t_s8 __trctraj_static_save__ (void *__traj__, t_u8 **v_file)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_load__ (void *__traj__, t_u8 **v_file)
{
	s_trctraj_static *traj = (s_trctraj_static*) __traj__;
	
	//	traj->eng = attr->eng;
	//	traj->ref = trceng_find_obj(traj->eng, traj->ref_hash);
	//	traj->ellp = trceng_find_ellp(traj->eng, traj->ellp_hash);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_compile__ (s_trctraj *traj)
{
	if (self->ellp_en != 0x00)
	{
		trcellp_ecef(self->ellp, self->pos_cache, self->pos);
		
		t_f64 ecef_ctn[9];
		trcellp_ecefrot(self->ellp, self->pos_cache, ecef_ctn);
		
		vld3m_mulm(self->rot_cache, ecef_ctn, self->rot);
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_pos__ (s_trctraj *traj, t_f64 time, t_f64 *pos)
{
	if (self->ellp_en == 0x00)
	{ vld3v_copy(pos, self->pos); }
	
	else
	{ vld3v_copy(pos, self->pos_cache); }
	
	vld3m_mulv(pos, &self->ref->rot[0][0], pos);
	vl3_vsum(pos, pos, &self->ref->pos[0][0]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_static_rot__ (s_trctraj *traj, t_f64 time, t_f64 *rot)
{
	if (self->ellp_en == 0x00)
	{ vld3m_copy(rot, self->rot); }
	
	else
	{ vld3m_copy(rot, self->rot_cache); }
	
	vld3m_mulm(rot, &self->ref->rot[0][0], rot);
	
	return 0x00;
}

//------------------------------------------------------------------------------

s_trctraj_intf __trctraj_static__ = {
		.guid    = "trctraj_static",
		
		.data_sz = sizeof(s_trctraj_static),
		.attr_sz = sizeof(s_trctraj_static_attr),
		
		.init    = __trctraj_static_init__,
		.free    = __trctraj_static_free__,
		.pack    = __trctraj_static_pack__,
		.unpack  = __trctraj_static_unpack__,
		.save    = __trctraj_static_save__,
		.load    = __trctraj_static_load__,
		.compile = __trctraj_static_compile__,
		.pos     = __trctraj_static_pos__,
		.rot     = __trctraj_static_rot__,
};

//------------------------------------------------------------------------------

t_s8 trctraj_static_init (s_trctraj_static **traj, void *attr)
{
	*traj = malloc(sizeof(s_trctraj_static));
	
	(*traj)->intf = &__trctraj_static__;
	
	trctraj_init(&(*traj)->super, &attr->super);
}

//------------------------------------------------------------------------------

#endif /* __trctraj__ */
