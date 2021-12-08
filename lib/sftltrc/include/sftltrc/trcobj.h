
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
	
	s32_t				data_sz;
	s32_t 				attr_sz;
	
	s8_t (*init) 		(void *obj, void *attr);
	s8_t (*free) 		(void *obj);
	s8_t (*save) 		(void *obj, s_trcspl *spl, u8_t **v_file);
	s8_t (*load) 		(void *obj, s_trcspl *spl, u8_t **v_file);
	
	void (*gui_attr)	(void *obj, void *attr);
	void (*gui_edit)	(void *obj);
	void (*gui_view)	(void *obj);
	
}	s_trcobj_intf;

//------------------------------------------------------------------------------

typedef struct trcobj_data
{
	f64_t 			time[2];
	
	f64_t 			pos[3][3];
	f64_t 			rot[3][9];
	
	f64_t 			pos_force[3];
	f64_t 			rot_force[3];
	
	f64_t 			pos_error;
	f64_t 			rot_error;
	
}	s_trcobj_data;

typedef struct trcobj
{
	s_trcobj_intf  *intf;
	
	char 			name[32];
	
	f64_t 			*time;
	
	f64_t 			pos[3][3];
	f64_t 			rot[3][9];
	
	f64_t 			pos_force[3];
	f64_t 			rot_force[3];
	
	f64_t 			pos_inert;
	f64_t 			rot_inert;
	
	u32_t 			log_sz;
	s_trcobj_data 	*log_ls;
	
} 	s_trcobj;

typedef struct trcobj_attr
{
	char 			name[32];
	
} s_trcobj_attr;

//------------------------------------------------------------------------------

inline
s8_t trcobj_init (s_trcobj *obj, s_trcobj_attr *attr)
{
	strcpy(obj->name, attr->name);
	
	obj->log_ls = NULL;
	obj->log_sz = 0x00;
	
	return obj->intf->init(obj, attr);
}

//------------------------------------------------------------------------------

inline
s8_t trcobj_free (s_trcobj *obj)
{
	return obj->intf->free(obj);
}

//------------------------------------------------------------------------------

inline
s8_t trcobj_save (s_trcobj *obj, s_trcspl *spl, u8_t **v_file)
{
	s_trcobj *v_self = (s_trcobj*) *v_file;
	*v_file += sizeof(s_trcobj);
	
	*v_self = *obj;
	
	v_self->time = NULL;
	
	return obj->intf->save(obj, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcobj_load (s_trcobj *obj, s_trcspl *spl, u8_t **v_file)
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
//	for (s32_t i = 0; i < obj->traj_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->traj_ls[i].desc);
//	}
//
//	printf(vl_lsep);
//	printf("ctrl_ls \r\n");
//
//	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->ctrl_ls[i].desc);
//	}
//}

//------------------------------------------------------------------------------
//
//void trcobj_ctrl_update(s_trcobj *obj, f64_t d_time)
//{
//	obj->time[2] = d_time * d_time * 0.5;
//	obj->time[1] = d_time;
//	obj->time[0] = obj->time[0] + d_time;
//
//	u32_t i;
//
//	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
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
//	s32_t offset;
//
//	api->free(&api->data);
//
//	if (obj->traj_sz == 0x00) { return 0x00; }
//
//	for (s32_t i = 0; i < obj->traj_sz-1; ++i)
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
//s8_t trcobj_ctrl_add (s_trcobj *obj, s_trcctrl *ctrl)
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
//s8_t trcobj_ctrl_del (s_trcobj *obj, s_trcctrl *ctrl)
//{
//	s32_t offset = 0x00;
//
//	api->free(&api->data);
//
//	if (self->ctrl_sz == 0x00) { return 0x00; }
//
//	for (s32_t i = 0; i < self->ctrl_sz-1; ++i)
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

//s8_t trcobj_data_add (s_trcobj *self, s_trcdata data_api)
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
//s8_t trcobj_data_del (s_trcobj *self, s_trcdata *api)
//{
//	s32_t i;
//	s32_t offset = 0x00;
//
//	api->free(&api->data);
//
//	if (self->data_sz == 0x00) { return 0x00; }
//
//	for (s32_t i = 0; i < self->data_sz-1; ++i)
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

//void trcobj_save (s_trcobj *obj, s_trceng *eng, u8_t **v_file)
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
//	for (s32_t i = 0; i < obj->traj_sz; ++i)
//	{
//		trctraj_save (&obj->traj_ls[i], eng, v_file);
//	}
//
//	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
//	{
//		trcctrl_save (&obj->ctrl_ls[i], eng, v_file);
//	}
//
//	for (s32_t i = 0; i < obj->data_sz; ++i)
//	{
//		trcdata_save (&obj->data_ls[i], eng, v_file);
//	}
//}
//
//void trcobj_load (s_trcobj *obj, s_trceng *eng, u8_t **v_file)
//{
//	s_trcobj *v_self = (s_trcobj*) *v_file;
//	*v_file += sizeof(s_trcobj);
//
//	*obj = *v_self;
//
//	// support custom widget save/load
//	for (s32_t i = 0; i < obj->traj_sz; ++i)
//	{
//		trctraj_load(&obj->traj_ls[i], eng, v_file);
//	}
//
//	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
//	{
//		trcctrl_load(&obj->ctrl_ls[i], eng, v_file);
//	}
//
//	for (s32_t i = 0; i < obj->data_sz; ++i)
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
//	for (s32_t i = 0; i < src->traj_sz; ++i)
//	{
//		trctraj_copy(eng, &dest->traj_ls[i], &src->traj_ls[i]);
//	}
//
//	for (s32_t i = 0; i < src->ctrl_sz; ++i)
//	{
//		trcctrl_copy(eng, &dest->ctrl_ls[i], &src->ctrl_ls[i]);
//	}
//
//	for (s32_t i = 0; i < src->data_sz; ++i)
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
//	for (s32_t i = 0; i < obj->traj_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->traj_ls[i].desc);
//	}
//
//	printf(vl_lsep);
//	printf("ctrl_ls \r\n");
//
//	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
//	{
//		printf("- [%02d] %s \r\n", i, obj->ctrl_ls[i].desc);
//	}
//}

//------------------------------------------------------------------------------
//
//void trcobj_ctrl_update(s_trcobj *obj, f64_t d_time)
//{
//	obj->time[2] = d_time * d_time * 0.5;
//	obj->time[1] = d_time;
//	obj->time[0] = obj->time[0] + d_time;
//
//	u32_t i;
//
//	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
//	{
//		s_trcctrl_api *header = (s_trcctrl_api*) obj->ctrl_data[i];
//		header->update(obj->ctrl_data);
//	}
//
//    return;
//}


//------------------------------------------------------------------------------

#endif /* __TRCOBJ__ */



























