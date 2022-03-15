
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUITRAJ_BZ2__
#define __GUITRAJ_BZ2__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

//typedef struct view_data
//{
//	ImVec2 	  p0;
//	ImVec2 	  p1;
//
//	float32_t width;
//	float32_t height;
//
//	ImRect 	  rect;
//
//	bool 	  int_drag;
//	ImVec2    int_drag_d;
//
//} 	s_view_data;
//
////------------------------------------------------------------------------------
//
//enum st
//{
//	st_view_p0_x = 100, // range point 0
//	st_view_p0_y,
//	st_view_p1_x,
//	st_view_p1_y,
//	st_int_drag,
//	st_int_drag_dx,
//	st_int_drag_dy,
//};
//
////------------------------------------------------------------------------------
//
//inline void gui_traj_edit_bz2(s_trctraj *self)
//{
//	ImGui::PushID(self);
//
//	s_trctraj_bz *traj = (s_trctraj_bz*) self->data;
//
//	// !!! UPDATE HASHES !!!
//	// if ref name was changed we must recalc hash
//	// to retain save/load and gui objsel functionality
//	if (traj->ref  != NULL) { traj->ref_hash  = traj->ref->hash ; }
//	if (traj->ellp != NULL) { traj->ellp_hash = traj->ellp->hash; }
//
//	ImGui::Text("desc  ");
//	ImGui::SameLine();
//	ImGui::Text(self->desc);
//
////	ImGui::Text("hash  ");
////	ImGui::SameLine();
////	imgui_hash("##hash", self->hash);
////
////	ImGui::Text("eng   ");
////	ImGui::SameLine();
////	ImGui::Text("%08X", (t_u32) traj->eng);
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	gui_objsel("##ref", traj->eng->obj_sz, traj->eng->obj_ls, &traj->ref);
//	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ellp  ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(-40);
//	t_u8 ellp_en = traj->ellp_en;
//	s_trcellp *ellp_ref = traj->ellp;
//	gui_ellpsel("##ellp", traj->eng->ellp_offset, traj->eng->ellp_ls, &traj->ellp);
//	ImGui::SameLine(0.0, 0.0);
//	imgui_bool("##ellp_en", ImVec2(-1, 0), &traj->ellp_en);
//	if (traj->ellp == NULL) { traj->ellp_en = 0x00; }
//	if (traj->ellp != NULL) { traj->ellp_hash = traj->ellp->hash; }
//
//	if (ellp_en != traj->ellp_en) // support conversion when changing ellp support
//	{
//		// if we turn off ellp support then convert degrees to m
//		if (traj->ellp_en == 0x00)
//		{
//			t_f64 pos_ecef[3];
//
//			for (int i = 0; i < traj->pts_offset; ++i)
//			{
//				trcellp_ecef(ellp_ref, pos_ecef, traj->pts[i].pos_p);
//				vl3_vcopy(traj->pts[i].pos_p, pos_ecef);
//			}
//		}
//
//		// if we turn on ellp support then convert m to lla
//		if (traj->ellp_en != 0x00)
//		{
//			t_f64 pos_lla[3];
//
//			for (int i = 0; i < traj->pts_offset; ++i)
//			{
//				trcellp_lla(traj->ellp, pos_lla, traj->pts[i].pos_p);
//				vl3_vcopy(traj->pts[i].pos_p, pos_lla);
//			}
//		}
//
//		trctraj_bz_compile(traj);
//	}
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	if (traj->pts_offset == 0x00)
//	{
//		ImGui::TextWrapped("No points added. To add a point press CTRL+LM.");
//	}
//
//	for (int i = 0; i < traj->pts_offset; ++i)
//	{
//		ImGui::PushID(i);
//
//		ImGui::BeginGroup();
//
//		t_f64 time_min = 0.0;
//		ImGui::SetNextItemWidth(50);
//		ImGui::DragScalar("##time", ImGuiDataType_Double, &traj->pts[i].time, 1.0, &time_min, NULL, "%.0f");
//		ImGui::SameLine();
//
//		if (traj->ellp_en == 0x00)
//		{
//			imgui_vec("point", traj->pts[i].pos_p, 1.0, NULL, NULL, "%.3f");
//		}
//
//		else
//		{
//			t_f64 lla_deg[3] = {vld_deg(traj->pts[i].pos_p[0]), vld_deg(traj->pts[i].pos_p[1]), traj->pts[i].pos_p[2]};
//			imgui_vec("##point", lla_deg, 0.001, NULL, NULL, "%.3f");
//
//			traj->pts[i].pos_p[0] = vld_rad(lla_deg[0]);
//			traj->pts[i].pos_p[1] = vld_rad(lla_deg[1]);
//			traj->pts[i].pos_p[2] = lla_deg[2];
//		}
//
//		ImGui::EndGroup();
//
//		if (ImGui::BeginPopupContextItem("edit"))
//		{
//			if (ImGui::Selectable("delete")) {
//				trctraj_bz_rem(traj, i);
//				trctraj_bz_compile(traj);
//			}
//
//			ImGui::EndPopup();
//		}
//
//		ImGui::PopID();
//	}
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::PopID();
//
//	return;
//}
//
////------------------------------------------------------------------------------
//
//inline ImVec2 __gui_traj_view_bz_tf__(s_view_data *view, ImVec2 pos)
//{
//	float32_t x = view->rect.GetWidth()  * (pos.x - view->p0.x) / (view->width);
//	float32_t y = view->rect.GetHeight() - view->rect.GetHeight() * (pos.y - view->p0.y) / (view->height);
//
//	return ImVec2(x + view->rect.Min.x, y + view->rect.Min.y);
//}
//
//inline ImVec2 __gui_traj_view_bz_inv__(s_view_data *view, ImVec2 pos)
//{
//	ImVec2 temp = ImVec2(pos.x - view->rect.Min.x, pos.y - view->rect.Min.y);
//
//	float32_t x = temp.x * view->width / view->rect.GetWidth()  + view->p0.x;
//	float32_t y = (- temp.y + view->rect.GetHeight()) * view->height / view->rect.GetHeight() + view->p0.y;
//
//	return ImVec2(x, y);
//}
//
//inline void gui_traj_view_bz2(s_trctraj *traj)
//{
//	s_trctraj_bz *self = (s_trctraj_bz*) traj->data;
//
//	t_u32 del_index = 0x00;
//	t_u8 del_req = 0x00;
//
//	s_view_data view_top;
//	s_view_data view_bot;
//
//	ImGuiContext& context = *ImGui::GetCurrentContext();
//	ImGuiStyle& style = ImGui::GetStyle();
//	ImGuiIO& io = ImGui::GetIO();
//
//	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
//	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
//
//	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
//
//	return;
//}
//
//------------------------------------------------------------------------------

#endif /* __GUITRAJ_BZ2__ */
