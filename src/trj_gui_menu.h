
#ifndef __TRJ_GUI_MENU__
#define __TRJ_GUI_MENU__

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

#include "trj_gui_env.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_menu
{
	s_trj_gui_env *env;
	
	int height;
	
} 	s_trj_gui_menu;

//------------------------------------------------------------------------------

uint8_t trj_gui_menu_file(s_trj_gui_menu *self);
uint8_t trj_gui_menu_main(s_trj_gui_menu *self);

//------------------------------------------------------------------------------

#endif
