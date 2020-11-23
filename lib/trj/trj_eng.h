
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_ENG__
#define __TRJ_ENG__

//------------------------------------------------------------------------------

#include <string.h>

#include "vl.h"
#include "trj_api.h"
#include "trj_obj.h"

//------------------------------------------------------------------------------

typedef struct trj_eng_init_attr
{
	s_trj_proc *proc;
	
	s_trj_obj  *obj_list;
	
	s_trj_ellp *ellp_list;
	s_trj_traj *traj_list;
	s_trj_ctrl *ctrl_list;
	s_trj_data *data_list;
	s_trj_proc *proc_list;
	
}	s_trj_eng_init;

//------------------------------------------------------------------------------

inline void trj_eng_print(s_trj_eng *eng)
{
	printf("obj_count %d \r\n", eng->obj_count);
	
	printf("obj_list \r\n");
	
	uint32_t i;
	
	for (i = 0; i < eng->obj_count; ++i)
	{ printf("%s \r\n", eng->obj_list[i].desc); }
	
	return;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_init(s_trj_eng *self, s_trj_eng_init attr)
{
	self->ellp_list = attr.ellp_list;
	self->ellp_offset = 0x00;
	
	self->obj_list = attr.obj_list;
	self->obj_count = 0x00;
	
	self->traj_list = attr.traj_list;
	self->traj_offset = 0x00;
	
	self->ctrl_list = attr.ctrl_list;
	self->ctrl_offset = 0x00;
	
	self->data_list = attr.data_list;
	self->data_offset = 0x00;
	
	self->proc_list = attr.proc_list;
	self->proc_offset = 0x00;
	
	self->proc = attr.proc;
	
	self->update_count = 0x00;
	self->proc_count   = 0x00;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_add_ellpapi(s_trj_eng *self, s_trj_ellp api)
{
	self->ellp_list[self->ellp_offset] = api;
	++self->ellp_offset;
	
	return 0x00;
}

inline uint8_t trj_eng_add_trajapi(s_trj_eng *self, s_trj_traj api)
{
	self->traj_list[self->traj_offset] = api;
	++self->traj_offset;
	
	return 0x00;
}

inline uint8_t trj_eng_add_ctrlapi(s_trj_eng *self, s_trj_ctrl api)
{
	self->ctrl_list[self->ctrl_offset] = api;
	++self->ctrl_offset;
	
	return 0x00;
}

inline uint8_t trj_eng_add_dataapi(s_trj_eng *self, s_trj_data api)
{
	self->data_list[self->data_offset] = api;
	++self->data_offset;
	
	return 0x00;
}

inline uint8_t trj_eng_add_procapi(s_trj_eng *self, s_trj_proc api)
{
	self->proc_list[self->proc_offset] = api;
	s_trj_proc *proc = &self->proc_list[self->proc_offset];
	++self->proc_offset;
	
	// !!!  IMPORTANT proc is selected by reference in eng api
	// so it is basically initialized once
	proc->init(&proc->data, proc->config);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_add(s_trj_eng *self, s_trj_obj_init attr)
{
	s_trj_obj *obj = &self->obj_list[self->obj_count];
	
	trj_obj_init(obj, attr);
	
	obj->id 		= self->obj_count;
	obj->time 		= self->time;
	obj->log_list   = NULL;
	obj->log_offset = 0x00;
	
	self->obj_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_reset(s_trj_eng *self)
{
	uint32_t i;
	uint32_t j;
	
	s_trj_obj *obj;
	
	self->time[0] = 0.0;
	self->time[1] = 0.0;
	
	self->update_count = 0x01;
	self->proc_count   = 0x00;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		
		// must retain order
		// first reset offset then NULL the ptr
		// some RT plotting functions may still try to draw this
		obj->log_offset = 0x00;
		free(obj->log_list);
		
		for (j = 0; j < obj->traj_offset; ++j)
		{
			obj->traj_list[j].compile(obj->traj_list[j].data);
		}
		
		for (j = 0; j < obj->ctrl_offset; ++j)
		{
			obj->ctrl_list[j].reset(obj->ctrl_list[j].data, obj);
		}
		
		for (j = 0; j < obj->data_offset; ++j)
		{
			obj->data_list[j].reset(obj->data_list[j].data, obj);
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
	
	self->time[0] += d_time;
	self->time[1] = d_time;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		
		for (j = 0; j < obj->ctrl_offset; ++j)
		{
			obj->ctrl_list[j].update(obj->ctrl_list[j].data, obj);
		}
	}
	
	++self->update_count;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_proc(s_trj_eng *self)
{
	uint32_t i;
	
	s_trj_obj *obj;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		self->proc->update(self->proc->data, obj, self->proc_count);
	}
	
	++self->proc_count;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_render(s_trj_eng *self)
{
	uint32_t i;
	uint32_t j;
	
	s_trj_obj *obj;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		
		for (j = 0; j < obj->data_offset; ++j)
		{
			obj->data_list[j].render(obj->data_list[j].data, obj);
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_log(s_trj_eng *self)
{
	uint32_t i;
	
	s_trj_obj *obj;
	s_trj_obj_data *data;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		obj = &self->obj_list[i];
		data = &obj->log_list[obj->log_offset];
		
		data->time[0] = self->time[0];
		data->time[1] = self->time[1];
		
		vl_mcopy(&data->rot[0][0], &obj->rot[0][0]);
		vl_mcopy(&data->rot[1][0], &obj->rot[1][0]);
		vl_mcopy(&data->rot[2][0], &obj->rot[2][0]);

		vl_mcopy(&data->pos[0][0], &obj->pos[0][0]);
		vl_mcopy(&data->pos[1][0], &obj->pos[1][0]);
		vl_mcopy(&data->pos[2][0], &obj->pos[2][0]);
		
		vl_vcopy(data->rot_force, obj->rot_force);
		vl_vcopy(data->pos_force, obj->pos_force);
		
		++obj->log_offset;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_ENG__ */
