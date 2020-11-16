
#include "trj_gui_w.h"

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
			
			trj_hpr_to_ctn(&vl3d_view.rot[0][0], view_hpr);
			
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
			
			trj_hpr_to_ctn(mat, rot_hpr);
			
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

//------------------------------------------------------------------------------
