
#include "trj_gui_obj.h"

//------------------------------------------------------------------------------

uint8_t vl_gui_vec(char *label, vlf_t *vec, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalarN(label, ImGuiDataType_Double, vec, 3, v_speed, min, max, format);
	
	return 0x00;
}


uint8_t vl_gui_mat(char *label, vlf_t *mat, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	bool user = false;
	
	ImGui::BeginGroup();
	
	for (int i = 0; i < 3; ++i)
	{
		ImGui::PushID(i);
		ImGui::SetNextItemWidth(-1);
		user |= ImGui::DragScalarN(label, ImGuiDataType_Double, &mat[3*i], 3, v_speed, min, max, format);
		ImGui::PopID();
	}
	
	ImGui::EndGroup();
	
	return 0x00;
}


uint8_t trj_gui_obj_edit(s_trj_obj *self)
{
	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##name", self->name, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Columns(2);
	
	ImGui::Text("id");
	ImGui::NextColumn();
	ImGui::Text("[%06d]", self->id);
	ImGui::NextColumn();
	
	ImGui::Text("ref");
	ImGui::NextColumn();
	const char* items[*self->obj_count];
	for (int i = 0; i < *self->obj_count; ++i)
	{ items[i] = self->obj_list[i].name; }
	ImGui::SetNextItemWidth(-1);
	if (ImGui::BeginCombo("##ref",
			items[self->ref->id],
			ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < IM_ARRAYSIZE(items); ++i)
		{
			const bool is_selected = (self->ref->id == i);
			
			if (ImGui::Selectable(items[i], is_selected)) { self->ref = &self->obj_list[i]; }
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
		}
		
		ImGui::EndCombo();
	}
	ImGui::NextColumn();
	
	ImGui::Text("pos_inert");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[kg]"); }
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##pos_inert", ImGuiDataType_Double, &self->pos_inert, 0.1);
	ImGui::NextColumn();
	
	ImGui::Text("rot_inert");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[?]"); }
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##rot_inert", ImGuiDataType_Double, &self->rot_inert, 0.1);
	ImGui::NextColumn();
	
	ImGui::Columns(1);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("POS");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
	ImGui::SameLine();
	vl_gui_vec("##pos", &self->pos[0][0], 1, NULL, NULL, "%.1f");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::AlignTextToFramePadding();
	ImGui::Text("VEL");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m/s]"); }
	ImGui::SameLine();
	vl_gui_vec("##vel", &self->pos[1][0], 0.1, NULL, NULL, "%.1f");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ACC");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m/s2]"); }
	ImGui::SameLine();
	vl_gui_vec("##acc", &self->pos[2][0], 0.1, NULL, NULL, "%.1f");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	vlf_t min = -1.0;
	vlf_t max = +1.0;
	
	ImGui::Text("ROT");
	ImGui::SameLine();
	vl_gui_mat("##rot", &self->rot[0][0], 0.001, &min, &max, "%.3f");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Text("SOR");
	ImGui::SameLine();
	vl_gui_mat("##sor", &self->rot[1][0], 0.001, &min, &max, "%.3f");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	
	
//	ImGui::Columns(2);
//	ImGui::Text("time");
//	ImGui::NextColumn();
//	if (self->ref == NULL) { ImGui::Text("none"); }
//	else { ImGui::Text("%s", (char*) self->ref->name); }
//	ImGui::NextColumn();
	
//	vlf_t time[3];
//	vlf_t pos[3][3];
//	vlf_t rot[3][9];
//
//	vlf_t pos_inert;
//	vlf_t rot_inert;
//
//	vlf_t pos_force[3];
//	vlf_t rot_force[3];
//
//	uint8_t ctrl_offset;
//	uint8_t proc_offset;
//	uint8_t data_offset;
//
	// traj list
	if(ImGui::CollapsingHeader("traj"))
	{
		for (int i = 0; i < self->traj_offset; ++i)
		{
		
		}
	}
	
	if (ImGui::CollapsingHeader("ctrl"))
	{
		for (int i = 0; i < self->ctrl_offset; ++i)
		{
		
		}
	}
	
	if (ImGui::CollapsingHeader("proc"))
	{
		for (int i = 0; i < self->proc_offset; ++i)
		{
		
		}
	}
	
	if (ImGui::CollapsingHeader("data"))
	{
		for (int i = 0; i < self->data_offset; ++i)
		{
		
		}
	}
	
//	static ImGuiTextFilter filter;
//	static void* selected_item = NULL;
//
//	uint8_t* filter_data[self->obj_count];
//
//	for (int i = 0; i < self->obj_count; ++i)
//	{
//		filter_data[i] = self->obj_list[i].name;
//	}
//
//	filter.Draw("");
//
//	ImGui::SameLine();
//	ImGui::Button("add obj");
//	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);
//
//	for (int i = 0; i < IM_ARRAYSIZE(filter_data); i++)
//	{
//		ImGui::PushID(i);
//
//		if (filter.PassFilter((char*) filter_data[i]))
//		{
//			s_trj_obj *obj = &self->obj_list[i];
//
//			bool node_open = ImGui::TreeNodeEx((void*) i, 0x00, (char*) obj->name);
//			static bool node_visible = true;
//
//			ImGui::SameLine();
//			bool temp;
//			ImGui::SmallButton("hide");
//
//			if (node_open)
//			{
//				if (ImGui::TreeNodeEx("Ctrl"))
//				{
//					if (obj->ctrl_offset == 0x00)
//					{
////						ImGui::
//					}
//
//					for (int j = 0; j < obj->ctrl_offset; ++j)
//					{
//
//					}
//
//					ImGui::TreePop();
//				}
//
//				if (ImGui::TreeNodeEx("Proc"))
//				{
//					ImGui::TreePop();
//				}
//
//				if (ImGui::TreeNodeEx("Data"))
//				{
//					ImGui::TreePop();
//				}
//
//				ImGui::TreePop();
//			}
//		}
//
//		ImGui::PopID();
//	}
//
//	ImGui::PopStyleVar();
	
	return 0x00;
}


//------------------------------------------------------------------------------
