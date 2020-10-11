
#include "trj_obj.h"

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

uint8_t trj_obj_init(s_trj_obj *self, s_trj_obj_init attr)
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

uint8_t trj_obj_traj_add(s_trj_obj *self, void *traj_data)
{
	self->traj_data[self->traj_offset] = traj_data;
	++self->traj_offset;
	
	return 0x00;
}

//------------------------------------------------------------------------------



