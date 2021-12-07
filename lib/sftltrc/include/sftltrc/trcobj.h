
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcobj - trace

#ifndef __TRCOBJ__
#define __TRCOBJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------

typedef struct trcobj_intf
{
	char 				guid[32];
	
	s32_t				data_sz;
	s32_t 				attr_sz;
	
	s8_t (*init) 		(void *obj, void *attr);
	s8_t (*free) 		(void *obj);
	s8_t (*pack) 		(void *obj, s_trcspl *spl);
	s8_t (*unpack) 		(void *obj, s_trcspl *spl);
	s8_t (*save) 		(void *obj, u8_t **v_file);
	s8_t (*load) 		(void *obj, u8_t **v_file);
	
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
s8_t trcobj_pack (s_trcobj *obj, s_trcspl *spl)
{
	return obj->intf->pack(obj, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcobj_unpack (s_trcobj *obj, s_trcspl *spl)
{
	return obj->intf->unpack(obj, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcobj_save (s_trcobj *obj, u8_t **v_file)
{
	s_trcobj *v_self = (s_trcobj*) *v_file;
	*v_file += sizeof(s_trcobj);
	
	*v_self = *obj;
	
	v_self->time = NULL;
	
	return obj->intf->save(obj, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcobj_load (s_trcobj *obj, u8_t **v_file)
{
	s_trcobj *v_self = (s_trcobj*) *v_file;
	*v_file += sizeof(s_trcobj);
	
	*obj = *v_self;
	
	return obj->intf->load(obj, v_file);
}

//------------------------------------------------------------------------------

static
s8_t __trcobj_init__ (void *__obj__, void *__attr__)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
s8_t __trcobj_free__ (void *__obj__)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
s8_t __trcobj_save__ (void *__obj__, u8_t **v_file)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
s8_t __trcobj_load__ (void *__obj__, u8_t **v_file)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
s8_t __trcobj_pack__ (void *__obj__, s_trcspl *spl)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
s8_t __trcobj_unpack__ (void *__obj__, s_trcspl *spl)
{
	return 0x00;
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

s_trcobj_intf __trcobj__ = {
	.guid   = "trcobj",

	.data_sz = sizeof(s_trcobj),
	.attr_sz = sizeof(s_trcobj_attr),
	
	.init   = __trcobj_init__,
	.free   = __trcobj_free__,
	.pack   = __trcobj_pack__,
	.unpack = __trcobj_unpack__,
	.save   = __trcobj_save__,
	.load   = __trcobj_load__,
};

//------------------------------------------------------------------------------

s8_t trcobj_init (s_trcobj **obj, s_trcobj_attr *attr)
{
	*obj = (s_trcobj*) malloc(sizeof(s_trcobj));
	
	(*obj)->intf = &__trcobj__;
	
	trcobj_init(*obj, attr);
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCOBJ__ */



























