
#ifndef __TRJ_GUI_CTRL__
#define __TRJ_GUI_CTRL__

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>
#include <math.h>

extern "C"
{
	#include <lib/trj/trj_types.h>
	#include <lib/trj/trj_obj.h>
	#include <lib/trj/trj_traj.h>
	#include <lib/trj/vl.h>
}

//------------------------------------------------------------------------------

void trj_gui_ctrl_edit(s_trj_ctrl *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_CTRL__ */
