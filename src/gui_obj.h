
#ifndef __GUI_OBJ__
#define __GUI_OBJ__

//----------------------------------------------------------------

#include <softael_lib/vl.h>
#include <softael_lib/vl3d.h>

#include <lib/imgui/imgui.h>
#include <lib/trj/trj_obj.h>

#include "gui_w.h"

//----------------------------------------------------------------

typedef struct gui_obj
{
	s_trj_obj *ref;
	uint8_t hide;
	
	s_vl3d_eng traj_vl3d_eng;
	s_vl3d_obj traj_vl3d_eng_objlist[8000];
	
} 	s_gui_obj;

typedef struct gui_obj_init
{
	s_trj_obj *ref;

} 	s_gui_obj_init;

//----------------------------------------------------------------

inline uint8_t gui_obj_init(s_gui_obj *gui, s_gui_obj_init attr)
{
	gui->ref = attr.ref;
	
	return 0x00;
}

inline uint8_t gui_obj_edit(s_gui_obj *gui, s_trj_obj *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::InputText("##desc", self->desc, 255);
	// !!! UPDATE HASH !!!
	self->hash = vl_crc32(self->desc);

	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Columns(2);
	
//	ImGui::Text("hash");
//	ImGui::NextColumn();
//	imgui_hash("##hash", self->hash);
//	ImGui::NextColumn();
	
	ImGui::Text("pos_inert");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[kg]"); }
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::DragScalar("##pos_inert", ImGuiDataType_Double, &self->pos_inert, 0.1, NULL, NULL, "%.3f");
	ImGui::NextColumn();
	
	ImGui::Text("rot_inert");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[?]"); }
	ImGui::NextColumn();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::DragScalar("##rot_inert", ImGuiDataType_Double, &self->rot_inert, 0.1, NULL, NULL, "%.3f");
	ImGui::NextColumn();
	
	ImGui::Columns();
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("POS");
//	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
//	ImGui::SameLine();
//	vl_gui_vec("##pos_p", &self->pos[0][0], 1, NULL, NULL, "%.1f");
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("VEL");
//	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m/s]"); }
//	ImGui::SameLine();
//	vl_gui_vec("##vel", &self->pos[1][0], 0.1, NULL, NULL, "%.1f");
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ACC");
//	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m/s2]"); }
//	ImGui::SameLine();
//	vl_gui_vec("##acc", &self->pos[2][0], 0.1, NULL, NULL, "%.1f");
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	vlf_t min = -1.0;
//	vlf_t max = +1.0;
//
//	ImGui::Text("ROT");
//	ImGui::SameLine();
//	vl_gui_rot("##rot", &self->rot[0][0]);
//	vl_rnorm(&self->rot[0][0]);
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Text("SOR");
//	ImGui::SameLine();
//	vl_gui_mat("##sor", &self->rot[1][0], 0.001, &min, &max, "%.3f");

//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t gui_obj_view(s_gui_obj *gui, s_trj_obj *self)
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
					.color = vl3d_col_l,
					.p0 = { p0[0], p0[1], p0[2] },
					.p1 = { p1[0], p1[1], p1[2] }
			});

			time = time + time_step;
		}
	}

	s_vl3d_view view = {
			
			.pos = { 0.0, 0.0, 0.0 },
			.scale = 1.0,

			.tbar_en = 0x01,
			
			.xyz_en = 0x01,
			.xyz_scale = 0.25,

			.grid_mode = 0x01,
			.grid_pt_size = 2.0,
			.grid_pt_disp = 2.0,
	};

	vl3d_view_load(self, &view, view);
	vl3d_view_grid(&view, &gui->traj_vl3d_eng);
	vl3d_view_xyz(&view, &gui->traj_vl3d_eng);
	vl3d_eng_render(&gui->traj_vl3d_eng, &view, "temp", ImVec2(-1, -1));
	vl3d_view_save(self, &view);
	
	return 0x00;
}

//----------------------------------------------------------------

#endif /* __GUI_OBJ__ */
