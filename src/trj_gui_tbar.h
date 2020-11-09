
#ifndef __TRJ_GUI_TBAR__
#define __TRJ_GUI_TBAR__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lib/imgui/imgui.h>

extern "C"
{
	#include <lib/trj/trj_eng.h>
	#include <lib/trj/trj_types.h>
}

#include "trj_gui_eng.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_tbar
{
	int height;
	
	s_trj_gui_eng *eng_gui;
	
} 	s_trj_gui_tbar;

//------------------------------------------------------------------------------

uint8_t trj_gui_tbar_main(s_trj_gui_tbar *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_TBAR__ */




