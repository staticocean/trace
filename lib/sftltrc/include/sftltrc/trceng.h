
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trceng - trace

#ifndef __TRCENG__
#define __TRCENG__

//------------------------------------------------------------------------------

#include <sftlstd/vl.h>
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
	f64_t 				time[2];
	
	f64_t 				time_limit;
	f64_t 				time_step;
	u32_t 				time_iter;
	
	s_trcproc 			*proc;
	
	u32_t 				update_count;
	u32_t 				proc_count;

	s_trcspl 			intf_spl;
	
	s32_t 				proc_sz;
	s_trcproc 			**proc_ls;
	
	s32_t 				refs_sz;
	s_trcrefs			**refs_ls;
	
	s32_t  				obj_sz;
	s_trcobj 			**obj_ls;
	
	s32_t 				traj_sz;
	s_trctraj			**traj_ls;
	
	s32_t 				ctrl_sz;
	s_trcctrl			**ctrl_ls;
	
	s32_t 				data_sz;
	s_trcdata			**data_ls;
	
	s32_t 				proc_intf_sz;
	s_trcproc_intf		**proc_intf_ls;
	
	s32_t 				refs_intf_sz;
	s_trcrefs_intf 		**refs_intf_ls;
	
	s32_t 				obj_intf_sz;
	s_trcobj_intf		**obj_intf_ls;
	
	s32_t 				traj_intf_sz;
	s_trctraj_intf 		**traj_intf_ls;
	
	s32_t 				ctrl_intf_sz;
	s_trcctrl_intf		**ctrl_intf_ls;
	
	s32_t 				data_intf_sz;
	s_trcdata_intf		**data_intf_ls;

}	s_trceng;

typedef struct trceng_attr
{
	u8_t 				*stack;
	
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
	
	f64_t 				time_limit;
	f64_t 				time_step;
	
}	s_trceng_attr;

//------------------------------------------------------------------------------

s8_t trceng_init (s_trceng *eng, s_trceng_attr *attr)
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
s32_t trceng_obj_index (s_trceng *eng, s_trcobj *obj)
{
	return (s32_t) (((sp_t) eng->obj_ls - (sp_t) obj)
	/ (sp_t) sizeof(s_trcobj));
}

//------------------------------------------------------------------------------

void trceng_print (s_trceng *eng)
{
	printf("obj_sz %d \r\n", eng->obj_sz);
	printf("obj_ls \r\n");
	
	for (s32_t i = 0; i < eng->obj_sz; ++i)
	{
		printf("%s \r\n", eng->obj_ls[i]->name);
	}
}

//------------------------------------------------------------------------------

s8_t trceng_add_proc_intf (s_trceng *eng, s_trcproc_intf *proc_intf)
{
	eng->proc_intf_ls[eng->proc_intf_sz] = proc_intf;
	eng->proc_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_add_refs_intf (s_trceng *eng, s_trcrefs_intf *refs_intf)
{
	eng->refs_intf_ls[eng->refs_intf_sz] = refs_intf;
	eng->refs_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_add_obj_intf (s_trceng *eng, s_trcobj_intf *obj_intf)
{
	eng->obj_intf_ls[eng->obj_intf_sz] = obj_intf;
	eng->obj_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_add_traj_intf (s_trceng *eng, s_trctraj_intf *traj_intf)
{
	eng->traj_intf_ls[eng->traj_intf_sz] = traj_intf;
	eng->traj_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_add_ctrl_intf (s_trceng *eng, s_trcctrl_intf *ctrl_intf)
{
	eng->ctrl_intf_ls[eng->ctrl_intf_sz] = ctrl_intf;
	eng->ctrl_intf_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_add_data_intf (s_trceng *eng, s_trcdata_intf *data_intf)
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

s8_t trceng_save (s_trceng *eng, char *file_name)
{
	u8_t *__v_file__ = (u8_t*) malloc(256*1024*1024);
	u8_t *v_file = __v_file__;
	
	s_trceng *v_self = (s_trceng*) v_file;
	v_file += sizeof(s_trceng);
	
	*v_self = *eng;
	
	for (s32_t i = 0; i < eng->proc_sz; ++i)
	{
		trcproc_save(eng->proc_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (s32_t i = 0; i < eng->refs_sz; ++i)
	{
		trcrefs_save(eng->refs_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (s32_t i = 0; i < eng->obj_sz; ++i)
	{
		trcobj_save(eng->obj_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (s32_t i = 0; i < eng->traj_sz; ++i)
	{
		trctraj_save(eng->traj_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (s32_t i = 0; i < eng->ctrl_sz; ++i)
	{
		trcctrl_save(eng->ctrl_ls[i], &eng->intf_spl, &v_file);
	}
	
	for (s32_t i = 0; i < eng->data_sz; ++i)
	{
		trcdata_save(eng->data_ls[i], &eng->intf_spl, &v_file);
	}
	
	// must be after we did all the allocation on v_file
	// do not move up
	u32_t file_size = v_file - __v_file__;
	
	FILE *file_handle = fopen(file_name, "wb+");
	fwrite(&file_size, 1, sizeof(u32_t), file_handle);
	fwrite(__v_file__, 1, v_file - __v_file__, file_handle);
	fclose(file_handle);
	
	free(__v_file__);
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_load (s_trceng *eng, char *file_name)
{
//	FILE *file_handle = fopen(file_name, "rb");
//
//	if (file_handle != NULL)
//	{
//		eng->proc.free(&self->proc.data);
//
//		for (s32_t i = 0; i < self->obj_sz; ++i)
//		{
//			if (self->obj_ls[i].log_ls != NULL)
//			{
//				free(self->obj_ls[i].log_ls);
//				eng->obj_ls[i].log_ls = NULL;
//			}
//
//			for (s32_t j = 0; j < self->obj_ls[i].traj_sz; ++j)
//			{
//				eng->obj_ls[i].traj_ls[j].free(&self->obj_ls[i].traj_ls[j].data);
//			}
//
//			for (s32_t j = 0; j < self->obj_ls[i].ctrl_sz; ++j)
//			{
//				eng->obj_ls[i].ctrl_ls[j].free(&self->obj_ls[i].ctrl_ls[j].data);
//			}
//
//			for (s32_t j = 0; j < self->obj_ls[i].data_sz; ++j)
//			{
//				eng->obj_ls[i].data_ls[j].free(&self->obj_ls[i].data_ls[j].data);
//			}
//		}
//
//		u8_t *__v_file__ = (u8_t*) malloc(256*1024*1024);
//		u8_t *v_file = __v_file__;
//		u32_t file_size;
//
//		fread(&file_size, 1, sizeof(u32_t), file_handle);
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
//		for (s32_t i = 0; i < self->obj_sz; ++i)
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

s8_t trceng_reset (s_trceng *eng)
{
//	self->time[0] = 0.0;
//	self->time[1] = 0.0;
//
//	self->update_count = 0x01;
//	self->proc_count   = 0x00;
//
//	for (s32_t i = 0; i < self->obj_sz; ++i)
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
//		vl3_vset(obj->pos_force, 0.0);
//		vl3_vset(obj->rot_force, 0.0);
//
//		if (obj->log_ls != NULL)
//		{
//			free(obj->log_ls);
//			obj->log_ls = NULL;
//		}
//
//		for (s32_t j = 0; j < obj->traj_sz; ++j)
//		{
//			obj->traj_ls[j].compile(obj->traj_ls[j].data);
//		}
//
//		for (s32_t j = 0; j < obj->ctrl_sz; ++j)
//		{
//			obj->ctrl_ls[j].reset(obj->ctrl_ls[j].data, obj);
//		}
//
//		for (s32_t j = 0; j < obj->data_sz; ++j)
//		{
//			obj->data_ls[j].reset(obj->data_ls[j].data, obj);
//		}
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_update(s_trceng *self, f64_t d_time)
{
//	self->time[0] += d_time;
//	self->time[1]  = d_time;
//
//	for (s32_t i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &self->obj_ls[i];
//
//		// remove all forces coz they are additive
//		// thus all the plugins will add and never set
//		// so we need to zero it manually
//		vl3_vset(obj->pos_force, 0.0);
//		vl3_vset(obj->rot_force, 0.0);
//
//		for (s32_t j = 0; j < obj->ctrl_sz; ++j)
//		{
//			obj->ctrl_ls[j].update(obj->ctrl_ls[j].data, obj);
//		}
//	}
//
////	for (s32_t i = 0; i < self->obj_sz; ++i)
////	{
////	    vl3v_subv(&obj->pos[1][0], &obj->pos[0][0], &obj->log_ls[obj->log_sz-1] = &self->obj_ls[i];
////
////	    // remove all forces coz they are additive
////	    // thus all the plugins will add and never set
////	    // so we need to zero it manually
////	    vl3_vset(obj->pos_force, 0.0);
////	    vl3_vset(obj->rot_force, 0.0);
////
////	    for (s32_t j = 0; j < obj->ctrl_sz; ++j)
////	    {
////	        obj->ctrl_ls[j].update(obj->ctrl_ls[j].data, obj);
////	    }
////	}

	
	self->update_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_proc (s_trceng *eng)
{
//	for (s32_t i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &eng->obj_ls[i];
//		eng->proc.update(eng->proc.data, obj, self->proc_count);
//	}
//
	eng->proc_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_render (s_trceng *eng)
{
//	for (s32_t i = 0; i < eng->obj_sz; ++i)
//	{
//		s_trcobj *obj = &eng->obj_ls[i];
//
//		for (s32_t j = 0; j < obj->data_sz; ++j)
//		{
//			obj->data_ls[j].render(obj->data_ls[j].data, obj);
//		}
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_log (s_trceng *eng)
{
//	for (s32_t i = 0; i < self->obj_sz; ++i)
//	{
//		s_trcobj *obj = &eng->obj_ls[i];
//		s_trcobj_data *data = &obj->log_ls[obj->log_sz];
//
//		data->time[0] = eng->time[0];
//		data->time[1] = eng->time[1];
//
//		vl3m_copy(&data->rot[0][0], &obj->rot[0][0]);
//		vl3m_copy(&data->rot[1][0], &obj->rot[1][0]);
//		vl3m_copy(&data->rot[2][0], &obj->rot[2][0]);
//
//		vl3m_copy(&data->pos[0][0], &obj->pos[0][0]);
//		vl3m_copy(&data->pos[1][0], &obj->pos[1][0]);
//		vl3m_copy(&data->pos[2][0], &obj->pos[2][0]);
//
//		vl3v_copy(data->rot_force, obj->rot_force);
//		vl3v_copy(data->pos_force, obj->pos_force);
//
//		obj->log_sz++;
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCENG__ */
