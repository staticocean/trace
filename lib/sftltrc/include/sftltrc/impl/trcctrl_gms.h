
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __TRCCTRL_GMS__
#define __TRCCTRL_GMS__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcctrl.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl_gms
{
    s_trcctrl           super;

    s_trcobj 	       *ref;

}   s_trcctrl_gms;

typedef struct trcctrl_gms_attr
{
    s_trcctrl_attr      super;

    s_trcobj 		   *ref;

}   s_trcctrl_gms_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcctrl_gms_init__ (void *__ctrl__, void *__attr__)
{
    s_trcctrl_gms *ctrl = (s_trcctrl_gms*) __ctrl__;
    s_trcctrl_gms_attr *attr = (s_trcctrl_gms_attr *) __attr__;

    ctrl->ref = attr->ref;

    return 0x00;
}

static
t_s8 __trcctrl_gms_free__ (void *__ctrl__)
{
    return 0x00;
}

static
t_s8 __trcctrl_gms_save__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcctrl_gms_load__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
    s_trcctrl_gms *ctrl = (s_trcctrl_gms*) __ctrl__;

//    ctrl->ref = trceng_find_obj (self->eng, self->ref_hash);

    return 0x00;
}

static inline
void __trcctrl_gms_calc__ (s_trcobj *ref, s_trcobj *obj)
{
    const t_f64 g = 6.67428E-11;

    t_f64 force_magn = 0.0;
    t_f64 dist2 = vld3v_dist2(&ref->pos[0][0], &obj->pos[0][0]);

    if (dist2 > 1E-16)
    { force_magn = g * ref->pos_inert * obj->pos_inert / dist2; }

    t_f64 force_dir[3];
    vld3v_set(force_dir, 0.0);

    if (dist2 > 1E-16)
    {
        vld3v_subv(force_dir, &ref->pos[0][0], &obj->pos[0][0]);
        vld3v_muls(force_dir, force_dir, force_magn / vld_sqrt(dist2));
    }

    vld3v_addv(obj->pos_force, obj->pos_force, force_dir);
}

static
t_s8 __trcctrl_gms_reset__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_gms *ctrl = (s_trcctrl_gms*) __ctrl__;
    __trcctrl_gms_calc__(ctrl->ref, obj);

    return 0x00;
}

static
t_s8 __trcctrl_gms_update__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_gms *ctrl = (s_trcctrl_gms*) __ctrl__;
    __trcctrl_gms_calc__(ctrl->ref, obj);

    return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCCTRL_GMS__ */

