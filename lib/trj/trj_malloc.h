
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_MALLOC__
#define __TRJ_MALLOC__

//------------------------------------------------------------------------------

#include "vl.h"

//------------------------------------------------------------------------------

typedef struct trj_ellp
{
	char desc[32];
	uint32_t hash;
	
	vlf_t a;
	vlf_t b;
	vlf_t c;
	vlf_t e;
	vlf_t f;
	
	vlf_t ee		;
	vlf_t invaa		;
	vlf_t l		 	;
	vlf_t p1mee		;
	vlf_t p1meedaa	;
	vlf_t Hmin		;
	vlf_t ll4		;
	vlf_t ll		;
	vlf_t invcbrt2	;
	
} 	s_trj_ellp;

//------------------------------------------------------------------------------

typedef struct trj_traj_info
{
	vlf_t preview_time[2];
	
} 	s_trj_traj_info;

#ifdef __TRJ_ENV__

typedef struct trj_traj
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	void *init;
	void *free;
	void *compile;
	void *info;
	void *pos;
	void *rot;
	
} 	s_trj_traj;

#else

typedef struct trj_traj
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	uint8_t (*init) 	(void **data, void *config);
	uint8_t (*free) 	(void **data);
	uint8_t (*compile) 	(void *data);
	uint8_t (*info) 	(void *data, s_trj_traj_info *info);
	uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *pos);
	uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *rot);
	
} 	s_trj_traj;

#endif

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__

typedef struct trj_ctrl
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	void *init;
	void *free;
	void *reset;
	void *update;
	
} 	s_trj_ctrl;

#else

typedef struct trj_ctrl
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	uint8_t (*init) 	(void **data, void *config);
	uint8_t (*free) 	(void **data);
	uint8_t (*reset) 	(void *data, void *obj);
	uint8_t (*update) 	(void *data, void *obj);
	
} 	s_trj_ctrl;

#endif

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__

typedef struct trj_data
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	void *init;
	void *free;
	void *reset;
	void *render;
	
} 	s_trj_data;

#else

typedef struct trj_data
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	uint8_t (*init) 	(void **data, void *config);
	uint8_t (*free) 	(void **data);
	uint8_t (*reset) 	(void *data, void *obj);
	uint8_t (*render) 	(void *data, void *obj);
	
} 	s_trj_data;

#endif

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__

typedef struct trj_proc
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	void *init;
	void *free;
	void *update;
	
} 	s_trj_proc;

#else

typedef struct trj_proc
{
	char desc[32];
	uint64_t hash;
	
	uint32_t config_size;
	void *config;
	
	uint32_t data_size;
	void *data;
	
	uint8_t (*init) 	(void **data, void *config);
	uint8_t (*free) 	(void **data);
	uint8_t (*update) 	(void *data, void *obj, uint32_t offset);
	
} 	s_trj_proc;

#endif

//------------------------------------------------------------------------------

typedef struct trj_obj_data
{
	vlf_t time[2];
	
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t pos_force[3];
	vlf_t rot_force[3];
	
}	s_trj_obj_data;

typedef struct trj_obj
{
	uint32_t id;
	
	char desc[32];
	
	vlf_t *time;
	
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t pos_force[3];
	vlf_t rot_force[3];
	
	vlf_t pos_inert;
	vlf_t rot_inert;
	
	s_trj_obj_data *log_list;
	uint32_t log_offset;
	
	uint8_t traj_offset;
	uint8_t ctrl_offset;
	uint8_t data_offset;
	
	s_trj_traj traj_list[8];
	s_trj_ctrl ctrl_list[8];
	s_trj_data data_list[8];
	
} 	s_trj_obj;

//------------------------------------------------------------------------------

#ifndef __TRJ_ENV__

inline uint8_t trj_obj_add_traj(s_trj_obj *self, s_trj_traj traj_api)
{
	traj_api.hash = vl_crc32(traj_api.desc);
	
	s_trj_traj *api = &self->traj_list[self->traj_offset];
	self->traj_offset += 0x01;
	
	*api = traj_api;
	api->init(&api->data, api->config);
	
	return 0x00;
}

inline uint8_t trj_obj_del_traj(s_trj_obj *self, s_trj_traj *api)
{
	int32_t i;
	int32_t offset = 0x00;
	
	api->free(&api->data);
	
	if (self->traj_offset == 0x00) { return 0x00; }
	
	for (i = 0; i < self->traj_offset-1; ++i)
	{
		if (&self->traj_list[i] == api)
		{ offset = 0x01; }
		
		self->traj_list[i] = self->traj_list[i+offset];
	}
	
	self->traj_offset -= 0x01;
	
	return 0x00;
}

inline uint8_t trj_obj_add_ctrl(s_trj_obj *self, s_trj_ctrl ctrl_api)
{
	ctrl_api.hash = vl_crc32(ctrl_api.desc);
	
	s_trj_ctrl *api = &self->ctrl_list[self->ctrl_offset];
	self->ctrl_offset += 0x01;
	
	*api = ctrl_api;
	api->init(&api->data, api->config);
	
	return 0x00;
}

inline uint8_t trj_obj_del_ctrl(s_trj_obj *self, s_trj_ctrl *api)
{
	int32_t i;
	int32_t offset = 0x00;
	
	api->free(&api->data);
	
	if (self->ctrl_offset == 0x00) { return 0x00; }
	
	for (i = 0; i < self->ctrl_offset-1; ++i)
	{
		if (&self->ctrl_list[i] == api)
		{ offset = 0x01; }
		
		self->ctrl_list[i] = self->ctrl_list[i+offset];
	}
	
	self->ctrl_offset -= 0x01;
	
	return 0x00;
}

inline uint8_t trj_obj_add_data(s_trj_obj *self, s_trj_data data_api)
{
	data_api.hash = vl_crc32(data_api.desc);
	
	s_trj_data *api = &self->data_list[self->data_offset];
	self->data_offset += 0x01;
	
	*api = data_api;
	api->init(&api->data, api->config);
	
	return 0x00;
}

inline uint8_t trj_obj_del_data(s_trj_obj *self, s_trj_data *api)
{
	int32_t i;
	int32_t offset = 0x00;
	
	api->free(&api->data);
	
	if (self->data_offset == 0x00) { return 0x00; }
	
	for (i = 0; i < self->data_offset-1; ++i)
	{
		if (&self->data_list[i] == api)
		{ offset = 0x01; }
		
		self->data_list[i] = self->data_list[i+offset];
	}
	
	self->data_offset -= 0x01;
	
	return 0x00;
}

#endif

//------------------------------------------------------------------------------

typedef struct trj_eng
{
	uint32_t  	obj_count;
	s_trj_obj 	*obj_list;
	
	vlf_t 		time[2];
	
	s_trj_proc 	*proc;
	
	uint32_t 	update_count;
	uint32_t 	proc_count;
	
	uint32_t 	ellp_offset;
	uint32_t 	traj_offset;
	uint32_t 	ctrl_offset;
	uint32_t 	data_offset;
	uint32_t 	proc_offset;
	
	s_trj_ellp 	*ellp_list;
	s_trj_traj 	*traj_list;
	s_trj_ctrl 	*ctrl_list;
	s_trj_data 	*data_list;
	s_trj_proc 	*proc_list;
	
}	s_trj_eng;

//------------------------------------------------------------------------------

inline uint8_t trj_traj_save(s_trj_traj *self, uint8_t **v_file)
{
	s_trj_traj *v_self = (s_trj_traj*) *v_file;
	*v_file += sizeof(s_trj_traj);
	
	*v_self = *self;
	
	memcpy(*v_file, self->config, self->config_size);
	*v_file += self->config_size;
	
	memcpy(*v_file, self->data, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

inline uint8_t trj_traj_load(s_trj_traj *self, uint8_t **v_file)
{
	s_trj_traj *v_self = (s_trj_traj*) *v_file;
	*v_file += sizeof(s_trj_traj);
	
	*self = *v_self;
	
	self->config = malloc(self->config_size);
	self->data   = malloc(self->data_size);
	
	memcpy(self->config, *v_file, self->config_size);
	*v_file += self->config_size;
	
	memcpy(self->data, *v_file, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_ctrl_save(s_trj_ctrl *self, uint8_t **v_file)
{
	s_trj_ctrl *v_self = (s_trj_ctrl*) *v_file;
	*v_file += sizeof(s_trj_ctrl);
	
	*v_self = *self;
	
	memcpy(*v_file, self->config, self->config_size);
	*v_file += self->config_size;
	
	memcpy(*v_file, self->data, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

inline uint8_t trj_ctrl_load(s_trj_ctrl *self, uint8_t **v_file)
{
	s_trj_ctrl *v_self = (s_trj_ctrl*) *v_file;
	*v_file += sizeof(s_trj_ctrl);
	
	*self = *v_self;
	
	self->config = malloc(self->config_size);
	self->data   = malloc(self->data_size);
	
	memcpy(self->config, *v_file, self->config_size);
	*v_file += self->config_size;
	
	memcpy(self->data, *v_file, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_data_save(s_trj_data *self, uint8_t **v_file)
{
	s_trj_data *v_self = (s_trj_data*) *v_file;
	*v_file += sizeof(s_trj_data);
	
	*v_self = *self;
	
	memcpy(*v_file, self->config, self->config_size);
	*v_file += self->config_size;
	
	memcpy(*v_file, self->data, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

inline uint8_t trj_data_load(s_trj_data *self, uint8_t **v_file)
{
	s_trj_data *v_self = (s_trj_data*) *v_file;
	*v_file += sizeof(s_trj_data);
	
	*self = *v_self;
	
	self->config = malloc(self->config_size);
	self->data   = malloc(self->data_size);
	
	memcpy(self->config, *v_file, self->config_size);
	*v_file += self->config_size;
	
	memcpy(self->data, *v_file, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_proc_save(s_trj_proc *self, uint8_t **v_file)
{
	s_trj_proc *v_self = (s_trj_proc*) *v_file;
	*v_file += sizeof(s_trj_proc);
	
	*v_self = *self;
	
	memcpy(*v_file, self->config, self->config_size);
	*v_file += self->config_size;
	
	memcpy(*v_file, self->data, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

inline uint8_t trj_proc_load(s_trj_proc *self, uint8_t **v_file)
{
	s_trj_proc *v_self = (s_trj_proc*) *v_file;
	*v_file += sizeof(s_trj_proc);
	
	*self = *v_self;
	
	self->config = malloc(self->config_size);
	self->data   = malloc(self->data_size);
	
	memcpy(self->config, *v_file, self->config_size);
	*v_file += self->config_size;
	
	memcpy(self->data, *v_file, self->data_size);
	*v_file += self->data_size;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline void trj_obj_save(s_trj_obj *self, uint8_t **v_file)
{
	uint32_t i;
	
	s_trj_obj *v_self = (s_trj_obj*) *v_file;
	*v_file += sizeof(s_trj_obj);
	
	*v_self = *self;
	
	v_self->time       = NULL;
	v_self->log_list   = NULL;
	v_self->log_offset = 0x00;
	
	for (i = 0; i < self->traj_offset; ++i)
	{ trj_traj_save(&self->traj_list[i], v_file); }
	
	for (i = 0; i < self->ctrl_offset; ++i)
	{ trj_ctrl_save(&self->ctrl_list[i], v_file); }
	
	for (i = 0; i < self->data_offset; ++i)
	{ trj_data_save(&self->data_list[i], v_file); }
	
	return;
}

inline void trj_obj_load(s_trj_obj *self, uint8_t **v_file)
{
	uint32_t i;
	
	s_trj_obj *v_self = (s_trj_obj*) *v_file;
	*v_file += sizeof(s_trj_obj);
	
	*self = *v_self;
	
	for (i = 0; i < self->traj_offset; ++i)
	{ trj_traj_load(&self->traj_list[i], v_file); }
	
	for (i = 0; i < self->ctrl_offset; ++i)
	{ trj_ctrl_load(&self->ctrl_list[i], v_file); }
	
	for (i = 0; i < self->data_offset; ++i)
	{ trj_data_load(&self->data_list[i], v_file); }
	
	return;
}

//------------------------------------------------------------------------------

inline uint8_t trj_eng_save(s_trj_eng *self, char *file_name)
{
	uint32_t i;
	
	uint8_t *__v_file__ = (uint8_t*) malloc(256*1024*1024);
	uint8_t *v_file = __v_file__;
	
	s_trj_eng *v_self = (s_trj_eng*) v_file;
	v_file += sizeof(s_trj_eng);
	
	*v_self = *self;
	
	for (i = 0; i < self->obj_count; ++i)
	{ trj_obj_save(&self->obj_list[i], &v_file); }
	
	// must be after we did all the allocation on v_file
	// do not move up
	uint32_t file_size = v_file - __v_file__;
	
	FILE *file_handle = fopen(file_name, "wb+");
	fwrite(&file_size, 1, sizeof(uint32_t), file_handle);
	fwrite(__v_file__, 1, v_file - __v_file__, file_handle);
	fclose(file_handle);
	
	free(__v_file__);
	
	return 0x00;
}

inline s_trj_traj* trj_eng_find_traj(s_trj_eng *self, s_trj_traj *traj)
{
	s_trj_traj *res = NULL;
	uint32_t i;
	
	for (i = 0; i < self->traj_offset; ++i)
	{
		if (self->traj_list[i].hash == traj->hash)
		{
			res = &self->traj_list[i];
			return res;
		}
	}
	
	return res;
}

inline s_trj_ctrl* trj_eng_find_ctrl(s_trj_eng *self, s_trj_ctrl *ctrl)
{
	s_trj_ctrl *res = NULL;
	uint32_t i;
	
	for (i = 0; i < self->ctrl_offset; ++i)
	{
		if (self->ctrl_list[i].hash == ctrl->hash)
		{
			res = &self->ctrl_list[i];
			return res;
		}
	}
	
	return res;
}

inline s_trj_data* trj_eng_find_data(s_trj_eng *self, s_trj_data *data)
{
	s_trj_data *res = NULL;
	uint32_t i;
	
	for (i = 0; i < self->data_offset; ++i)
	{
		if (self->data_list[i].hash == data->hash)
		{
			res = &self->data_list[i];
			return res;
		}
	}
	
	return res;
}

inline uint8_t trj_eng_load(s_trj_eng *self, char *file_name)
{
	uint32_t i;
	uint32_t j;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		if (self->obj_list[i].log_list != NULL)
		{
			free(self->obj_list[i].log_list);
			self->obj_list[i].log_list = NULL;
		}
		
		for (j = 0; j < self->obj_list[i].traj_offset; ++j)
		{ self->obj_list[i].traj_list[j].free(&self->obj_list[i].traj_list[j].data); }
		
		for (j = 0; j < self->obj_list[i].ctrl_offset; ++j)
		{ self->obj_list[i].ctrl_list[j].free(&self->obj_list[i].ctrl_list[j].data); }
		
		for (j = 0; j < self->obj_list[i].data_offset; ++j)
		{ self->obj_list[i].data_list[j].free(&self->obj_list[i].data_list[j].data); }
	}

	uint8_t *__v_file__ = (uint8_t*) malloc(256*1024*1024);
	uint8_t *v_file = __v_file__;
	uint32_t file_size;

	FILE *file_handle = fopen(file_name, "rb");
	fread(&file_size, 1, sizeof(uint32_t), file_handle);
	fread(__v_file__, 1, file_size, file_handle);
	fclose(file_handle);
	
	s_trj_eng *v_self = (s_trj_eng*) v_file;
	v_file += sizeof(s_trj_eng);
	
//	*self = *v_self;
	
	self->obj_count = v_self->obj_count;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		s_trj_obj *obj = &self->obj_list[i];
		trj_obj_load(obj, &v_file);
		
		obj->time = self->time;
		obj->log_list = NULL;
		obj->log_offset = 0x00;
		obj->id = i;
		
		for (j = 0; j < obj->traj_offset; ++j)
		{
			obj->traj_list[j].init    = trj_eng_find_traj(self, &obj->traj_list[j])->init  ;
			obj->traj_list[j].free    = trj_eng_find_traj(self, &obj->traj_list[j])->free  ;
			obj->traj_list[j].compile = trj_eng_find_traj(self, &obj->traj_list[j])->compile;
			obj->traj_list[j].pos     = trj_eng_find_traj(self, &obj->traj_list[j])->pos   ;
			obj->traj_list[j].rot     = trj_eng_find_traj(self, &obj->traj_list[j])->rot   ;
			obj->traj_list[j].info    = trj_eng_find_traj(self, &obj->traj_list[j])->info  ;
		}
		
		for (j = 0; j < obj->ctrl_offset; ++j)
		{
			obj->ctrl_list[j].init    = trj_eng_find_ctrl(self, &obj->ctrl_list[j])->init  ;
			obj->ctrl_list[j].free    = trj_eng_find_ctrl(self, &obj->ctrl_list[j])->free  ;
			obj->ctrl_list[j].reset   = trj_eng_find_ctrl(self, &obj->ctrl_list[j])->reset ;
			obj->ctrl_list[j].update  = trj_eng_find_ctrl(self, &obj->ctrl_list[j])->update;
		}
		
		for (j = 0; j < obj->data_offset; ++j)
		{
			obj->data_list[j].init    = trj_eng_find_data(self, &obj->data_list[j])->init  ;
			obj->data_list[j].free    = trj_eng_find_data(self, &obj->data_list[j])->free  ;
			obj->data_list[j].render  = trj_eng_find_data(self, &obj->data_list[j])->render;
		}
	}
	
	free(__v_file__);
	
	return 0x00;
}
//------------------------------------------------------------------------------

#endif /* __TRJ_API__ */




