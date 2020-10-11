
#ifndef __TRJ_GUI_OBJ__
#define __TRJ_GUI_OBJ__

#include <lib/imgui/imgui.h>

#include <lib/trj/trj_types.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_utils.h>
#include <lib/trj/vl.h>
#include <lib/trj/vl3d.h>
#include <lib/trj/vl_svd.h>

//------------------------------------------------------------------------------

typedef struct trj_gui_obj
{
	s_trj_obj *ref;
	
	uint8_t hide;
	
	uint8_t traj_sel;
	uint8_t ctrl_sel;
	uint8_t proc_sel;
	uint8_t data_sel;
	
} 	s_trj_gui_obj;

typedef struct trj_gui_obj_init
{
	s_trj_obj *ref;

} 	s_trj_gui_obj_init;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

uint8_t trj_gui_obj_edit(s_trj_gui_obj *gui, s_trj_obj *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_OBJ__ */
