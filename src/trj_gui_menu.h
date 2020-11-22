
#ifndef __TRJ_GUI_MENU__
#define __TRJ_GUI_MENU__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <lib/trj/vl.h>
#include <lib/trj/trj_eng.h>

#include "trj_gui_env.h"
#include "trj_gui_cmd.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_menu
{
	s_trj_gui_env *env;
	s_trj_gui_cmd *cmd;
	
	int height;
	
} 	s_trj_gui_menu;

//------------------------------------------------------------------------------

uint8_t trj_gui_menu_file(s_trj_gui_menu *self);
uint8_t trj_gui_menu_main(s_trj_gui_menu *self);

//------------------------------------------------------------------------------

#endif
