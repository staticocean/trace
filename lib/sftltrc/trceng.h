
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCENG__
#define __TRCENG__

//------------------------------------------------------------------------------

#include <string.h>

#include <sftlstd/vl.h>
#include <sftlstd/crc.h>

#include <sftltrc/trcapi.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trceng
{
	u32_t  			obj_sz;
	s_trcobj 		*obj_ls;
	
	f64_t 			time[2];
	
	f64_t 			time_limit;
	f64_t 			time_step;
	u32_t 			time_iter;
	
	s_trcproc 		proc;
	
	u32_t 			update_count;
	u32_t 			proc_count;
	
	u32_t 			ellp_sz;
	s_trcellp 		*ellp_ls;
	
	u32_t 			traj_sz;
	s_trctraj 		*traj_ls;
	
	u32_t 			ctrl_sz;
	s_trcctrl 		*ctrl_ls;
	
	u32_t 			data_sz;
	s_trcdata 		*data_ls;
	
	u32_t 			proc_sz;
	s_trcproc 		*proc_ls;
	
}	s_trceng;

typedef struct trceng_attr
{
	u8_t 			*stack;
	
	s_trcproc 		proc;
	
	s_trcobj  		*obj_ls;
	
	s_trcellp 		*ellp_ls;
	s_trctraj 		*traj_ls;
	s_trcctrl 		*ctrl_ls;
	s_trcdata 		*data_ls;
	s_trcproc 		*proc_ls;
	
	f64_t 			time_limit;
	f64_t 			time_step;
	
}	s_trceng_attr;

//------------------------------------------------------------------------------

s8_t trceng_init (s_trceng *eng, s_trceng_attr attr)
{
	eng->time_limit 	= attr.time_limit;
	eng->time_step  	= attr.time_step;
	
	if (eng->time_step < 1E-6) { eng->time_step = 1E-6; }
	
	eng->time_iter 		= eng->time_limit / eng->time_step;
	
	eng->ellp_ls 		= attr.ellp_ls;
	eng->ellp_sz 		= 0x00;
	
	eng->obj_ls 		= attr.obj_ls;
	eng->obj_sz 		= 0x00;
	
	eng->traj_ls 		= attr.traj_ls;
	eng->traj_sz 		= 0x00;
	
	eng->ctrl_ls 		= attr.ctrl_ls;
	eng->ctrl_sz 		= 0x00;
	
	eng->data_ls 		= attr.data_ls;
	eng->data_sz 		= 0x00;
	
	eng->proc_ls		= attr.proc_ls;
	eng->proc_sz 		= 0x00;
	
	eng->update_count 	= 0x00;
	eng->proc_count   	= 0x00;

	return 0x00;
}

inline s32_t trceng_obj_index (s_trceng *eng, s_trcobj *obj)
{
	return (u32_t) ((intptr_t) eng->obj_ls - (intptr_t) obj)
	/ sizeof(s_trcobj);
}

//------------------------------------------------------------------------------

void trceng_print (s_trceng *eng)
{
	printf("obj_sz %d \r\n", eng->obj_sz);
	printf("obj_ls \r\n");
	
	for (s32_t i = 0; i < eng->obj_sz; ++i)
	{
		printf("%s \r\n", eng->obj_ls[i].desc);
	}
}

//------------------------------------------------------------------------------

s8_t trceng_ellp_add (s_trceng *eng, s_trcellp *ellp)
{
	ellp->hash = crc32_iso_str(ellp->desc);
	
	eng->ellp_ls[eng->ellp_sz] = api;
	eng->ellp_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_traj_add (s_trceng *eng, s_trctraj *traj)
{
	traj->hash = crc32_iso_str(traj->desc);
	
	eng->traj_ls[eng->traj_sz] = api;
	eng->traj_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_ctrl_add (s_trceng *eng, s_trcctrl *ctrl)
{
	ctrl->hash = crc32_iso_str(ctrl->desc);
	
	eng->ctrl_ls[eng->ctrl_sz] = api;
	++eng->ctrl_sz;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_data_add (s_trceng *eng, s_trcdata *data)
{
	data->hash = crc32_iso_str(data->desc);
	
	eng->data_ls[eng->data_sz] = *data;
	eng->data_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_proc_add (s_trceng *eng, s_trcproc *proc)
{
	proc->hash = crc32_iso_str(proc->desc);
	
	eng->proc_ls[eng->proc_sz] = api;
	eng->proc_sz++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s_trcobj* trceng_obj_add (s_trceng *eng, s_trcobj_init attr)
{
	s_trcobj *obj = &eng->obj_ls[eng->obj_sz];
	
	trcobj_init(obj, attr);
	
	obj->hash 		= crc32_iso_str(obj->desc);
	obj->time 		= eng->time;
	obj->log_ls   	= NULL;
	obj->log_sz 	= 0x00;
	obj->pos_inert  = 1.0;
	obj->rot_inert  = 1.0;
	
	eng->obj_sz++;
	
	return obj;
}

//------------------------------------------------------------------------------

s8_t trceng_obj_del (s_trceng *eng, u32_t index)
{
	s_trcobj *obj = &eng->obj_ls[index];
	
	if (obj->log_ls != NULL)
	{
		free(obj->log_ls);
		obj->log_ls = NULL;
	}
	
	for (s32_t i = 0; i < obj->traj_sz; ++i)
	{
		obj->traj_ls[i].free(&obj->traj_ls[i].data);
	}
	
	for (s32_t i = 0; i < obj->ctrl_sz; ++i)
	{
		obj->ctrl_ls[i].free(&obj->ctrl_ls[i].data);
	}
	
	for (s32_t i = 0; i < obj->data_sz; ++i)
	{
		obj->data_ls[i].free(&obj->data_ls[i].data);
	}
	
	for (s32_t i = index; i < self->obj_sz; ++i)
	{
		self->obj_ls[i] = self->obj_ls[i+1];
	}
	
	self->obj_sz--;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_save (s_trceng *eng, char *file_name)
{
	u8_t *__v_file__ = (u8_t*) malloc(256*1024*1024);
	u8_t *v_file = __v_file__;
	
	s_trceng *v_self = (s_trceng*) v_file;
	v_file += sizeof(s_trceng);
	
	*v_self = *eng;

	trcproc_save(&eng->proc, self, &v_file);

	for (s32_t i = 0; i < eng->obj_sz; ++i)
	{
		trcobj_save(&eng->obj_ls[i], self, &v_file);
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
	FILE *file_handle = fopen(file_name, "rb");

	if (file_handle != NULL)
	{
		eng->proc.free(&self->proc.data);

		for (s32_t i = 0; i < self->obj_sz; ++i)
		{
			if (self->obj_ls[i].log_ls != NULL)
			{
				free(self->obj_ls[i].log_ls);
				eng->obj_ls[i].log_ls = NULL;
			}

			for (s32_t j = 0; j < self->obj_ls[i].traj_sz; ++j)
			{
				eng->obj_ls[i].traj_ls[j].free(&self->obj_ls[i].traj_ls[j].data);
			}

			for (s32_t j = 0; j < self->obj_ls[i].ctrl_sz; ++j)
			{
				eng->obj_ls[i].ctrl_ls[j].free(&self->obj_ls[i].ctrl_ls[j].data);
			}

			for (s32_t j = 0; j < self->obj_ls[i].data_sz; ++j)
			{
				eng->obj_ls[i].data_ls[j].free(&self->obj_ls[i].data_ls[j].data);
			}
		}

		u8_t *__v_file__ = (u8_t*) malloc(256*1024*1024);
		u8_t *v_file = __v_file__;
		u32_t file_size;

		fread(&file_size, 1, sizeof(u32_t), file_handle);
		fread(__v_file__, 1, file_size, file_handle);
		fclose(file_handle);

		s_trceng *v_self = (s_trceng *) v_file;
		v_file += sizeof(s_trceng);

		self->obj_sz = v_self->obj_sz;
		self->time_limit = v_self->time_limit;
		self->time_step = v_self->time_step;
		if (self->time_step < 1E-6) { self->time_step = 1E-6; }
		self->time_iter = self->time_limit / self->time_step;

		trcproc_load(&self->proc, self, &v_file);

		for (s32_t i = 0; i < self->obj_sz; ++i)
		{
			s_trcobj *obj = &self->obj_ls[i];

			trcobj_load(obj, self, &v_file);

			obj->time = self->time;
			obj->log_ls = NULL;
			obj->log_sz = 0x00;
			obj->hash = crc32_iso_str(obj->desc);
		}

		free(__v_file__);
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_reset (s_trceng *eng)
{
	self->time[0] = 0.0;
	self->time[1] = 0.0;
	
	self->update_count = 0x01;
	self->proc_count   = 0x00;
	
	for (s32_t i = 0; i < self->obj_sz; ++i)
	{
		s_trcobj *obj = &self->obj_ls[i];
		
		// must retain order
		// first reset offset then NULL the ptr
		// some RT plotting functions may still try to draw this
		obj->log_sz = 0x00;
		
		// remove all forces coz they are additive
		// thus all the plugins will add and never set
		// so we need to zero it manually
		vl3_vset(obj->pos_force, 0.0);
		vl3_vset(obj->rot_force, 0.0);
		
		if (obj->log_ls != NULL)
		{
			free(obj->log_ls);
			obj->log_ls = NULL;
		}
		
		for (s32_t j = 0; j < obj->traj_sz; ++j)
		{
			obj->traj_ls[j].compile(obj->traj_ls[j].data);
		}
		
		for (s32_t j = 0; j < obj->ctrl_sz; ++j)
		{
			obj->ctrl_ls[j].reset(obj->ctrl_ls[j].data, obj);
		}
		
		for (s32_t j = 0; j < obj->data_sz; ++j)
		{
			obj->data_ls[j].reset(obj->data_ls[j].data, obj);
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_update(s_trceng *self, f64_t d_time)
{
	self->time[0] += d_time;
	self->time[1]  = d_time;
	
	for (s32_t i = 0; i < self->obj_sz; ++i)
	{
		s_trcobj *obj = &self->obj_ls[i];
		
		// remove all forces coz they are additive
		// thus all the plugins will add and never set
		// so we need to zero it manually
		vl3_vset(obj->pos_force, 0.0);
		vl3_vset(obj->rot_force, 0.0);
		
		for (s32_t j = 0; j < obj->ctrl_sz; ++j)
		{
			obj->ctrl_ls[j].update(obj->ctrl_ls[j].data, obj);
		}
	}

//	for (s32_t i = 0; i < self->obj_sz; ++i)
//	{
//	    vl3_vsub(&obj->pos[1][0], &obj->pos[0][0], &obj->log_ls[obj->log_sz-1] = &self->obj_ls[i];
//
//	    // remove all forces coz they are additive
//	    // thus all the plugins will add and never set
//	    // so we need to zero it manually
//	    vl3_vset(obj->pos_force, 0.0);
//	    vl3_vset(obj->rot_force, 0.0);
//
//	    for (s32_t j = 0; j < obj->ctrl_sz; ++j)
//	    {
//	        obj->ctrl_ls[j].update(obj->ctrl_ls[j].data, obj);
//	    }
//	}

	
	self->update_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_proc (s_trceng *eng)
{
	for (s32_t i = 0; i < self->obj_sz; ++i)
	{
		s_trcobj *obj = &eng->obj_ls[i];
		eng->proc.update(eng->proc.data, obj, self->proc_count);
	}
	
	eng->proc_count++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_render (s_trceng *eng)
{
	for (s32_t i = 0; i < eng->obj_sz; ++i)
	{
		s_trcobj *obj = &eng->obj_ls[i];
		
		for (s32_t j = 0; j < obj->data_sz; ++j)
		{
			obj->data_ls[j].render(obj->data_ls[j].data, obj);
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

s8_t trceng_log (s_trceng *eng)
{
	for (s32_t i = 0; i < self->obj_sz; ++i)
	{
		s_trcobj *obj = &eng->obj_ls[i];
		s_trcobj_data *data = &obj->log_ls[obj->log_sz];
		
		data->time[0] = eng->time[0];
		data->time[1] = eng->time[1];
		
		vl3_mcopy(&data->rot[0][0], &obj->rot[0][0]);
		vl3_mcopy(&data->rot[1][0], &obj->rot[1][0]);
		vl3_mcopy(&data->rot[2][0], &obj->rot[2][0]);

		vl3_mcopy(&data->pos[0][0], &obj->pos[0][0]);
		vl3_mcopy(&data->pos[1][0], &obj->pos[1][0]);
		vl3_mcopy(&data->pos[2][0], &obj->pos[2][0]);
		
		vl3_vcopy(data->rot_force, obj->rot_force);
		vl3_vcopy(data->pos_force, obj->pos_force);
		
		obj->log_sz++;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __trceng__ */
