
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_ENG__
#define __TRJ_ENG__

//------------------------------------------------------------------------------

#include <string.h>

#include "trj_types.h"
#include "vl.h"

#include "trj_obj.h"

//------------------------------------------------------------------------------

typedef struct trj_eng
{
	uint32_t obj_count;
	s_trj_obj *obj_list;
	
}	s_trj_eng;

typedef struct trj_eng_init_attr
{
	s_trj_obj *st_objects;
	
}	s_trj_eng_init;

//------------------------------------------------------------------------------

inline void trj_eng_print(s_trj_eng *eng)
{
	printf("obj_count %d \r\n", eng->obj_count);
	
	printf("obj_list \r\n");
	
	uint32_t i;
	
	for (i = 0; i < eng->obj_count; ++i)
	{ printf("%s \r\n", eng->obj_list[i].name); }
	
	return;
}

inline uint8_t trj_eng_init(s_trj_eng *self, s_trj_eng_init attr)
{
	self->obj_count = 0x00;
	self->obj_list = attr.st_objects;
	
	return 0x00;
}

inline uint8_t trj_eng_add(s_trj_eng *self, s_trj_obj_init attr)
{
	trj_obj_init(&self->obj_list[self->obj_count], attr);
	
	self->obj_list[self->obj_count].id = self->obj_count;
	self->obj_list[self->obj_count].obj_list = self->obj_list;
	self->obj_list[self->obj_count].obj_count = &self->obj_count;
	
	self->obj_count++;
	
	return 0x00;
}

inline uint8_t trj_eng_objnames(s_trj_eng *self, char **objnames)
{
	uint32_t i;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		objnames[i] = self->obj_list[i].name;
	}
	
	return 0x00;
}


//void ins_engine_update_forces(trj_eng_t *engine)
//{
//    uint8_t i;
//    uint8_t j;
//
//    vlf_t gforce[3] = { 0, 0, 0 };
//
//    for (i = 0; i < engine->obj_count; ++i)
//    {
//        for (j = i+1; j < engine->obj_count; ++j)
//        {
//            trj_gforce(gforce, engine->obj_list[i], engine->obj_list[j]);
//
//            vl_vsub(engine->obj_list[i]->pos_force, engine->obj_list[i]->pos_force, gforce);
//            vl_vsum(engine->obj_list[j]->pos_force, engine->obj_list[j]->pos_force, gforce);
//        }
//    }
//
//    return;
//}

inline uint8_t trj_eng_reset(s_trj_eng *self)
{
	uint32_t i;
	uint32_t j;
	
	s_trj_obj *obj;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		
		obj->time[0] = 0.0;
		obj->time[1] = 0.0;
		obj->time[2] = 0.0;
		
		for (j = 0; j < obj->traj_offset; ++j)
		{
			obj->traj_list[j].compile(obj->traj_list[j].data);
		}
		
		for (j = 0; j < obj->ctrl_offset; ++j)
		{
			obj->ctrl_list[j].reset(obj->ctrl_list[j].data, obj);
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_update(s_trj_eng *self, vlf_t d_time)
{
	uint32_t i;
	uint32_t j;

	s_trj_obj *obj;
    
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		
		obj->time[0] += d_time;
		obj->time[1] = d_time;
		obj->time[2] = 0.0;
		
		for (j = 0; j < obj->ctrl_offset; ++j)
		{
			obj->ctrl_list[j].update(obj->ctrl_list[j].data, obj);
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_ENG__ */
