
#include "trj_gui_ctrl.h"

//------------------------------------------------------------------------------

void trj_gui_ctrl_edit(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##name", self->desc, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Columns(2);
	
	ImGui::Text("id");
	ImGui::NextColumn();
	ImGui::Text("[%06d]", self->id);
	ImGui::NextColumn();
//
//	ImGui::Text("pos_inert");
//	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[kg]"); }
//	ImGui::NextColumn();
//	ImGui::SetNextItemWidth(-1);
//	ImGui::DragScalar("##pos_inert", ImGuiDataType_Double, &self->pos_inert, 0.1, NULL, NULL, "%.3f");
//	ImGui::NextColumn();
	
	ImGui::Columns(1);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------
