
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_API__
#define __TRJ_API__

//------------------------------------------------------------------------------

#include "vl.h"

//------------------------------------------------------------------------------

typedef struct trj_traj
{
	char desc[32];
	
	void *data;
	void *config;
	
	void *init;
	void *free;
	void *compile;
	void *rot;
	void *pos;
	
} 	s_trj_traj;

//------------------------------------------------------------------------------

typedef struct trj_ctrl
{
	char desc[32];
	
	void *reset;
	void *update;
	
} 	s_trj_ctrl;

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
	
	s_trj_traj traj_list[8];
	s_trj_ctrl ctrl_list[8];
	void *proc_list[8];
	void *data_list[8];
	
	uint8_t traj_offset;
	uint8_t ctrl_offset;
	uint8_t proc_offset;
	uint8_t data_offset;
	
} 	s_trj_obj;

//------------------------------------------------------------------------------

#endif /* __TRJ_API__ */

