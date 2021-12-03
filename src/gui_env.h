
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_ENV_H__
#define __GUI_ENV_H__

//------------------------------------------------------------------------------

#define PICOC_STACK_SIZE (32*1024*1024)

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcommon/vl.h>

#include <lib/trj/trceng.h>
#include <lib/trj/trcobj.h>

extern "C"
{
	#include <lib/fmemopen/fmemopen.h>
	#include <lib/picoc/picoc.h>
	#include <lib/picoc/picoc_interpreter.h>
}

//------------------------------------------------------------------------------

typedef struct gui_env
{
	Picoc env;
	s_trceng *eng;
	
	u8_t out_s_data[1024*1024];
	FILE *out_s;
	
	void *traj_sz;
	void *ctrl_sz;
	void *data_offset;
	
	void *traj_ls;
	void *ctrl_ls;
	void *data_list;
	
	void **api_ctrl_data_ref;
	void *api_ctrl_data;
	void *api_ctrl_config;
	void *api_ctrl_obj;
	
	void **api_data_data_ref;
	void *api_data_data;
	void *api_data_config;
	void *api_data_obj;
	
} 	s_gui_env;

typedef struct gui_env_init
{
	s_trceng *eng;
	
	void *traj_sz;
	void *ctrl_sz;
	void *data_offset;
	
	void *traj_ls;
	void *ctrl_ls;
	void *data_list;
	
} 	s_gui_env_init;

//------------------------------------------------------------------------------

void gui_env_init(s_gui_env *self, s_gui_env_init attr);
void gui_env_reset(s_gui_env *self);
void gui_env_sreset(s_gui_env *self);

//------------------------------------------------------------------------------

#endif /* __GUI_ENV_H__ */

