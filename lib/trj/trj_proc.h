
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __INS_PROC__
#define __INS_PROC__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

#include "trj_utils.h"
#include "trj_obj.h"

//------------------------------------------------------------------------------

typedef struct ins_proc_euler
{
    s_trj_obj *parent;
    
    uint64_t offset;
    vlf_t pos_0[4][3];
    vlf_t rot_0[4][9];
    vlf_t time_0[4];
    
//    vlf_t pos_0[3];
//    vlf_t pos_1[3];
//    vlf_t pos_2[3];
    
}   ins_proc_euler_t;

//------------------------------------------------------------------------------

#endif /* __INS_PROC__ */


