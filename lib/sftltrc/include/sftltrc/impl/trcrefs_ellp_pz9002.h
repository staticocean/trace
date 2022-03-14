
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrefs_ellp - ellipsoid refernce frame

#ifndef __TRCREFS_ELLP_PZ9002__
#define __TRCREFS_ELLP_PZ9002__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcrefs.h>
#include <sftltrc/impl/trcrefs_ellp.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_ellp_pz9002
{
    s_trcrefs           super;

    s_trcrefs_ellp      ellp;

}	s_trcrefs_ellp_pz9002;

typedef struct trcrefs_ellp_pz9002_attr
{
    s_trcrefs_attr      super;

}	s_trcrefs_ellp_pz9002_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcrefs_ellp_pz9002_init__ (void *__refs__, void *__attr__)
{
    s_trcrefs_ellp_pz9002 *ellp = (s_trcrefs_ellp_pz9002*) __refs__;
    s_trcrefs_ellp_pz9002_attr *attr = (s_trcrefs_ellp_pz9002_attr*) __attr__;

    s_trcrefs_ellp_attr ellp_attr = {
        .a 		=  6378136,
        .f  	=  3.35280374301947673491022158624e-0003,
    };

    trcrefs_ellp_init(&ellp->ellp, &ellp_attr);

    return 0x00;
}

static
t_s8 __trcrefs_ellp_pz9002_free__ (void *__refs__)
{
    return 0x00;
}

static
t_s8 __trcrefs_ellp_pz9002_save__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_ellp_pz9002_load__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcrefs_ellp_pz9002_pos_abs__ (void *__refs__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

//t_s8 (*pos_abs) 	(void *refs, t_f64 *abs, t_f64 *ref);
//t_s8 (*pos_ref) 	(void *refs, t_f64 *ref, t_f64 *abs);
//t_s8 (*vel_abs) 	(void *refs, t_f64 *abs, t_f64 *ref);
//t_s8 (*vel_ref) 	(void *refs, t_f64 *ref, t_f64 *abs);
//t_s8 (*rot_abs) 	(void *refs, t_f64 *abs, t_f64 *ref);
//t_s8 (*rot_ref) 	(void *refs, t_f64 *ref, t_f64 *abs);

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#endif /* __TRCREFS_ELLP_PZ9002__ */
