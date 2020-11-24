
#ifndef __TRJ_GUI_W__
#define __TRJ_GUI_W__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <lib/trj/vl.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_proc.h>
#include <lib/trj/trj_eng.h>

#include <lib/imgui/imgui_internal.h>
#include <lib/trj/vl3d.h>

//------------------------------------------------------------------------------

inline void vl_gui_mat_get(void *ptr, vlf_t *mat, vlf_t *def)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	s_trj_rot_hpr def_hpr;
	
	trj_ctn_to_hpr(&def_hpr, def);
	
	s_trj_rot_hpr hpr = {
//			.heading = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), vl_rad(45)),
//			.pitch 	 = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), vl_rad(45)),
//			.roll    = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), vl_rad( 0)),
			.heading = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), def_hpr.heading),
			.pitch 	 = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), def_hpr.pitch),
			.roll    = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), def_hpr.roll),
	};
	
	trj_hpr_to_ctn(mat, hpr);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_mat_set(void *ptr, vlf_t *mat)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	s_trj_rot_hpr hpr;
	
	trj_ctn_to_hpr(&hpr, mat);
	
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), (float) hpr.heading);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), (float) hpr.pitch);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), (float) hpr.roll);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_vec(char *label, vlf_t *vec, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalarN(label, ImGuiDataType_Double, vec, 3, v_speed, min, max, format);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_hpr(char *label, s_trj_rot_hpr *hpr, float v_speed, char *format)
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
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_mat(char *label, vlf_t *mat, float v_speed, vlf_t *min, vlf_t *max, char *format)
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
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_rot(char *label, vlf_t *mat)
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
			vl3d_view.tbar_en = 0x00;
			vl_vzero(vl3d_view.pos);
			
			vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) { .obj_list = vl3d_obj_list });
			
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l,
								(float64_t[]) { -mat[0*3+0], -mat[1*3+0], -mat[2*3+0] },
								(float64_t[]) { +mat[0*3+0], +mat[1*3+0], +mat[2*3+0] }
			);
			
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l,
								(float64_t[]) { -mat[0*3+1], -mat[1*3+1], -mat[2*3+1] },
								(float64_t[]) { +mat[0*3+1], +mat[1*3+1], +mat[2*3+1] }
			);
			
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l,
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
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l, (float64_t[]) { -1.0, +0.0, +0.0 }, (float64_t[]) { +1.0, +0.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l, (float64_t[]) { +0.0, -1.0, +0.0 }, (float64_t[]) { +0.0, +1.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l, (float64_t[]) { +0.0, +0.0, -1.0 }, (float64_t[]) { +0.0, +0.0, +1.0 } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { +1.0, +0.0, +0.0 }, .data = "X0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { +0.0, +1.0, +0.0 }, .data = "Y0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { +0.0, +0.0, +1.0 }, .data = "Z0" } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] }, .data = "X" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] }, .data = "Y" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] }, .data = "Z" } );
			
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .color = vl3d_col_l, .p0 = { +1.0, +0.0, +0.0 }, .p1 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .color = vl3d_col_l, .p0 = { +0.0, +1.0, +0.0 }, .p1 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .color = vl3d_col_l, .p0 = { +0.0, +0.0, +1.0 }, .p1 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] } } );
			
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
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_bool(char *label, ImVec2 size, uint8_t *data)
{
//	ImGuiStyle& style = ImGui::GetStyle();
//	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
//	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
//	ImU32 col_button_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Button]);
//
//	ImVec2 p = ImGui::GetCursorScreenPos();
//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
//
//	float height = ImGui::GetFrameHeight();
//	float width = height * 2.0f;
//
//	if (ImGui::InvisibleButton(label, ImVec2(width, height)))
//	{
//		*data = (!*data) & 0x01;
//	}
//
//	switch (*data)
//	{
//		case 0x00:
//		{
//			draw_list->AddRect(ImVec2(p.x, p.y), ImVec2(p.x+width, p.y+height), col_text_u32);
//			draw_list->AddText(ImVec2(p.x, p.y), col_text_u32, "OFF");
//
//			break;
//		}
//
//		default:
//		{
//			draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x+width, p.y+height), col_button_u32);
//			draw_list->AddText(ImVec2(p.x, p.y), col_text_u32, "ON");
//
//			break;
//		}
//	}
//
	ImGui::PushID(label);
	
	if (ImGui::Button((*data == 0x00) ? "OFF" : "ON", size))
	{ *data = (!*data) & 0x01; }
	
	ImGui::PopID();

//	ImU32 col_bg;
//
//	if (ImGui::IsItemHovered())
//		col_bg = *data ? IM_COL32(145+20, 211, 68+20, 255) : IM_COL32(218-20, 218-20, 218-20, 255);
//	else
//		col_bg = *data ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);
//
//	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg);
//	draw_list->AddRectFilled(ImVec2(*data ? (p.x + width) : (p.x), p.y), ImVec2(*data ? (p.x + width) : (p.x), p.y), IM_COL32(255, 255, 255, 255));
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_gui_switch(char *label, char *label_on, char *label_off, ImVec2 size, uint8_t *data)
{
	ImGui::PushID(label);
	
	if (ImGui::Button((*data == 0x00) ? label_off : label_on, size))
	{ *data = (!*data) & 0x01; }
	
	ImGui::PopID();

	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_objsel(char *label, uint32_t obj_count, s_trj_obj *obj_list, s_trj_obj **obj)
{
	if (ImGui::BeginCombo(label, (*obj)->desc, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < obj_count; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = ((*obj)->hash == obj_list[i].hash);
			
			if (ImGui::Selectable(obj_list[i].desc, is_selected))
			{ *obj = &obj_list[i]; }
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_ellpsel(char *label, uint32_t ellp_offset, s_trj_ellp *ellp_list, s_trj_ellp **ellp)
{
	bool is_none = *ellp == NULL;
	
	if (ImGui::BeginCombo(label,
						  is_none ? "none" : (*ellp)->desc,
						  ImGuiComboFlags_NoArrowButton))
	{
		if (ImGui::Selectable("none", is_none)) { *ellp = NULL; }
		if (is_none) { ImGui::SetItemDefaultFocus(); }
		
		// is_none can change after previous 2 lines
		is_none = *ellp == NULL;
		
		for (int i = 0; i < ellp_offset; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = false;
			
			if (!is_none && ((*ellp)->hash, ellp_list[i].hash))
			{ is_selected = true; }
			
			if (ImGui::Selectable(ellp_list[i].desc, is_selected))
			{ *ellp = &ellp_list[i]; }
			
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_procsel(char *label, s_trj_eng *eng)
{
	if (ImGui::BeginCombo(label, eng->proc->desc, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < eng->proc_offset; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = (eng->proc->hash == eng->proc_list[i].hash);
			
			if (ImGui::Selectable(eng->proc_list[i].desc, is_selected))
			{ eng->proc = &eng->proc_list[i]; }
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_OBJ__ */


