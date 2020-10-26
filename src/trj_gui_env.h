
#ifndef __TRJ_GUI_ENV_H__
#define __TRJ_GUI_ENV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
	#include <lib/fmemopen/fmemopen.h>
	
	#include <lib/picoc/picoc.h>
	#include <lib/picoc/picoc_interpreter.h>
	
	#include <lib/trj/trj_types.h>
	#include <lib/trj/trj_eng.h>
	#include <lib/trj/trj_obj.h>
	#include <lib/trj/vl.h>
	#include <lib/trj/vl_svd.h>
}

typedef struct trj_gui_env
{
	Picoc env;
	s_trj_eng *eng;
	
	uint8_t out_s_data[1024*1024];
	FILE *out_s;
	
	void *traj_offset;
	void *ctrl_offset;
	void *proc_offset;
	void *data_offset;
	
	void *traj_list;
	void *ctrl_list;
	void *proc_list;
	void *data_list;
	
} 	s_trj_gui_env;

typedef struct trj_gui_env_init
{
	s_trj_eng *eng;
	
	void *traj_offset;
	void *ctrl_offset;
	void *proc_offset;
	void *data_offset;
	
	void *traj_list;
	void *ctrl_list;
	void *proc_list;
	void *data_list;
	
} 	s_trj_gui_env_init;

void trj_gui_env_init(s_trj_gui_env *self, s_trj_gui_env_init attr);
void trj_gui_env_reset(s_trj_gui_env *self);
void trj_gui_env_sreset(s_trj_gui_env *self);

#define PICOC_STACK_SIZE (32*1024*1024)

#endif /* __TRJ_GUI_ENV_H__ */

