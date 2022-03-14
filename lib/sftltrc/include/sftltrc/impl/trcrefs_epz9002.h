
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrefs_ellp - ellipsoid refernce frame

#ifndef __TRCREFS_EPZ9002__
#define __TRCREFS_EPZ9002__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcrefs.h>
#include <sftltrc/impl/trcrefs_ellp.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_epz9002
{
    s_trcrefs           super;

    s_trcrefs_ellp      ellp;

}	s_trcrefs_epz9002;

typedef struct trcrefs_epz9002_attr
{
    s_trcrefs_attr      super;

}	s_trcrefs_epz9002_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcrefs_epz9002_init__ (void *__refs__, void *__attr__)
{
    s_trcrefs_epz9002 *ellp = (s_trcrefs_epz9002*) __refs__;
    s_trcrefs_epz9002_attr *attr = (s_trcrefs_epz9002_attr*) __attr__;

    s_trcrefs_ellp_attr ellp_attr = {
            .a = 6378136,
            .f = 3.35280374301947673491022158624e-0003,
    };

    trcrefs_ellp_init(&ellp->ellp, &ellp_attr);

    return 0x00;
}

static
t_s8 __trcrefs_epz9002_free__ (void *__refs__)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_save__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_load__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_pos_abs__ (void *__refs__, t_f64 *pos_abs, t_f64 *pos_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_pos_ref__ (void *__refs__, t_f64 *pos_ref, t_f64 *pos_abs)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_vel_abs__ (void *__refs__, t_f64 *vel_abs, t_f64 *vel_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_vel_ref__ (void *__refs__, t_f64 *vel_ref, t_f64 *vel_abs)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_rot_abs__ (void *__refs__, t_f64 *rot_abs, t_f64 *rot_ref)
{
    return 0x00;
}

static
t_s8 __trcrefs_epz9002_rot_ref__ (void *__refs__, t_f64 *rot_ref, t_f64 *rot_abs)
{
    return 0x00;
}

//------------------------------------------------------------------------------

s_trcrefs_intf __trcrefs_epz9002__ = {

        .desc 		= "trcrefs_epz9002",

        .data_sz 	= sizeof(s_trcrefs_epz9002),
        .attr_sz 	= sizeof(s_trcrefs_epz9002_attr),

        .init 		= __trcrefs_epz9002_init__,
        .free 		= __trcrefs_epz9002_free__,
        .save 		= __trcrefs_epz9002_save__,
        .load 		= __trcrefs_epz9002_load__,
        .pos_abs    = __trcrefs_epz9002_pos_abs__,
        .pos_ref    = __trcrefs_epz9002_pos_ref__,
        .vel_abs    = __trcrefs_epz9002_vel_abs__,
        .vel_ref    = __trcrefs_epz9002_vel_ref__,
        .rot_abs    = __trcrefs_epz9002_rot_abs__,
        .rot_ref    = __trcrefs_epz9002_rot_ref__,

        .gui_attr 	= NULL,
        .gui_edit 	= NULL,
        .gui_view 	= NULL,
};

//------------------------------------------------------------------------------

#endif /* __TRCREFS_EPZ9002__ */
