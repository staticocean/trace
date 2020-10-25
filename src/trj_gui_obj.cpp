
#include "trj_gui_obj.h"

//------------------------------------------------------------------------------

uint8_t vl_gui_vec(char *label, vlf_t *vec, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalarN(label, ImGuiDataType_Double, vec, 3, v_speed, min, max, format);
	
	return 0x00;
}

uint8_t vl_gui_hpr(char *label, s_trj_rot_hpr *hpr, float v_speed, char *format)
{
	static vlf_t heading_min = 0.0;
	static vlf_t heading_max = 360.0;

	static vlf_t pitch_min = -90.0;
	static vlf_t pitch_max = +90.0;

	static vlf_t roll_min = -180.0;
	static vlf_t roll_max = +180.0;
	
	s_trj_rot_hpr hpr_deg;
	
	hpr_deg.heading = vl_deg(hpr->heading);
	hpr_deg.pitch   = vl_deg(hpr->pitch);
	hpr_deg.roll    = vl_deg(hpr->roll);
	
//	vlf_t hpr_deg[3] = {
//			(float) vl_deg(hpr->heading),
//			(float) vl_deg(hpr->pitch),
//			(float) vl_deg(hpr->roll)
//	};
//
	float item_width = ImGui::GetContentRegionAvailWidth() / 3 - 2.0/3.0 * ImGui::GetStyle().ItemInnerSpacing.x;
	
	ImGui::PushID(label);
	
//	vl_gui_vec("##hpr", hpr_deg, 1.0, NULL, NULL, "%.0f");
	
	ImGui::SetNextItemWidth(item_width);
	ImGui::DragScalar("##heading", ImGuiDataType_Double, &hpr_deg.heading, v_speed, &heading_min, &heading_max, format);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("heading [deg]"); }
	ImGui::SameLine(1*item_width+ImGui::GetStyle().ItemInnerSpacing.x, 0.0);
	ImGui::SetNextItemWidth(item_width);
	ImGui::DragScalar("##pitch", ImGuiDataType_Double, &hpr_deg.pitch  , v_speed, &pitch_min, &pitch_max, format);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("pitch [deg]"); }
	ImGui::SameLine(2*(item_width+ImGui::GetStyle().ItemInnerSpacing.x), 0.0);
	ImGui::SetNextItemWidth(item_width);
	ImGui::DragScalar("##roll", ImGuiDataType_Double, &hpr_deg.roll   , v_speed, &pitch_min, &pitch_max, format);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("roll [deg]"); }
	
	ImGui::PopID();
	
	hpr->heading = vl_rad(hpr_deg.heading);
	hpr->pitch   = vl_rad(hpr_deg.pitch);
	hpr->roll    = vl_rad(hpr_deg.roll);
	
//	hpr->heading = vl_rad(hpr_deg[0]);
//	hpr->pitch   = vl_rad(hpr_deg[1]);
//	hpr->roll    = vl_rad(hpr_deg[2]);
//
	return 0x00;
}

uint8_t vl_gui_mat(char *label, vlf_t *mat, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	ImGui::PushID(label);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	ImGui::BeginGroup();
	
	for (int i = 0; i < 3; ++i)
	{
		ImGui::PushID(i);
		ImGui::SetNextItemWidth(-1);
		ImGui::DragScalarN("##mat_view", ImGuiDataType_Double, &mat[3 * i], 3, v_speed, min, max, format);
		ImGui::PopID();
	}
	
	ImGui::EndGroup();

	ImGui::PopID();
	
	return 0x00;
}

uint8_t vl_gui_rot(char *label, vlf_t *mat)
{
	enum st
	{
		st_mode = 100,
		st_heading,
		st_pitch,
		st_roll,
		st_scale
	};
	
	ImGui::PushID(label);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	char st_id_mode[32];
	char st_id_heading[32];
	char st_id_pitch[32];
	char st_id_roll[32];
	char st_id_scale[32];
	
	sprintf(st_id_mode, "%s%d", label, st_mode);
	
	int mode = window->StateStorage.GetInt(ImGui::GetID(st_id_mode), 0x00);
	
	ImGui::BeginGroup();
	
	switch (mode)
	{
		default:
		{
			s_vl3d_eng vl3d_eng;
			s_vl3d_obj vl3d_obj_list[32];
			s_vl3d_view vl3d_view;
			
			sprintf(st_id_heading, "%s%d", label, st_heading);
			sprintf(st_id_pitch, "%s%d", label, st_pitch);
			sprintf(st_id_roll, "%s%d", label, st_roll);
			sprintf(st_id_scale, "%s%d", label, st_scale);
			
			s_trj_rot_hpr view_hpr;
			
			view_hpr.heading = window->StateStorage.GetFloat(ImGui::GetID(st_id_heading), vl_rad(45));
			view_hpr.pitch = window->StateStorage.GetFloat(ImGui::GetID(st_id_pitch), vl_rad(45));
			view_hpr.roll = window->StateStorage.GetFloat(ImGui::GetID(st_id_roll), vl_rad(0));
			
			trj_hpr_to_ctn(&vl3d_view.rot[0][0], &view_hpr);
			
			vl3d_view.scale = 0.75;
			vl_vzero(vl3d_view.pos);
			
			vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) { .obj_list = vl3d_obj_list });
			
			vl3d_eng_draw_arrow(&vl3d_eng,
								(float64_t[]) { -mat[0*3+0], -mat[1*3+0], -mat[2*3+0] },
								(float64_t[]) { +mat[0*3+0], +mat[1*3+0], +mat[2*3+0] }
			);

			vl3d_eng_draw_arrow(&vl3d_eng,
								(float64_t[]) { -mat[0*3+1], -mat[1*3+1], -mat[2*3+1] },
								(float64_t[]) { +mat[0*3+1], +mat[1*3+1], +mat[2*3+1] }
			);

			vl3d_eng_draw_arrow(&vl3d_eng,
								(float64_t[]) { -mat[0*3+2], -mat[1*3+2], -mat[2*3+2] },
								(float64_t[]) { +mat[0*3+2], +mat[1*3+2], +mat[2*3+2] }
			);

//			vl3d_eng_draw_arrow(&vl3d_eng,
//								(float64_t[]) { 0, 0, 0 },
//								(float64_t[]) { +mat[0*3+0], +mat[1*3+0], +mat[2*3+0] }
//			);
//
//			vl3d_eng_draw_arrow(&vl3d_eng,
//								(float64_t[]) { 0, 0, 0 },
//								(float64_t[]) { +mat[0*3+1], +mat[1*3+1], +mat[2*3+1] }
//			);
//
//			vl3d_eng_draw_arrow(&vl3d_eng,
//								(float64_t[]) { 0, 0, 0 },
//								(float64_t[]) { +mat[0*3+2], +mat[1*3+2], +mat[2*3+2] }
//			);
//
			vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { -1.0, +0.0, +0.0 }, (float64_t[]) { +1.0, +0.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { +0.0, -1.0, +0.0 }, (float64_t[]) { +0.0, +1.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { +0.0, +0.0, -1.0 }, (float64_t[]) { +0.0, +0.0, +1.0 } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { +1.0, +0.0, +0.0 }, .data = "X0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { +0.0, +1.0, +0.0 }, .data = "Y0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { +0.0, +0.0, +1.0 }, .data = "Z0" } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] }, .data = "X" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] }, .data = "Y" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] }, .data = "Z" } );
			
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { +1.0, +0.0, +0.0 }, .p1 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { +0.0, +1.0, +0.0 }, .p1 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { +0.0, +0.0, +1.0 }, .p1 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] } } );
			
			vl3d_eng_render(&vl3d_eng, &vl3d_view, "##mat_view",
					ImVec2(ImGui::GetContentRegionAvailWidth(),
						   ImGui::GetContentRegionAvailWidth()));
			
			trj_ctn_to_hpr(&view_hpr, &vl3d_view.rot[0][0]);
			
			window->StateStorage.SetFloat(ImGui::GetID(st_id_heading), (float) view_hpr.heading);
			window->StateStorage.SetFloat(ImGui::GetID(st_id_pitch), (float) view_hpr.pitch);
			window->StateStorage.SetFloat(ImGui::GetID(st_id_roll), (float) view_hpr.roll);
			window->StateStorage.SetFloat(ImGui::GetID(st_id_scale), (float) vl3d_view.scale);
			
			s_trj_rot_hpr rot_hpr;
			
			trj_ctn_to_hpr(&rot_hpr, mat);
			
			vl_gui_hpr("##hpr", &rot_hpr, 1.0, "%.0f");
			
			trj_hpr_to_ctn(mat, &rot_hpr);
			
			break;
		}
		
		case 0x01:
		{
			static vlf_t min = -1;
			static vlf_t max = +1;
			
			vl_gui_mat(label, mat, 0.001, &min, &max, "%.001f");
			
			break;
		}
	}
	
	ImGui::EndGroup();
	
	if (ImGui::BeginPopupContextItem("item context menu"))
	{
		if (ImGui::Selectable("3d")) mode = 0x00;
		if (ImGui::Selectable("mat")) mode = 0x01;

		ImGui::EndPopup();
	}
	
	window->StateStorage.SetInt(ImGui::GetID(st_id_mode), mode);
	
	ImGui::PopID();
	
	return 0x00;
}

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
		vlf_t time = 0.0;
		vlf_t time_step = 1.0;
		
		vlf_t p0[3];
		vlf_t p1[3];
		
		s_trj_traj traj = self->traj_list[i];
		
		for (int t = 0; t < 1000; ++t)
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
