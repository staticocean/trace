
#ifndef __TRJ_GUI_MENU__
#define __TRJ_GUI_MENU__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/imgui/imgui.h"

#include "lib/picoc/picoc.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_menu
{
	int height;
	
} 	s_trj_gui_menu;

//------------------------------------------------------------------------------

uint8_t trj_gui_menu_file(s_trj_gui_menu *self);
uint8_t trj_gui_menu_main(s_trj_gui_menu *self);

//------------------------------------------------------------------------------

#endif
