
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_OBJ__
#define __TRJ_OBJ__

//------------------------------------------------------------------------------

#include "vl.h"
#include "trj_api.h"

//------------------------------------------------------------------------------

typedef struct trj_obj_init
{
	uint32_t id;
	s_trj_obj *obj_list;
	uint32_t *obj_count;
	char name[32];
	struct trj_obj *ref;
	
} s_trj_obj_init;


//------------------------------------------------------------------------------

inline void trj_obj_print(s_trj_obj *obj)
{
	printf(vl_lsep);
	printf("name        [%s] \r\n", obj->name);
	printf("id          [%06X] \r\n", obj->id);
//	printf("ref         [%s] \r\n", obj->ref->name);
	printf("traj_offset [%d] \r\n", obj->traj_offset);
	printf("ctrl_offset [%d] \r\n", obj->ctrl_offset);
	printf("data_offset [%d] \r\n", obj->data_offset);
	
	uint32_t i;
	
	printf(vl_lsep);
	printf("traj_list \r\n");
	
	for (i = 0; i < obj->traj_offset; ++i)
	{ printf("- [%02d] %s \r\n", i, obj->traj_list[i].desc); }
	
	printf(vl_lsep);
	printf("ctrl_list \r\n");
	
	for (i = 0; i < obj->ctrl_offset; ++i)
	{ printf("- [%02d] %s \r\n", i, obj->ctrl_list[i].desc); }
	
	
	return;
}

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
//	self->ref = attr.ref;
	
	self->traj_offset = 0x00;
	self->ctrl_offset = 0x00;
	self->data_offset = 0x00;
	
	self->log_list = NULL;
	self->log_offset = 0x00;
	
	uint32_t i;
	
	for (i = 0; i < sizeof(attr.name); ++i)
	{
		self->name[i] = attr.name[i];
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_OBJ__ */



























