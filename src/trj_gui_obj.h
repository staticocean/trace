
#ifndef __TRJ_GUI_OBJ__
#define __TRJ_GUI_OBJ__

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <lib/trj/vl.h>
#include <lib/trj/vl3d.h>
#include <lib/trj/vl_svd.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_utils.h>

#include "trj_gui_w.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_obj
{
	s_trj_obj *ref;
	
	uint8_t hide;
	
	uint8_t traj_sel;
	uint8_t ctrl_sel;
	uint8_t data_sel;
	
	s_vl3d_view traj_vl3d_view;
	s_vl3d_eng traj_vl3d_eng;
	s_vl3d_obj traj_vl3d_eng_objlist[8000];
	
} 	s_trj_gui_obj;

typedef struct trj_gui_obj_init
{
	s_trj_obj *ref;

} 	s_trj_gui_obj_init;

//------------------------------------------------------------------------------

uint8_t trj_gui_obj_init(s_trj_gui_obj *gui, s_trj_gui_obj_init attr);
uint8_t trj_gui_obj_edit(s_trj_gui_obj *gui, s_trj_obj *self);
uint8_t trj_gui_obj_traj(s_trj_gui_obj *gui, s_trj_obj *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_OBJ__ */
