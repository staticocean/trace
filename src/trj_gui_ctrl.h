
#ifndef __TRJ_GUI_CTRL__
#define __TRJ_GUI_CTRL__

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

#include <lib/trj/vl.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>

//------------------------------------------------------------------------------

void trj_gui_ctrl_edit(s_trj_ctrl *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_CTRL__ */
