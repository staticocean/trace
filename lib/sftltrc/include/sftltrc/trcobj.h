
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcobj - trace

#ifndef __TRCOBJ__
#define __TRCOBJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------

typedef struct trcobj_intf
{
	char 				desc[32];
	
	t_s32				data_sz;
	t_s32 				attr_sz;
	
	t_s8 (*init) 		(void *obj, void *attr);
	t_s8 (*free) 		(void *obj);
	t_s8 (*save) 		(void *obj, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*load) 		(void *obj, s_trcspl *spl, t_u8 **v_file);
	
	void (*gui_attr)	(void *obj, void *attr);
	void (*gui_edit)	(void *obj);
	void (*gui_view)	(void *obj);
	
}	s_trcobj_intf;

//------------------------------------------------------------------------------

typedef struct trcobj_data
{
	t_f64 			    time[2];
	
	t_f64 			    pos[3][3];
	t_f64 			    rot[3][9];
	
	t_f64 			    pos_force[3];
	t_f64 			    rot_force[3];
	
	t_f64 			    pos_error;
	t_f64 			    rot_error;
	
}	s_trcobj_data;

typedef struct trcobj
{
	s_trcobj_intf      *intf;

    t_u64               guid;

    char 			    name[32];
	
	t_f64 			   *time;
	
	t_f64 			    pos[3][3];
	t_f64 			    rot[3][9];
	
	t_f64 			    pos_force[3];
	t_f64 			    rot_force[3];
	
	t_f64 			    pos_inert;
	t_f64 			    rot_inert;
	
	t_u32 			    log_sz;
	s_trcobj_data 	   *log_ls;
	
} 	s_trcobj;

typedef struct trcobj_attr
{
    t_u64               guid;

    char 			    name[32];
	
} s_trcobj_attr;

//------------------------------------------------------------------------------

inline
t_s8 trcobj_init (s_trcobj *obj, s_trcobj_attr *attr)
{
    obj->guid = attr->guid;

	strcpy(obj->name, attr->name);
	
	obj->log_ls = NULL;
	obj->log_sz = 0x00;
	
	return obj->intf->init(obj, attr);
}

//------------------------------------------------------------------------------

inline
t_s8 trcobj_free (s_trcobj *obj)
{
	return obj->intf->free(obj);
}

//------------------------------------------------------------------------------

inline
t_s8 trcobj_save (s_trcobj *obj, s_trcspl *spl, t_u8 **v_file)
{
	s_trcobj *v_self = (s_trcobj*) *v_file;
	*v_file += sizeof(s_trcobj);
	
	*v_self = *obj;
	
	v_self->time = NULL;
	
	return obj->intf->save(obj, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcobj_load (s_trcobj *obj, s_trcspl *spl, t_u8 **v_file)
{
	s_trcobj *v_self = (s_trcobj*) *v_file;
	*v_file += sizeof(s_trcobj);
	
	*obj = *v_self;
	
	return obj->intf->load(obj, spl, v_file);
}

//------------------------------------------------------------------------------

inline
void trcobj_gui_attr (s_trcobj *obj, s_trcobj_attr *attr)
{
	obj->intf->gui_attr(obj, attr);
}

//------------------------------------------------------------------------------

inline
void trcobj_gui_edit (s_trcobj *obj)
{
	obj->intf->gui_edit(obj);
}

//------------------------------------------------------------------------------

inline
void trcobj_gui_view (s_trcobj *obj)
{
	obj->intf->gui_view(obj);
}

//------------------------------------------------------------------------------

inline
s_trcobj* trcobj_malloc (s_trcobj_intf *intf_obj, s_trcobj_attr *attr)
{
	s_trcobj *obj = (s_trcobj*) malloc(intf_obj->data_sz);
	
	trcobj_init(obj, attr);
	
	return obj;
}

//------------------------------------------------------------------------------
//
//void trcobj_print (s_trcobj *obj)
//{
//	printf(vl_lsep);
//	printf("desc        [%s] \r\n", obj->desc);
//	printf("hash        [%08X] \r\n", obj->hash);
////	printf("ref         [%s] \r\n", obj->ref->name);
//	printf("traj_sz [%d] \r\n", obj->traj_sz);
//	printf("ctrl_sz [%d] \r\n", obj->ctrl_sz);
//	printf("data_sz [%d] \r\n", obj->data_sz);
//
//	printf(vl_lsep);
//	printf("traj_ls \r\n");
//
//	for (t_s32 i = 0; i < obj->traj_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->traj_ls[i].desc);
//	}
//
//	printf(vl_lsep);
//	printf("ctrl_ls \r\n");
//
//	for (t_s32 i = 0; i < obj->ctrl_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->ctrl_ls[i].desc);
//	}
//}

//------------------------------------------------------------------------------
//
//void trcobj_ctrl_update(s_trcobj *obj, t_f64 d_time)
//{
//	obj->time[2] = d_time * d_time * 0.5;
//	obj->time[1] = d_time;
//	obj->time[0] = obj->time[0] + d_time;
//
//	t_u32 i;
//
//	for (t_s32 i = 0; i < obj->ctrl_sz; ++i)
//	{
//		s_trcctrl_api *header = (s_trcctrl_api*) obj->ctrl_data[i];
//		header->update(obj->ctrl_data);
//	}
//
//    return;
//}

//------------------------------------------------------------------------------

//void trcobj_addtraj (s_trcobj *obj, void *traj)
//{
//	s_trctraj *traj = &self->traj_ls[self->traj_sz];
//	self->traj_sz += 0x01;
//
//	*api = traj_api;
//	api->init(&api->data, api->config);
//}
//
////------------------------------------------------------------------------------
//
//void trcobj_traj_del (s_trcobj *obj, s_trctraj *traj)
//{
//	t_s32 offset;
//
//	api->free(&api->data);
//
//	if (obj->traj_sz == 0x00) { return 0x00; }
//
//	for (t_s32 i = 0; i < obj->traj_sz-1; ++i)
//	{
//		if (&obj->traj_ls[i] == api)
//		{ offset = 0x01; }
//
//		obj->traj_ls[i] = obj->traj_ls[i+offset];
//	}
//
//	obj->traj_sz -= 0x01;
//}
//
////------------------------------------------------------------------------------
//
//t_s8 trcobj_ctrl_add (s_trcobj *obj, s_trcctrl *ctrl)
//{
//	ctrl->hash = crc32_iso_str(ctrl->desc);
//
//	s_trcctrl *api = &self->ctrl_ls[obj->ctrl_sz];
//	obj->ctrl_sz++;
//
//	*api = *ctrl;
//	api->init(&api->data, api->config);
//
//	return 0x00;
//}
//
////------------------------------------------------------------------------------
//
//t_s8 trcobj_ctrl_del (s_trcobj *obj, s_trcctrl *ctrl)
//{
//	t_s32 offset = 0x00;
//
//	api->free(&api->data);
//
//	if (self->ctrl_sz == 0x00) { return 0x00; }
//
//	for (t_s32 i = 0; i < self->ctrl_sz-1; ++i)
//	{
//		if (&self->ctrl_ls[i] == api)
//		{ offset = 0x01; }
//
//		self->ctrl_ls[i] = self->ctrl_ls[i+offset];
//	}
//
//	self->ctrl_sz -= 0x01;
//
//	return 0x00;
//}

//------------------------------------------------------------------------------

//t_s8 trcobj_data_add (s_trcobj *self, s_trcdata data_api)
//{
//	data_api.hash = crc32_iso_str(data_api.desc);
//
//	s_trcdata *api = &self->data_ls[self->data_sz];
//	self->data_sz += 0x01;
//
//	*api = data_api;
//	api->init(&api->data, api->config);
//
//	return 0x00;
//}
//
////------------------------------------------------------------------------------
//
//t_s8 trcobj_data_del (s_trcobj *self, s_trcdata *api)
//{
//	t_s32 i;
//	t_s32 offset = 0x00;
//
//	api->free(&api->data);
//
//	if (self->data_sz == 0x00) { return 0x00; }
//
//	for (t_s32 i = 0; i < self->data_sz-1; ++i)
//	{
//		if (&self->data_ls[i] == api)
//		{ offset = 0x01; }
//
//		self->data_ls[i] = self->data_ls[i+offset];
//	}
//
//	self->data_sz -= 0x01;
//
//	return 0x00;
//}

//------------------------------------------------------------------------------

//void trcobj_save (s_trcobj *obj, s_trceng *eng, t_u8 **v_file)
//{
//	s_trcobj *v_self = (s_trcobj*) *v_file;
//	*v_file += sizeof(s_trcobj);
//
//	*v_self = *obj;
//
//	v_self->time       = NULL;
//	v_self->log_ls   = NULL;
//	v_self->log_sz = 0x00;
//
//	// support custom widget save/load
//
//	for (t_s32 i = 0; i < obj->traj_sz; ++i)
//	{
//		trctraj_save (&obj->traj_ls[i], eng, v_file);
//	}
//
//	for (t_s32 i = 0; i < obj->ctrl_sz; ++i)
//	{
//		trcctrl_save (&obj->ctrl_ls[i], eng, v_file);
//	}
//
//	for (t_s32 i = 0; i < obj->data_sz; ++i)
//	{
//		trcdata_save (&obj->data_ls[i], eng, v_file);
//	}
//}
//
//void trcobj_load (s_trcobj *obj, s_trceng *eng, t_u8 **v_file)
//{
//	s_trcobj *v_self = (s_trcobj*) *v_file;
//	*v_file += sizeof(s_trcobj);
//
//	*obj = *v_self;
//
//	// support custom widget save/load
//	for (t_s32 i = 0; i < obj->traj_sz; ++i)
//	{
//		trctraj_load(&obj->traj_ls[i], eng, v_file);
//	}
//
//	for (t_s32 i = 0; i < obj->ctrl_sz; ++i)
//	{
//		trcctrl_load(&obj->ctrl_ls[i], eng, v_file);
//	}
//
//	for (t_s32 i = 0; i < obj->data_sz; ++i)
//	{
//		trcdata_load(&obj->data_ls[i], eng, v_file);
//	}
//}

//void trcobj_copy (s_trceng *eng, s_trcobj *dest, s_trcobj *src)
//{
//	// Prevent GUI glitches in case of task switch
//	dest->traj_sz = 0x00;
//	dest->ctrl_sz = 0x00;
//	dest->data_sz = 0x00;
//
//	for (t_s32 i = 0; i < src->traj_sz; ++i)
//	{
//		trctraj_copy(eng, &dest->traj_ls[i], &src->traj_ls[i]);
//	}
//
//	for (t_s32 i = 0; i < src->ctrl_sz; ++i)
//	{
//		trcctrl_copy(eng, &dest->ctrl_ls[i], &src->ctrl_ls[i]);
//	}
//
//	for (t_s32 i = 0; i < src->data_sz; ++i)
//	{
//		trcdata_copy(eng, &dest->data_ls[i], &src->data_ls[i]);
//	}
//
//	dest->traj_sz = src->traj_sz;
//	dest->ctrl_sz = src->ctrl_sz;
//	dest->data_sz = src->data_sz;
//}

//------------------------------------------------------------------------------

//void trcobj_print(s_trcobj *obj)
//{
//	printf(vl_lsep);
//	printf("desc        [%s] \r\n", obj->desc);
//	printf("hash        [%08X] \r\n", obj->hash);
//	//	printf("ref         [%s] \r\n", obj->ref->name);
//	printf("traj_sz [%d] \r\n", obj->traj_sz);
//	printf("ctrl_sz [%d] \r\n", obj->ctrl_sz);
//	printf("data_sz [%d] \r\n", obj->data_sz);
//
//	printf(vl_lsep);
//	printf("traj_ls \r\n");
//
//	for (t_s32 i = 0; i < obj->traj_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->traj_ls[i].desc);
//	}
//
//	printf(vl_lsep);
//	printf("ctrl_ls \r\n");
//
//	for (t_s32 i = 0; i < obj->ctrl_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->ctrl_ls[i].desc);
//	}
//}

//------------------------------------------------------------------------------
//
//void trcobj_ctrl_update(s_trcobj *obj, t_f64 d_time)
//{
//	obj->time[2] = d_time * d_time * 0.5;
//	obj->time[1] = d_time;
//	obj->time[0] = obj->time[0] + d_time;
//
//	t_u32 i;
//
//	for (t_s32 i = 0; i < obj->ctrl_sz; ++i)
//	{
//		s_trcctrl_api *header = (s_trcctrl_api*) obj->ctrl_data[i];
//		header->update(obj->ctrl_data);
//	}
//
//    return;
//}


//------------------------------------------------------------------------------

#endif /* __TRCOBJ__ */



























