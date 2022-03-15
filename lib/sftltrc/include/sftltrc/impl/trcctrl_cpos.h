
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __TRCCTRL_CPOS__
#define __TRCCTRL_CPOS__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcctrl.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl_cpos
{
    s_trcctrl               super;
	
}   s_trcctrl_cpos;

typedef struct trcctrl_cpos_attr
{
    s_trcctrl_attr          super;
	
}   s_trcctrl_cpos_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcctrl_cpos_init__ (void *__ctrl__, void *__attr__)
{
	return 0x00;
}

static
t_s8 __trcctrl_cpos_free__ (void *__ctrl__)
{
    return 0x00;
}

static
t_s8 __trcctrl_cpos_save__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

static
t_s8 __trcctrl_cpos_load__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

static
t_s8 __trcctrl_cpos_reset__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_cpos *ctrl = (s_trcctrl_cpos*) __ctrl__;

//	if (obj->traj_sz > 0x00)
//	{
//		s_trctraj *traj = &obj->traj_ls[0];
//		traj->pos(traj->data, 0.0, &obj->pos[0][0]);
//	}
	
	return 0x00;
}

static
t_s8 __trcctrl_cpos_update__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_cpos *ctrl = (s_trcctrl_cpos*) __ctrl__;

//	if (obj->traj_sz > 0x00)
//	{
//		s_trctraj *traj = &obj->traj_ls[0];
//		traj->pos(traj->data, *obj->time, &obj->pos[0][0]);
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCCTRL_CPOS__ */

