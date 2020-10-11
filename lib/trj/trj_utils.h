
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_UTILS__
#define __TRJ_UTILS__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

#include "trj_obj.h"

//------------------------------------------------------------------------------

typedef struct trj_pos_lla
{
    vlf_t lat;
    vlf_t lon;
    vlf_t alt;
    
}   trj_pos_lla_t;

typedef struct trj_pos_ecef
{
    vlf_t vec[3];
    
}   trj_pos_ecef_t;

typedef struct insu_rot_hpr
{
    vlf_t heading;
    vlf_t pitch;
    vlf_t roll;
    
}   s_trj_rot_hpr;

void trj_gforce         (vlf_t *res, s_trj_obj *obj_0, s_trj_obj *obj_1);
void trj_ecef_to_lla    (trj_pos_lla_t *pos_lla, trj_pos_ecef_t *pos_ecef);
void trj_lla_to_ecef    (trj_pos_ecef_t *pos_ecef, trj_pos_lla_t *pos_lla);
void trj_ctn_to_hpr     (s_trj_rot_hpr *rot_hpr, vlf_t *c_tn);
void trj_hpr_to_ctn     (vlf_t *c_tn, s_trj_rot_hpr *rot_hpr);
void trj_pos_ecef       (trj_pos_ecef_t *ecef, s_trj_obj *obj);

//------------------------------------------------------------------------------

#endif /* __TRJ_UTILS__ */
