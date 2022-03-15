
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_OBJ__
#define __GUI_OBJ__

//------------------------------------------------------------------------------

#include <implot/implot.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <sftlstd/vld3.h>
#include <sftlstd/vl3d.h>
#include <sftlstd/vl3d_imgui.h>

#include <sftlgui/sftlgui.h>

#include <sftltrc/sftltrc.h>

#include "gui_w.h"

//------------------------------------------------------------------------------

typedef struct gui_obj
{
	s_trcobj *ref;
	t_u8 hide;
	
	s_vl3d traj_vl3d_eng;
	s_vl3d_obj traj_vl3d_objlist[8000];
	
} 	s_gui_obj;

typedef struct gui_obj_init
{
	s_trcobj *ref;

} 	s_gui_obj_init;

//------------------------------------------------------------------------------

inline t_u8 gui_obj_init(s_gui_obj *gui, s_gui_obj_init attr)
{
	gui->ref = attr.ref;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline t_u8 gui_obj_edit(s_gui_obj *gui, s_trcobj *self)
{
//	ImGui::PushID(self);
//
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	ImGui::InputText("##desc", self->desc, 255);
//	// !!! UPDATE HASH !!!
//	self->hash = crc32_iso_str(self->desc);
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
////	ImGui::Text("hash");
////	ImGui::NextColumn();
////	imgui_hash("##hash", self->hash);
////	ImGui::NextColumn();
//
//	t_f64 pos_inert_min = 0.0;
//	t_f64 rot_inert_min = 0.0;
//
//	ImGui::Text("pos_inert");
//	gui_hint("[kg]");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	ImGui::DragScalar("##pos_inert", ImGuiDataType_Double,
//			&self->pos_inert, 1.0, &pos_inert_min, NULL, "%.3f");
//
//	ImGui::Text("rot_inert");
//	gui_hint("[kg m2]");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	ImGui::DragScalar("##rot_inert", ImGuiDataType_Double,
//			&self->rot_inert, 1.0, &rot_inert_min, NULL, "%.3f");
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
////	ImGui::AlignTextToFramePadding();
////	ImGui::Text("POS");
////	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
////	ImGui::SameLine();
////	vl_gui_vec("##pos_p", &self->pos[0][0], 1, NULL, NULL, "%.1f");
////
////	ImGui::Dummy(ImVec2(0, 5));
////	ImGui::AlignTextToFramePadding();
////	ImGui::Text("VEL");
////	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m/s]"); }
////	ImGui::SameLine();
////	vl_gui_vec("##vel", &self->pos[1][0], 0.1, NULL, NULL, "%.1f");
////
////	ImGui::Dummy(ImVec2(0, 5));
////	ImGui::AlignTextToFramePadding();
////	ImGui::Text("ACC");
////	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m/s2]"); }
////	ImGui::SameLine();
////	vl_gui_vec("##acc", &self->pos[2][0], 0.1, NULL, NULL, "%.1f");
////
////	ImGui::Dummy(ImVec2(0, 5));
////	ImGui::Separator();
////	ImGui::Dummy(ImVec2(0, 5));
////
////	t_f64 min = -1.0;
////	t_f64 max = +1.0;
////
////	ImGui::Text("ROT");
////	ImGui::SameLine();
////	vl_gui_rot("##rot", &self->rot[0][0]);
////	vl_rnorm(&self->rot[0][0]);
////
////	ImGui::Dummy(ImVec2(0, 5));
////	ImGui::Text("SOR");
////	ImGui::SameLine();
////	vl_gui_mat("##sor", &self->rot[1][0], 0.001, &min, &max, "%.3f");
//
////	ImGui::Dummy(ImVec2(0, 5));
////	ImGui::Separator();
////	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::PopID();
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline t_u8 gui_obj_view(s_gui_obj *gui, s_trcobj *self)
{
//	vl3d_init(&gui->traj_vl3d_eng, (s_vl3d_attr) {
//			.obj_sz = sizeof(gui->traj_vl3d_objlist) / sizeof(s_vl3d_obj),
//			.obj_ls = gui->traj_vl3d_objlist
//	});
//
//	for (int i = 0; i < self->traj_sz; ++i)
//	{
//		t_f64 p0[3];
//		t_f64 p1[3];
//
//		s_trctraj traj = self->traj_ls[i];
//		s_trctraj_info traj_info;
//
//		traj.info(traj.data, &traj_info);
//
//		t_f64 time = traj_info.preview_time[0];
//		t_f64 time_step = (traj_info.preview_time[1] - traj_info.preview_time[0]) / 1000.0;
//
//		for (int t = 0; t < 1000-1; ++t)
//		{
//			traj.pos(traj.data, time, p0);
//			traj.pos(traj.data, time+time_step, p1);
//
//			vl3d_add_line(&gui->traj_vl3d_eng, (s_vl3d_line) {
//					.color = vl3d_col_legacy,
//					.p0 = { p0[0], p0[1], p0[2] },
//					.p1 = { p1[0], p1[1], p1[2] }
//			});
//
//			time = time + time_step;
//		}
//	}
//
//	s_vl3d_view view = {
//			.scale = 1.0,
//			.pos = { 0.0, 0.0, 0.0 },
//	};
//
//	vl3d_view_load(self, &view, view);
//
//	s_vl3d_xyz 		xyz  = vl3d_xyz_init(&view);
//	s_vl3d_gridline grid = vl3d_gridline_init(&view);
//
//	vl3d_begin(&gui->traj_vl3d_eng);
//	vl3d_view_ctrl3d(&gui->traj_vl3d_eng, &view);
//	vl3d_xyz_draw(&gui->traj_vl3d_eng, &xyz);
//	vl3d_gridline_draw(&gui->traj_vl3d_eng, &grid);
//	vl3d_view_draw(&gui->traj_vl3d_eng, &view);
//	vl3d_end();
//	vl3d_view_save(self, &view);
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __GUI_OBJ__ */
