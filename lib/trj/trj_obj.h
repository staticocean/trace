
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __INS_OBJ__
#define __INS_OBJ__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

//------------------------------------------------------------------------------

typedef struct trj_obj
{
    struct trj_obj *ref;

    uint32_t id;
    uint8_t  name[255];

    vlf_t time[3];
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t pos_inert;
	vlf_t rot_inert;
    
	vlf_t pos_force[3];
	vlf_t rot_force[3];
	
	uint8_t traj_offset;
	uint8_t ctrl_offset;
	uint8_t proc_offset;
	uint8_t data_offset;
    
    void *ctrl_data;
    void (*ctrl_update) (void *ctrl_data);
    
    void *proc_data;
    void (*proc_update) (void *proc_data);
    
} 	s_trj_obj;

//------------------------------------------------------------------------------

void trj_obj_update(s_trj_obj *obj, vlf_t d_time);

//------------------------------------------------------------------------------

#endif /* __INS_OBJ__ */



























