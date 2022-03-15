
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __TRCCTRL_CROT__
#define __TRCCTRL_CROT__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcctrl.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl_crot
{
    s_trcctrl               super;
	
}   s_trcctrl_crot;

typedef struct trcctrl_crot_attr
{
    s_trcctrl_attr          super;
	
}   s_trcctrl_crot_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcctrl_crot_init__ (void *__ctrl__, void *__attr__)
{
	return 0x00;
}

static
t_s8 __trcctrl_crot_free__ (void *__ctrl__)
{
    return 0x00;
}

static
t_s8 __trcctrl_crot_save__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

static
t_s8 __trcctrl_crot_load__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

static
t_s8 __trcctrl_crot_reset__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_crot *ctrl = (s_trcctrl_crot*) __ctrl__;

//    if (obj->traj_sz > 0x00)
//    {
//        s_trctraj *traj = &obj->traj_ls[0];
//        traj->rot(traj->data, 0.0, &obj->rot[0][0]);
//    }

    return 0x00;
}

static
t_s8 __trcctrl_crot_update__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_crot *ctrl = (s_trcctrl_crot*) __ctrl__;

//    if (obj->traj_sz > 0x00)
//    {
//        s_trctraj *traj = &obj->traj_ls[0];
//        traj->rot(traj->data, *obj->time, &obj->rot[0][0]);
//    }

    return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCCTRL_CROT__ */

