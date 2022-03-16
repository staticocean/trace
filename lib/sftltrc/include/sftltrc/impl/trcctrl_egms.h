
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __TRCCTRL_EGMS__
#define __TRCCTRL_EGMS__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcctrl.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl_egms
{
    s_trcctrl               super;

    s_trcobj               *ref;

}   s_trcctrl_egms;

typedef struct trcctrl_egms_attr
{
    s_trcctrl_attr          super;

    s_trcobj               *ref;

}   s_trcctrl_egms_attr;

static
t_s8 __trcctrl_egms_init__ (void *__ctrl__, void *__attr__)
{
    s_trcctrl_egms *ctrl = (s_trcctrl_egms*) __ctrl__;
    s_trcctrl_egms_attr *attr = (s_trcctrl_egms_attr *) __attr__;

    ctrl->ref = attr->ref;

    return 0x00;
}

static
t_s8 __trcctrl_egms_free__ (void *__ctrl__)
{
    return 0x00;
}

static
t_s8 __trcctrl_egms_save__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcctrl_egms_load__ (void *__ctrl__, s_trcspl *spl, t_u8 **v_file)
{
    s_trcctrl_egms *ctrl = (s_trcctrl_egms*) __ctrl__;

//    ctrl->ref = trceng_find_obj (self->eng, self->ref_hash);

    return 0x00;
}

static inline
void __trcctrl_egms_calc__ (s_trcobj *ref, s_trcobj *obj)
{
//    const t_f64 beta  = 0.0053171;
//    const t_f64 beta1 = 71E-7;
//    const t_f64 q     = 0.00346775;
//    const t_f64 ge    = 9.78049;
//    const t_f64 a     = 6378136;
//    const t_f64 e2    = 0.0066943662;
//
//    t_f64 ecef[3];
//
//    vld3v_subv(ecef, &obj->pos[0][0], &ref->pos[0][0]);
//    vld3m_tmulv(ecef, &ref->rot[0][0], ecef);
//
//    t_f64 lla[3];
//
//    trcellp_lla(&trcellp_pz90, lla, ecef);
//
//    t_f64 g0 = ge*(1 + beta*pow(sin(lla[0]),2) + beta1*pow(sin(2*lla[0]),2));
//
//    t_f64 g_hor[3] = {
//            g0*sin(2*lla[0])*(lla[2]/a)*((e2)/a - 2*q),
//            g0 + (lla[2]/a)*((3*lla[2]/a) - 2*q*ge*pow(cos(lla[0]),2)
//                             + (e2)*(3*pow(sin(lla[0]),2) - 1) - q*(1+6*pow(sin(lla[0]),2))),
//            0.0
//    };
//
//    vld3v_muls(g_hor, g_hor, -1.0);
//
//    t_f64 ecef_ctn[9];
//    trcellp_ecefrot(&trcellp_pz90, ecef, ecef_ctn);
//
//    t_f64 g_ecef[3];
//    vld3m_mulv(g_ecef, ecef_ctn, g_hor);
//
//    t_f64 g_inert[3];
//    vld3m_mulv(g_inert, &ref->rot[0][0], g_ecef);
//    vld3v_muls(g_inert, g_inert, 1.0 * obj->pos_inert);
//
//    vl3_vsum(obj->pos_force, obj->pos_force, g_inert);
}

static
t_s8 __trcctrl_egms_reset__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_egms *ctrl = (s_trcctrl_egms*) __ctrl__;
    __trcctrl_egms_calc__(ctrl->ref, obj);

    return 0x00;
}

static
t_s8 __trcctrl_egms_update__ (void *__ctrl__, s_trcobj *obj)
{
    s_trcctrl_egms *ctrl = (s_trcctrl_egms*) __ctrl__;
    __trcctrl_egms_calc__(ctrl->ref, obj);

    return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCCTRL_EGMS__ */

