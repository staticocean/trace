
#ifndef __TRJ_GUI_CTRL__
#define __TRJ_GUI_CTRL__

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

#include <lib/trj/vl.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>

//------------------------------------------------------------------------------

inline void trj_gui_ctrl_edit(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##name", self->name, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

inline void trj_gui_ctrl_view(s_trj_ctrl *self)
{
	ImGui::Text("No view port available.");
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_CTRL__ */
