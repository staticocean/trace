
#ifndef __TRJ_GUI_ENG__
#define __TRJ_GUI_ENG__

#include <lib/imgui/imgui.h>

#include "trj_types.h"
#include "trj_eng.h"

#include "trj_gui_obj.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_eng
{
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
uint8_t trj_gui_eng_objlist(s_trj_gui_eng *gui, s_trj_eng *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_ENG__ */
