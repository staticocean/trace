
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrefs_ellp - ellipsoid refernce frame

#ifndef __TRCREFS_EWGS84__
#define __TRCREFS_EWGS84__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcrefs.h>
#include <sftltrc/impl/trcrefs_ellp.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_ewgs84
{
    s_trcrefs           super;

    s_trcrefs_ellp      ellp;

}	s_trcrefs_ewgs84;

typedef struct trcrefs_ewgs84_attr
{
    s_trcrefs_attr      super;

}	s_trcrefs_ewgs84_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcrefs_ewgs84_init__ (void *__refs__, void *__attr__)
{
    s_trcrefs_ewgs84 *ellp = (s_trcrefs_ewgs84*) __refs__;
    s_trcrefs_ewgs84_attr *attr = (s_trcrefs_ewgs84_attr*) __attr__;

    s_trcrefs_ellp_attr ellp_attr = {
            .a = 6.37813700000000000000e+0006,
            .f = 3.35281066474748071998e-0003,
    };

    trcrefs_ellp_init(&ellp->ellp, &ellp_attr);

    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_free__ (void *__refs__)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_save__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_load__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_pos_abs__ (void *__refs__, t_f64 *pos_abs, t_f64 *pos_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_pos_ref__ (void *__refs__, t_f64 *pos_ref, t_f64 *pos_abs)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_vel_abs__ (void *__refs__, t_f64 *vel_abs, t_f64 *vel_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_vel_ref__ (void *__refs__, t_f64 *vel_ref, t_f64 *vel_abs)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_rot_abs__ (void *__refs__, t_f64 *rot_abs, t_f64 *rot_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_ewgs84_rot_ref__ (void *__refs__, t_f64 *rot_ref, t_f64 *rot_abs)
{
    return 0x00;
}

//------------------------------------------------------------------------------

s_trcrefs_intf __trcrefs_ewgs84__ = {

        .desc 		= "trcrefs_ewgs84",

        .data_sz 	= sizeof(s_trcrefs_ewgs84),
        .attr_sz 	= sizeof(s_trcrefs_ewgs84_attr),

        .init 		= __trcrefs_ewgs84_init__,
        .free 		= __trcrefs_ewgs84_free__,
        .save 		= __trcrefs_ewgs84_save__,
        .load 		= __trcrefs_ewgs84_load__,
        .pos_abs    = __trcrefs_ewgs84_pos_abs__,
        .pos_ref    = __trcrefs_ewgs84_pos_ref__,
        .vel_abs    = __trcrefs_ewgs84_vel_abs__,
        .vel_ref    = __trcrefs_ewgs84_vel_ref__,
        .rot_abs    = __trcrefs_ewgs84_rot_abs__,
        .rot_ref    = __trcrefs_ewgs84_rot_ref__,

        .gui_attr 	= NULL,
        .gui_edit 	= NULL,
        .gui_view 	= NULL,
};

//------------------------------------------------------------------------------

#endif /* __TRCREFS_EWGS84__ */
