
#include "trj_gui_obj.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_obj_init(s_trj_gui_obj *gui, s_trj_gui_obj_init attr)
{
	gui->ref = attr.ref;
	
	gui->traj_vl3d_view.scale = 1.0;
	vl_mid(&gui->traj_vl3d_view.rot[0][0]);
	vl_vset(gui->traj_vl3d_view.pos, 0.0);
	
	return 0x00;
}

uint8_t trj_gui_obj_edit(s_trj_gui_obj *gui, s_trj_obj *self)
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
	
//	ImGui::Text("ref");
//	ImGui::NextColumn();
//	const char* items[*self->obj_count];
//	for (int i = 0; i < *self->obj_count; ++i)
//	{ items[i] = self->obj_list[i].name; }
//	ImGui::SetNextItemWidth(-1);
//	if (ImGui::BeginCombo("##ref",
//			items[self->ref->id],
//			ImGuiComboFlags_NoArrowButton))
//	{
//		for (int i = 0; i < IM_ARRAYSIZE(items); ++i)
//		{
//			const bool is_selected = (self->ref->id == i);
//
//			if (ImGui::Selectable(items[i], is_selected)) { self->ref = &self->obj_list[i]; }
//			if (is_selected) { ImGui::SetItemDefaultFocus(); }
//		}
//
//		ImGui::EndCombo();
//	}
//	ImGui::NextColumn();
	
	ImGui::Text("pos_inert");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[kg]"); }
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##pos_inert", ImGuiDataType_Double, &self->pos_inert, 0.1, NULL, NULL, "%.3f");
	ImGui::NextColumn();
	
	ImGui::Text("rot_inert");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[?]"); }
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##rot_inert", ImGuiDataType_Double, &self->rot_inert, 0.1, NULL, NULL, "%.3f");
	ImGui::NextColumn();
	
	ImGui::Columns(1);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("POS");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
	ImGui::SameLine();
	vl_gui_vec("##pos_p", &self->pos[0][0], 1, NULL, NULL, "%.1f");
	
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
	vl_gui_rot("##rot", &self->rot[0][0]);
	vl_rnorm(&self->rot[0][0]);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Text("SOR");
	ImGui::SameLine();
	vl_gui_mat("##sor", &self->rot[1][0], 0.001, &min, &max, "%.3f");
	
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_gui_obj_traj(s_trj_gui_obj *gui, s_trj_obj *self)
{
	vl3d_eng_init(&gui->traj_vl3d_eng, (s_vl3d_eng_init) {
		.obj_list = gui->traj_vl3d_eng_objlist,
	});
	
	for (int i = 0; i < self->traj_offset; ++i)
	{
		vlf_t p0[3];
		vlf_t p1[3];
		
		s_trj_traj traj = self->traj_list[i];
		s_trj_traj_info traj_info;
		
		traj.info(traj.data, &traj_info);
		
		vlf_t time = traj_info.preview_time[0];
		vlf_t time_step = (traj_info.preview_time[1] - traj_info.preview_time[0]) / 1000.0;
		
		for (int t = 0; t < 1000-1; ++t)
		{
			traj.pos(traj.data, time, p0);
			traj.pos(traj.data, time+time_step, p1);
			
			vl3d_eng_add_line(&gui->traj_vl3d_eng, (s_vl3d_line) {
					.p0 = { p0[0], p0[1], p0[2] },
					.p1 = { p1[0], p1[1], p1[2] }
			});
			
			time = time + time_step;
		}
	}
	
	vl3d_eng_render(&gui->traj_vl3d_eng, &gui->traj_vl3d_view, "temp", ImVec2(-1, -1));
	
	return 0x00;
}

//------------------------------------------------------------------------------
