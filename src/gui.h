
#ifndef __GUI__
#define __GUI__

//----------------------------------------------------------------

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lib/imgui/imgui.h>
#include <lib/picoc/picoc.h>
#include <softael_lib/vl.h>

#include <lib/trj/trj_eng.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_ellp.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_traj_.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_proc.h>

#include "imgui_custom.h"

#include "gui_eng.h"
#include "gui_obj.h"
#include "gui_map.h"
#include "gui_tbar.h"
#include "gui_traj.h"
#include "gui_ctrl.h"
#include "gui_data.h"
#include "gui_cmd.h"
#include "gui_env.h"

//----------------------------------------------------------------

typedef struct trj_gui
{
	float32_t w_height;
	float32_t w_width;
	
	s_trj_eng eng;
	s_trj_obj  st_eng_obj_list [255];
	s_trj_ellp st_eng_ellp_list[255];
	s_trj_traj st_eng_traj_list[255];
	s_trj_ctrl st_eng_ctrl_list[255];
	s_trj_data st_eng_data_list[255];
	s_trj_proc st_eng_proc_list[255];
	
	s_gui_obj st_gui_eng_obj[255];
	
	s_gui_tbar gui_tbar;
	s_gui_eng  gui_eng;
	s_gui_cmd  gui_cmd;
	s_gui_env  gui_env;
	
} 	s_trj_gui;

typedef struct gui_init_attr
{
	uint32_t temp;
	
}	s_gui_init;

//----------------------------------------------------------------

uint8_t gui_init(s_trj_gui *self, s_gui_init attr);
uint8_t gui_main(s_trj_gui *self);

//----------------------------------------------------------------

#endif
