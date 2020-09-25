
#ifndef __TRJ_GUI_ENG__
#define __TRJ_GUI_ENG__

#include <lib/imgui/imgui.h>

#include "trj_types.h"
#include "trj_eng.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_eng
{
	void* sel_item;
	uint8_t sel_type;

	uint8_t *obj_hide;
	
}	s_trj_gui_eng;


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_objlist(s_trj_gui_eng *gui, s_trj_eng *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_ENG__ */
