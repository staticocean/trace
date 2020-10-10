
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
		st_r00,
		st_r01,
		st_r02,
		st_r10,
		st_r11,
		st_r12,
		st_r20,
		st_r21,
		st_r22,
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
			
			vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init_attr) { .obj_list = vl3d_obj_list });
			
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
			
			vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { -1.0, +0.0, +0.0 }, (float64_t[]) { +1.0, +0.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { +0.0, -1.0, +0.0 }, (float64_t[]) { +0.0, +1.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { +0.0, +0.0, -1.0 }, (float64_t[]) { +0.0, +0.0, +1.0 } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { +1.0, +0.0, +0.0 }, .data = "x0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { +0.0, +1.0, +0.0 }, .data = "y0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { +0.0, +0.0, +1.0 }, .data = "z0" } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] }, .data = "x" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] }, .data = "y" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] }, .data = "z" } );
			
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { +1.0, +0.0, +0.0 }, .p1 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { +0.0, +1.0, +0.0 }, .p1 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { +0.0, +0.0, +1.0 }, .p1 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] } } );
			
			vl3d_eng_render(&vl3d_eng, &vl3d_view, "##mat_view", ImVec2(-1, -1));
			
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
//	vl_rnorm(&self->rot[0][0]);
	
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
//	vlf_t pos_p[3][3];
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
	
	ImGui::PopID();
	
	return 0x00;
}


//------------------------------------------------------------------------------
