
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_OBJ__
#define __TRJ_OBJ__

//------------------------------------------------------------------------------

#include "vl.h"

//------------------------------------------------------------------------------

typedef struct trj_obj
{
	struct trj_obj *obj_list;
	uint32_t *obj_count;
	
    struct trj_obj *ref;

    uint32_t id;
    char name[32];

    vlf_t time[3];
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t pos_inert;
	vlf_t rot_inert;
    
	vlf_t pos_force[3];
	vlf_t rot_force[3];
	
	void *traj_data[255];
	void *ctrl_data[255];
	void *proc_data[255];
	void *data_data[255];
	
	uint8_t traj_offset;
	uint8_t ctrl_offset;
	uint8_t proc_offset;
	uint8_t data_offset;
	
} 	s_trj_obj;

typedef struct trj_obj_init
{
	uint32_t id;
	s_trj_obj *obj_list;
	uint32_t *obj_count;
	char name[32];
	struct trj_obj *ref;
	
} s_trj_obj_init;


//------------------------------------------------------------------------------

uint8_t trj_obj_init(s_trj_obj *obj, s_trj_obj_init attr);
void trj_obj_ctrl_update(s_trj_obj *obj, vlf_t d_time);

//------------------------------------------------------------------------------

#endif /* __TRJ_OBJ__ */



























