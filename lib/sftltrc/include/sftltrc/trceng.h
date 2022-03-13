
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trceng - trace

#ifndef __TRCENG__
#define __TRCENG__

//------------------------------------------------------------------------------

#include <sftlstd/vld3.h>
#include <sftlstd/crc.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcrefs.h>
#include <sftltrc/trctraj.h>
#include <sftltrc/trcctrl.h>
#include <sftltrc/trcdata.h>
#include <sftltrc/trcproc.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trceng
{
	t_f64 				time[2];
	
	t_f64 				time_limit;
	t_f64 				time_step;
	t_u32 				time_iter;
	
	s_trcproc 			*proc;
	
	t_u32 				update_count;
	t_u32 				proc_count;

	s_trcspl 			intf_spl;
	
	t_s32 				proc_sz;
	s_trcproc 			**proc_ls;
	
	t_s32 				refs_sz;
	s_trcrefs			**refs_ls;
	
	t_s32  				obj_sz;
	s_trcobj 			**obj_ls;
	
	t_s32 				traj_sz;
	s_trctraj			**traj_ls;
	
	t_s32 				ctrl_sz;
	s_trcctrl			**ctrl_ls;
	
	t_s32 				data_sz;
	s_trcdata			**data_ls;
	
	t_s32 				proc_intf_sz;
	s_trcproc_intf		**proc_intf_ls;
	
	t_s32 				refs_intf_sz;
	s_trcrefs_intf 		**refs_intf_ls;
	
	t_s32 				obj_intf_sz;
	s_trcobj_intf		**obj_intf_ls;
	
	t_s32 				traj_intf_sz;
	s_trctraj_intf 		**traj_intf_ls;
	
	t_s32 				ctrl_intf_sz;
	s_trcctrl_intf		**ctrl_intf_ls;
	
	t_s32 				data_intf_sz;
	s_trcdata_intf		**data_intf_ls;

}	s_trceng;

typedef struct trceng_attr
{
	t_u8 				*stack;
	
	s_trcproc 			*proc;
	
	s_trcproc  			**proc_ls;
	s_trcrefs  			**refs_ls;
	s_trcobj  			**obj_ls;
	s_trctraj			**traj_ls;
	s_trcctrl			**ctrl_ls;
	s_trcdata			**data_ls;
	
	s_trcproc_intf 		**proc_intf_ls;
	s_trcrefs_intf 		**refs_intf_ls;
	s_trcobj_intf 		**obj_intf_ls;
	s_trctraj_intf 		**traj_intf_ls;
	s_trcctrl_intf 		**ctrl_intf_ls;
	s_trcdata_intf 		**data_intf_ls;
	
	t_f64 				time_limit;
	t_f64 				time_step;
	
}	s_trceng_attr;

//------------------------------------------------------------------------------

t_s8 trceng_init (s_trceng *eng, s_trceng_attr *attr)
{
	eng->time_limit 	= attr->time_limit;
	eng->time_step  	= attr->time_step;
	
	if (eng->time_step < 1E-6) { eng->time_step = 1E-6; }
	
	eng->time_iter 		= eng->time_limit / eng->time_step;
	
	eng->proc 			= attr->proc;
	
	eng->proc_ls 		= attr->proc_ls;
	eng->proc_sz 		= 0x00;
	
	eng->refs_ls 		= attr->refs_ls;
	eng->refs_sz 		= 0x00;
	
	eng->obj_ls 		= attr->obj_ls;
	eng->obj_sz 		= 0x00;
	
	eng->traj_ls 		= attr->traj_ls;
	eng->traj_sz 		= 0x00;
	
	eng->ctrl_ls 		= attr->ctrl_ls;
	eng->ctrl_sz 		= 0x00;
	
	eng->data_ls 		= attr->data_ls;
	eng->data_sz 		= 0x00;
	
	eng->refs_intf_ls 	= attr->refs_intf_ls;
	eng->refs_intf_sz 	= 0x00;
    
    eng->obj_intf_ls 	= attr->obj_intf_ls;
	eng->obj_intf_sz 	= 0x00;
    
    eng->traj_intf_ls 	= attr->traj_intf_ls;
	eng->traj_intf_sz 	= 0x00;
    
    eng->ctrl_intf_ls 	= attr->ctrl_intf_ls;
	eng->ctrl_intf_sz 	= 0x00;
    
    eng->data_intf_ls 	= attr->data_intf_ls;
	eng->data_intf_sz 	= 0x00;
    
    eng->proc_intf_ls	= attr->proc_intf_ls;
	eng->proc_intf_sz 	= 0x00;
	
	eng->update_count 	= 0x00;
	eng->proc_count   	= 0x00;

	return 0x00;
}

//------------------------------------------------------------------------------

inline
t_s32 trceng_obj_index (s_trceng *eng, s_trcobj *obj)
{
	return (t_s32) (((t_sp) eng->obj_ls - (t_sp) obj)
	/ (t_sp) sizeof(s_trcobj));
}

//------------------------------------------------------------------------------

void trceng_print (s_trceng *eng)
{
	printf("obj_sz %d \r\n", eng->obj_sz);
	printf("obj_ls \r\n");
	
	for (t_s32 i = 0; i < eng->obj_sz; ++i)
	{
		printf("%s \r\n", eng->obj_ls[i]->name);
	}
}

//------------------------------------------------------------------------------

t_s8 trceng_add_proc_intf (s_trceng *eng, s_trcproc_intf *proc_intf)
{
	eng->proc_intf_ls[eng->proc_intf_sz] = proc_intf;
	eng->proc_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_add_refs_intf (s_trceng *eng, s_trcrefs_intf *refs_intf)
{
	eng->refs_intf_ls[eng->refs_intf_sz] = refs_intf;
	eng->refs_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_add_obj_intf (s_trceng *eng, s_trcobj_intf *obj_intf)
{
	eng->obj_intf_ls[eng->obj_intf_sz] = obj_intf;
	eng->obj_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_add_traj_intf (s_trceng *eng, s_trctraj_intf *traj_intf)
{
	eng->traj_intf_ls[eng->traj_intf_sz] = traj_intf;
	eng->traj_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_add_ctrl_intf (s_trceng *eng, s_trcctrl_intf *ctrl_intf)
{
	eng->ctrl_intf_ls[eng->ctrl_intf_sz] = ctrl_intf;
	eng->ctrl_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_add_data_intf (s_trceng *eng, s_trcdata_intf *data_intf)
{
	eng->data_intf_ls[eng->data_intf_sz] = data_intf;
	eng->data_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s_trcproc* trceng_add_proc (s_trceng *eng,
							s_trcproc_intf *proc_intf, s_trcproc_attr *attr)
{
	eng->proc_ls[eng->proc_sz] = trcproc_malloc(proc_intf, attr);
	s_trcproc *proc = eng->proc_ls[eng->proc_sz];
	
	eng->proc_sz++;
	
	return proc;
}

//------------------------------------------------------------------------------

s_trcrefs* trceng_add_refs (s_trceng *eng,
							s_trcrefs_intf *refs_intf, s_trcrefs_attr *attr)
{
	eng->refs_ls[eng->refs_sz] = trcrefs_malloc(refs_intf, attr);
	s_trcrefs *refs = eng->refs_ls[eng->refs_sz];
	
	eng->refs_sz++;
	
	return refs;
}

//------------------------------------------------------------------------------

s_trcobj* trceng_add_obj (s_trceng *eng,
						  s_trcobj_intf *obj_intf, s_trcobj_attr *attr)
{
	eng->obj_ls[eng->obj_sz] = trcobj_malloc(obj_intf, attr);
	s_trcobj *obj = eng->obj_ls[eng->obj_sz];
	
	eng->obj_sz++;
	
	return obj;
}

//------------------------------------------------------------------------------

s_trctraj* trceng_add_traj (s_trceng *eng,
							s_trctraj_intf *traj_intf, s_trctraj_attr *attr)
{
	eng->traj_ls[eng->traj_sz] = trctraj_malloc(traj_intf, attr);
	s_trctraj *traj = eng->traj_ls[eng->traj_sz];
	
	eng->traj_sz++;
	
	return traj;
}

//------------------------------------------------------------------------------

s_trcctrl* trceng_add_ctrl (s_trceng *eng,
							s_trcctrl_intf *ctrl_intf, s_trcctrl_attr *attr)
{
	eng->ctrl_ls[eng->ctrl_sz] = trcctrl_malloc(ctrl_intf, attr);
	s_trcctrl *ctrl = eng->ctrl_ls[eng->ctrl_sz];
	
	eng->ctrl_sz++;
	
	return ctrl;
}

//------------------------------------------------------------------------------

s_trcdata* trceng_add_data (s_trceng *eng,
							s_trcdata_intf *data_intf, s_trcdata_attr *attr)
{
	eng->data_ls[eng->refs_sz] = trcdata_malloc(data_intf, attr);
	s_trcdata *data = eng->data_ls[eng->data_sz];
	
	eng->data_sz++;
	
	return data;
}

//------------------------------------------------------------------------------

t_s8 trceng_save (s_trceng *eng, char *file_name)
{
	t_u8 *__v_file__ = (t_u8*) malloc(256*1024*1024);
	t_u8 *v_file = __v_file__;
	
	s_trceng *v_self = (s_trceng*) v_file;
	v_file += sizeof(s_trceng);
	
	*v_self = *eng;
	
	for (t_s32 i = 0; i < eng->proc_sz; ++i)
	{
		trcproc_save(eng->proc_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (t_s32 i = 0; i < eng->refs_sz; ++i)
	{
		trcrefs_save(eng->refs_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (t_s32 i = 0; i < eng->obj_sz; ++i)
	{
		trcobj_save(eng->obj_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (t_s32 i = 0; i < eng->traj_sz; ++i)
	{
		trctraj_save(eng->traj_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (t_s32 i = 0; i < eng->ctrl_sz; ++i)
	{
		trcctrl_save(eng->ctrl_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (t_s32 i = 0; i < eng->data_sz; ++i)
	{
		trcdata_save(eng->data_ls[i], &eng->intf_spl, &v_file);
	}
	
	// must be after we did all the allocation on v_file
	// do not move up
	t_u32 file_size = v_file - __v_file__;
	
	FILE *file_handle = fopen(file_name, "wb+");
	fwrite(&file_size, 1, sizeof(t_u32), file_handle);
	fwrite(__v_file__, 1, v_file - __v_file__, file_handle);
	fclose(file_handle);
	
	free(__v_file__);
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_load (s_trceng *eng, char *file_name)
{
//	FILE *file_handle = fopen(file_name, "rb");
//
//	if (file_handle != NULL)
//	{
//		eng->proc.free(&self->proc.data);
//
//		for (t_s32 i = 0; i < self->obj_sz; ++i)
//		{
//			if (self->obj_ls[i].log_ls != NULL)
//			{
//				free(self->obj_ls[i].log_ls);
//				eng->obj_ls[i].log_ls = NULL;
//			}
//
//			for (t_s32 j = 0; j < self->obj_ls[i].traj_sz; ++j)
//			{
//				eng->obj_ls[i].traj_ls[j].free(&self->obj_ls[i].traj_ls[j].data);
//			}
//
//			for (t_s32 j = 0; j < self->obj_ls[i].ctrl_sz; ++j)
//			{
//				eng->obj_ls[i].ctrl_ls[j].free(&self->obj_ls[i].ctrl_ls[j].data);
//			}
//
//			for (t_s32 j = 0; j < self->obj_ls[i].data_sz; ++j)
//			{
//				eng->obj_ls[i].data_ls[j].free(&self->obj_ls[i].data_ls[j].data);
//			}
//		}
//
//		t_u8 *__v_file__ = (t_u8*) malloc(256*1024*1024);
//		t_u8 *v_file = __v_file__;
//		t_u32 file_size;
//
//		fread(&file_size, 1, sizeof(t_u32), file_handle);
//		fread(__v_file__, 1, file_size, file_handle);
//		fclose(file_handle);
//
//		s_trceng *v_self = (s_trceng *) v_file;
//		v_file += sizeof(s_trceng);
//
//		self->obj_sz = v_self->obj_sz;
//		self->time_limit = v_self->time_limit;
//		self->time_step = v_self->time_step;
//		if (self->time_step < 1E-6) { self->time_step = 1E-6; }
//		self->time_iter = self->time_limit / self->time_step;
//
//		trcproc_load(&self->proc, self, &v_file);
//
//		for (t_s32 i = 0; i < self->obj_sz; ++i)
//		{
//			s_trcobj *obj = &self->obj_ls[i];
//
//			trcobj_load(obj, self, &v_file);
//
//			obj->time = self->time;
//			obj->log_ls = NULL;
//			obj->log_sz = 0x00;
//			obj->hash = crc32_iso_str(obj->desc);
//		}
//
//		free(__v_file__);
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_reset (s_trceng *eng)
{
//	self->time[0] = 0.0;
//	self->time[1] = 0.0;
//
//	self->update_count = 0x01;
//	self->proc_count   = 0x00;
//
//	for (t_s32 i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &self->obj_ls[i];
//
//		// must retain order
//		// first reset offset then NULL the ptr
//		// some RT plotting functions may still try to draw this
//		obj->log_sz = 0x00;
//
//		// remove all forces coz they are additive
//		// thus all the plugins will add and never set
//		// so we need to zero it manually
//		vld3v_set(obj->pos_force, 0.0);
//		vld3v_set(obj->rot_force, 0.0);
//
//		if (obj->log_ls != NULL)
//		{
//			free(obj->log_ls);
//			obj->log_ls = NULL;
//		}
//
//		for (t_s32 j = 0; j < obj->traj_sz; ++j)
//		{
//			obj->traj_ls[j].compile(obj->traj_ls[j].data);
//		}
//
//		for (t_s32 j = 0; j < obj->ctrl_sz; ++j)
//		{
//			obj->ctrl_ls[j].reset(obj->ctrl_ls[j].data, obj);
//		}
//
//		for (t_s32 j = 0; j < obj->data_sz; ++j)
//		{
//			obj->data_ls[j].reset(obj->data_ls[j].data, obj);
//		}
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_update(s_trceng *self, t_f64 d_time)
{
//	self->time[0] += d_time;
//	self->time[1]  = d_time;
//
//	for (t_s32 i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &self->obj_ls[i];
//
//		// remove all forces coz they are additive
//		// thus all the plugins will add and never set
//		// so we need to zero it manually
//		vld3v_set(obj->pos_force, 0.0);
//		vld3v_set(obj->rot_force, 0.0);
//
//		for (t_s32 j = 0; j < obj->ctrl_sz; ++j)
//		{
//			obj->ctrl_ls[j].update(obj->ctrl_ls[j].data, obj);
//		}
//	}
//
////	for (t_s32 i = 0; i < self->obj_sz; ++i)
////	{
////	    vld3v_subv(&obj->pos[1][0], &obj->pos[0][0], &obj->log_ls[obj->log_sz-1] = &self->obj_ls[i];
////
////	    // remove all forces coz they are additive
////	    // thus all the plugins will add and never set
////	    // so we need to zero it manually
////	    vld3v_set(obj->pos_force, 0.0);
////	    vld3v_set(obj->rot_force, 0.0);
////
////	    for (t_s32 j = 0; j < obj->ctrl_sz; ++j)
////	    {
////	        obj->ctrl_ls[j].update(obj->ctrl_ls[j].data, obj);
////	    }
////	}

	
	self->update_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_proc (s_trceng *eng)
{
//	for (t_s32 i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &eng->obj_ls[i];
//		eng->proc.update(eng->proc.data, obj, self->proc_count);
//	}
//
	eng->proc_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_render (s_trceng *eng)
{
//	for (t_s32 i = 0; i < eng->obj_sz; ++i)
//	{
//		s_trcobj *obj = &eng->obj_ls[i];
//
//		for (t_s32 j = 0; j < obj->data_sz; ++j)
//		{
//			obj->data_ls[j].render(obj->data_ls[j].data, obj);
//		}
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

t_s8 trceng_log (s_trceng *eng)
{
//	for (t_s32 i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &eng->obj_ls[i];
//		s_trcobj_data *data = &obj->log_ls[obj->log_sz];
//
//		data->time[0] = eng->time[0];
//		data->time[1] = eng->time[1];
//
//		vld3m_copy(&data->rot[0][0], &obj->rot[0][0]);
//		vld3m_copy(&data->rot[1][0], &obj->rot[1][0]);
//		vld3m_copy(&data->rot[2][0], &obj->rot[2][0]);
//
//		vld3m_copy(&data->pos[0][0], &obj->pos[0][0]);
//		vld3m_copy(&data->pos[1][0], &obj->pos[1][0]);
//		vld3m_copy(&data->pos[2][0], &obj->pos[2][0]);
//
//		vld3v_copy(data->rot_force, obj->rot_force);
//		vld3v_copy(data->pos_force, obj->pos_force);
//
//		obj->log_sz++;
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCENG__ */
