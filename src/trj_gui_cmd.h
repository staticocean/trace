
#ifndef __TRJ_GUI_CMD__
#define __TRJ_GUI_CMD__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/imgui/imgui.h"

extern "C"
{
	#include <lib/fmemopen/fmemopen.h>

	#include <lib/picoc/picoc.h>
}

#include <isss_lib/vl.h>
#include <isss_lib/bz.h>

#include <lib/trj/trj_eng.h>
#include <lib/trj/trj_obj.h>

#include "trj_gui_env.h"

//------------------------------------------------------------------------------

typedef struct trj_gui_cmd
{
	char in_buff[2048];
	char out_buff[2048];
	ImVector<char *> Items;
	ImVector<const char *> Commands;
	ImVector<char *> History;
	int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter Filter;
	bool AutoScroll;
	bool ScrollToBottom;
	
	s_trj_gui_env *env;
	bool visible;
	char title[255];
	
}	s_trj_gui_cmd;

typedef struct trj_gui_cmd_init
{
	s_trj_gui_env *env;
	bool visible;
	char title[255];
	
} 	s_trj_gui_cmd_init;

void trj_gui_cmd_init(s_trj_gui_cmd *self, s_trj_gui_cmd_init attr);
void trj_gui_cmd_addlog(s_trj_gui_cmd *self, const char* fmt, ...);
void trj_gui_cmd_clearlog(s_trj_gui_cmd *self);
void trj_gui_cmd_render(s_trj_gui_cmd *self);
void trj_gui_cmd_exec(s_trj_gui_cmd *self);

//------------------------------------------------------------------------------


#endif /* __TRJ_GUI_CMD__ */
