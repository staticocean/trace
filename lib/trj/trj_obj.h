
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

//------------------------------------------------------------------------------
//
//void trj_obj_ctrl_update(s_trj_obj *obj, vlf_t d_time)
//{
//	obj->time[2] = d_time * d_time * 0.5;
//	obj->time[1] = d_time;
//	obj->time[0] = obj->time[0] + d_time;
//
//	uint32_t i;
//
//	for (i = 0; i < obj->ctrl_offset; ++i)
//	{
//		s_trj_ctrl_api *header = (s_trj_ctrl_api*) obj->ctrl_data[i];
//		header->update(obj->ctrl_data);
//	}
//
//    return;
//}

//------------------------------------------------------------------------------

inline uint8_t trj_obj_init(s_trj_obj *self, s_trj_obj_init attr)
{
	self->id = attr.id;
	self->obj_list = attr.obj_list;
	self->obj_count = attr.obj_count;
	self->ref = attr.ref;
	
	self->traj_offset = 0x00;
	self->ctrl_offset = 0x00;
	self->proc_offset = 0x00;
	self->data_offset = 0x00;
	
	uint32_t i;
	
	for (i = 0; i < sizeof(attr.name); ++i)
	{
		self->name[i] = attr.name[i];
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_obj_traj_add(s_trj_obj *self, void *traj_data)
{
	self->traj_data[self->traj_offset] = traj_data;
	++self->traj_offset;
	
	return 0x00;
}

//------------------------------------------------------------------------------



#endif /* __TRJ_OBJ__ */



























