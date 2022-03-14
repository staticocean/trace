
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrefs_ellp - ellipsoid refernce frame

#ifndef __TRCREFS_EPZ9011__
#define __TRCREFS_EPZ9011__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcrefs.h>
#include <sftltrc/impl/trcrefs_ellp.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_epz9011
{
    s_trcrefs           super;

    s_trcrefs_ellp      ellp;

}	s_trcrefs_epz9011;

typedef struct trcrefs_epz9011_attr
{
    s_trcrefs_attr      super;

}	s_trcrefs_epz9011_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcrefs_epz9011_init__ (void *__refs__, void *__attr__)
{
    s_trcrefs_epz9011 *ellp = (s_trcrefs_epz9011*) __refs__;
    s_trcrefs_epz9011_attr *attr = (s_trcrefs_epz9011_attr*) __attr__;

    s_trcrefs_ellp_attr ellp_attr = {
            .a = 6378136,
            .f = 0.0033528037345079697573169141833205,
    };

    trcrefs_ellp_init(&ellp->ellp, &ellp_attr);

    return 0x00;
}

static
t_s8 __trcrefs_epz9011_free__ (void *__refs__)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_save__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_load__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_pos_abs__ (void *__refs__, t_f64 *pos_abs, t_f64 *pos_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_pos_ref__ (void *__refs__, t_f64 *pos_ref, t_f64 *pos_abs)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_vel_abs__ (void *__refs__, t_f64 *vel_abs, t_f64 *vel_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_vel_ref__ (void *__refs__, t_f64 *vel_ref, t_f64 *vel_abs)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_rot_abs__ (void *__refs__, t_f64 *rot_abs, t_f64 *rot_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9011_rot_ref__ (void *__refs__, t_f64 *rot_ref, t_f64 *rot_abs)
{
    return 0x00;
}

//------------------------------------------------------------------------------

s_trcrefs_intf __trcrefs_epz9011__ = {

        .desc 		= "trcrefs_epz9011",

        .data_sz 	= sizeof(s_trcrefs_epz9011),
        .attr_sz 	= sizeof(s_trcrefs_epz9011_attr),

        .init 		= __trcrefs_epz9011_init__,
        .free 		= __trcrefs_epz9011_free__,
        .save 		= __trcrefs_epz9011_save__,
        .load 		= __trcrefs_epz9011_load__,
        .pos_abs    = __trcrefs_epz9011_pos_abs__,
        .pos_ref    = __trcrefs_epz9011_pos_ref__,
        .vel_abs    = __trcrefs_epz9011_vel_abs__,
        .vel_ref    = __trcrefs_epz9011_vel_ref__,
        .rot_abs    = __trcrefs_epz9011_rot_abs__,
        .rot_ref    = __trcrefs_epz9011_rot_ref__,

        .gui_attr 	= NULL,
        .gui_edit 	= NULL,
        .gui_view 	= NULL,
};

//------------------------------------------------------------------------------

#endif /* __TRCREFS_EPZ9011__ */
