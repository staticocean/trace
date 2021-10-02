
#ifndef __GUI_CMD__
#define __GUI_CMD__

//----------------------------------------------------------------

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libcommon/vl.h>

#include <libgui/imgui/imgui.h>
#include <lib/trj/trj_eng.h>
#include <lib/trj/trj_obj.h>

#include "gui_env.h"

extern "C"
{
	#include <lib/fmemopen/fmemopen.h>
	#include <lib/picoc/picoc.h>
}

//----------------------------------------------------------------

typedef struct gui_cmd
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
	
	s_gui_env *env;
	bool visible;
	char title[255];
	
}	s_gui_cmd;

typedef struct gui_cmd_init
{
	s_gui_env *env;
	bool visible;
	char title[255];
	
} 	s_gui_cmd_init;

void gui_cmd_init(s_gui_cmd *self, s_gui_cmd_init attr);
void gui_cmd_addlog(s_gui_cmd *self, const char* fmt, ...);
void gui_cmd_clearlog(s_gui_cmd *self);
void gui_cmd_render(s_gui_cmd *self);
void gui_cmd_exec(s_gui_cmd *self);

//----------------------------------------------------------------


#endif /* __GUI_CMD__ */
