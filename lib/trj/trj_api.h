
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_API__
#define __TRJ_API__

//------------------------------------------------------------------------------

#include "vl.h"

//------------------------------------------------------------------------------

typedef struct trj_traj_info
{
	vlf_t preview_time[2];
	
} 	s_trj_traj_info;

#ifdef __TRJ_ENV__

typedef struct trj_traj
{
	uint32_t id;
	
	char desc[32];
	
	void *data;
	void *config;
	
	void *init;
	void *free;
	void *compile;
	void *info;
	void *rot;
	void *pos;
	
} 	s_trj_traj;

#else

typedef struct trj_traj
{
	uint32_t id;
	
	char desc[32];
	
	void *data;
	void *config;
	
	uint8_t (*init) 	(void **data, void *config);
	uint8_t (*free) 	(void **data);
	uint8_t (*compile) 	(void *data);
	uint8_t (*info) 	(void *data, s_trj_traj_info *info);
	uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *pos);
	uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *rot);
	
} 	s_trj_traj;

#endif

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__

typedef struct trj_ctrl
{
	uint32_t id;
	
	char desc[32];
	
	void *data;
	void *config;
	
	void *init;
	void *free;
	void *reset;
	void *update;
	
} 	s_trj_ctrl;

#else

typedef struct trj_ctrl
{
	uint32_t id;
	
	char desc[32];
	
	void *data;
	void *config;
	
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
	uint32_t id;
	
	char desc[32];
	
	void *data;
	void *config;
	
	void *init;
	void *free;
	void *render;
	
} 	s_trj_data;

#else

typedef struct trj_data
{
	uint32_t id;
	
	char desc[32];
	
	void *data;
	void *config;
	
	uint8_t (*init) 	(void **data, void *config);
	uint8_t (*free) 	(void **data);
	uint8_t (*render) 	(void *data, void *obj);
	
} 	s_trj_data;

#endif

//------------------------------------------------------------------------------

typedef struct trj_obj_data
{
	vlf_t time[3];
	
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t pos_force[3];
	vlf_t rot_force[3];
	
}	s_trj_obj_data;

typedef struct trj_obj
{
	struct trj_obj *obj_list;
	uint32_t *obj_count;
	
	struct trj_obj *ref;
	
	uint32_t id;
	char name[32];
	
	vlf_t *time;
	
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t pos_force[3];
	vlf_t rot_force[3];
	
	s_trj_obj_data *log_list;
	uint32_t 	    log_offset;
	
	vlf_t pos_inert;
	vlf_t rot_inert;
	
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
	s_trj_traj *api = &self->traj_list[self->traj_offset];
	self->traj_offset += 0x01;
	
	*api = traj_api;
	api->init(&api->data, api->config);
	api->compile(api->data);
	
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

#endif /* __TRJ_API__ */



