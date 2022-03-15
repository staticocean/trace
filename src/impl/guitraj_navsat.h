
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUITRAJ_BZ2__
#define __GUITRAJ_BZ2__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

////------------------------------------------------------------------------------
//
//inline void gui_traj_edit_navsat(s_trctraj *self)
//{
//	s_trctraj_navsat *traj = (s_trctraj_navsat*) self->data;
//
//	ImGui::PushID(self);
//
//	// !!! UPDATE HASHES !!!
//	// if ref name was changed we must recalc hash
//	// to retain save/load and gui objsel functionality
//	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
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
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	gui_objsel("##ref", traj->eng->obj_sz, traj->eng->obj_ls, &traj->ref);
//	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("file  ");
//	if (ImGui::IsItemHovered())
//	{ ImGui::SetTooltip("Path to almanac file"); }
//	ImGui::SameLine();
//	gui_fileopen(traj->file_path, 0x00, NULL);
//
//	ImGui::Text("type  ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	int file_type = traj->file_type;
//	char *file_types[] = {
//			"none",
//			".AGP [glonass-iac.ru]",
//			".AGL [glonass-iac.ru]",
//	};
//
//	ImGui::Combo("##file_type", &file_type, file_types, IM_ARRAYSIZE(file_types), 10);
//	traj->file_type = (trctraj_navsat_filetype_t) file_type;
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("resol ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	ImGui::SliderInt("##resolution", &traj->resolution, 1, 1000);
//
//	if (traj->data_sz > 0x00)
//	{
//		ImGui::AlignTextToFramePadding();
//		ImGui::Text("satnum");
//		ImGui::SameLine();
//
//		char satnum[32];
//		sprintf(satnum, "SAT %d", traj->data_ls[traj->sat_offset].satnum);
//
//		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//		if (ImGui::BeginCombo("##satnum", satnum, ImGuiComboFlags_None))
//		{
//			for (int i = 0; i < traj->data_sz; ++i)
//			{
//				const bool is_selected = (i == traj->sat_offset);
//				sprintf(satnum, "SAT %d", traj->data_ls[i].satnum);
//				if (ImGui::Selectable(satnum, is_selected))
//				{ traj->sat_offset = i; }
//
//				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//				if (is_selected) ImGui::SetItemDefaultFocus();
//			}
//			ImGui::EndCombo();
//		}
//	}
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("time  ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	imgui_datetime("##datetime",
//		 &traj->day, &traj->month, &traj->year,
//		 &traj->hour, &traj->min, &traj->sec);
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
//inline void gui_traj_view_navsat(s_trctraj *self)
//{
//	s_trctraj_navsat *traj = (s_trctraj_navsat*) self->data;
//
//	ImGui::PushID(self);
//
//	if (ImGui::BeginTabBar("##data_view", ImGuiTabBarFlags_None))
//	{
//		if (ImGui::BeginTabItem("3D view"))
//		{
//			static s_vl3d vl3d;
//			static s_vl3d_obj obj_ls[4096*4];
//
//			vl3d_init(&vl3d, (s_vl3d_attr) {
//				.obj_sz = sizeof(obj_ls) / sizeof(s_vl3d_obj),
//				.obj_ls = obj_ls,
//			});
//
//			for (int i = 0; i < traj->data_sz; ++i)
//			{
//				t_f64 time = 0.0;
//				t_f64 time_limit = 3600*12;
//				t_f64 time_step = time_limit / 64;
//				int time_iter = time_limit / time_step;
//
//				t_f64 p0[3];
//				t_f64 p1[3];
//				t_f64 rot[9];
//
//				trctraj_navsat_pos_local(traj, time, p0, i);
//				trctraj_navsat_pos_local(traj, time, p1, i);
//
//				for (int j = 0; j < time_iter; ++j)
//				{
//					time += time_step;
//
//					vl3_vcopy(p0, p1);
//					trctraj_navsat_pos_local(traj, time, p1, i);
//
//					if (i == traj->sat_offset)
//					{
//						vl3d_add_line(&vl3d, (s_vl3d_line) {
//								.color = vl3d_col_legacy,
//								.p0 = { p0[0], p0[1], p0[2] },
//								.p1 = { p1[0], p1[1], p1[2] },
//						});
//					}
//
//					else
//					{
//						vl3d_add_line(&vl3d, (s_vl3d_line) {
//								.color = vl3d_col_d,
//								.p0 = { p0[0], p0[1], p0[2] },
//								.p1 = { p1[0], p1[1], p1[2] },
//						});
//					}
//				}
//			}
//
//			gui_vl3d(&vl3d);
//
//			ImGui::EndTabItem();
//		}
//
//		if (ImGui::BeginTabItem("Almanac"))
//		{
//			if (ImGui::Button("load"))
//			{ trctraj_navsat_compile(traj); }
//
//			ImGui::Dummy(ImVec2(0, 5));
//			ImGui::Separator();
//			ImGui::Dummy(ImVec2(0, 5));
//
//			if (ImGui::BeginTable("table1", 17, ImGuiTableFlags_None))
//			{
//				ImGui::TableSetupColumn("satnum", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("health", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("week  ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("tow   ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("day   ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("month ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("year  ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("atime ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("tcorr ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("dtcorr", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("domg  ", ImGuiTableColumnFlags_WidthFixed);
//
//				ImGui::TableSetupColumn("omg0  ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("i     ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("w     ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("e     ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("sqrta ", ImGuiTableColumnFlags_WidthFixed);
//				ImGui::TableSetupColumn("m0    ", ImGuiTableColumnFlags_WidthFixed);
//
//				ImGui::TableHeadersRow();
//
//				for (int row = 0; row < traj->data_sz; row++)
//				{
//					ImGui::TableNextRow();
//
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].satnum);
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].health);
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].week);
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].tow);
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].day);
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].month);
//					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_ls[row].year);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].atime);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].tcorr);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].dtcorr);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].domg);
//
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].omg0);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].i);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].w);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].e);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].sqrta);
//					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_ls[row].m0);
//				}
//				ImGui::EndTable();
//			}
//
//			ImGui::EndTabItem();
//		}
//
//		ImGui::EndTabBar();
//	}
//
//	ImGui::PopID();
//}

//------------------------------------------------------------------------------

#endif /* __GUITRAJ_BZ2__ */
