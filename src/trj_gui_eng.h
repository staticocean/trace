
#ifndef __TRJ_GUI_ENG__
#define __TRJ_GUI_ENG__

#include <lib/imgui/imgui.h>

extern "C"
{
	#include <lib/trj/trj_types.h>
	#include <lib/trj/trj_eng.h>
	#include <lib/trj/trj_traj.h>
	#include <lib/trj/trj_ctrl.h>
}

#include "trj_gui_obj.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_eng
{
	s_trj_traj_api traj_api_list[32];
	uint32_t       traj_api_offset;
	
	void* sel_item;
	uint8_t sel_type;

	s_trj_gui_obj *obj_list;
	
}	s_trj_gui_eng;

typedef struct trj_gui_eng_init
{
	s_trj_gui_obj *obj_list;

} 	s_trj_gui_eng_init;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_init(s_trj_gui_eng *gui, s_trj_gui_eng_init attr);
uint8_t trj_gui_eng_add_trajapi(s_trj_gui_eng *gui, s_trj_traj_api api);
uint8_t trj_gui_eng_objlist(s_trj_gui_eng *gui, s_trj_eng *self);
uint8_t trj_gui_eng_addbox(s_trj_gui_eng *gui, s_trj_eng *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_ENG__ */
