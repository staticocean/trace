
#ifndef __TRJ_GUI_DATA__
#define __TRJ_GUI_DATA__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

#include <lib/implot/implot.h>

#include <lib/trj/vl.h>
#include <lib/trj/vl3d.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_data_.h>

//------------------------------------------------------------------------------

inline void trj_gui_data_edit(s_trj_data *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##name", self->name, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_data_edit_text(s_trj_data *self)
{
	s_trj_data_text *data = (s_trj_data_text*) self->data;
	
	ImGui::PushID(self);
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
	ImGui::Text("hash  ");
	ImGui::SameLine();
	vl_gui_hash("##hash", self->hash);
	
	ImGui::Text("file  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##file", data->file_name, 256);
	
	ImGui::Text("size  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	ImGui::Text("%.1f kb", (float) data->file_size / 1024);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

inline void trj_gui_data_view_text(s_trj_data *self)
{
	s_trj_data_text *data = (s_trj_data_text*) self->data;
	
	if (data->file_data == NULL || data->file_data == 0x00)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
	ImGui::PushID(self);
	
	// Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGuiListClipper clipper;
	
	clipper.Begin(data->file_lcnt);
	
	while (clipper.Step())
	{
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			ImGui::Text("[%6d] %s", i, &data->file_data[i*256]);
		}
	}
	
	ImGui::PopStyleVar();
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_data_edit_ram(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_ram *data = (s_trj_data_ram*) self->data;
	
	// !!! UPDATE HASHES !!!
	// if ref name was changed we must recalc hash
	// to retain save/load and gui objsel functionality
	if (data->ref  != NULL) { data->ref_hash  = data->ref->hash ; }
	if (data->ellp != NULL) { data->ellp_hash = data->ellp->hash; }
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
	ImGui::Text("hash  ");
	ImGui::SameLine();
	vl_gui_hash("##hash", self->hash);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Text("eng   ");
	ImGui::SameLine();
	ImGui::Text("%08X", data->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	trj_gui_objsel("##ref", data->eng->obj_count, data->eng->obj_list, &data->ref);
	if (data->ref != NULL) { data->ref_hash = data->ref->hash; }
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ellp  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-40);
	trj_gui_ellpsel("##ellp", data->eng->ellp_offset, data->eng->ellp_list, &data->ellp);
	ImGui::SameLine(0.0, 0.0);
	vl_gui_bool("##ellp_en", ImVec2(-1, 0), &data->ellp_en);
	if (data->ellp == NULL) { data->ellp_en = 0x00; }
	if (data->ellp != NULL) { data->ellp_hash = data->ellp->hash; }
	
	ImGui::PopID();
	
	return;
}

inline void trj_gui_data_view_ram(s_trj_data *self)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	uint8_t mode = window->StateStorage.GetInt(ImGui::GetID("data_ram_view_mode"), 0x00);
	
	s_trj_data_ram *data = (s_trj_data_ram*) self->data;
	
	if (data->offset == 0x00)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
	ImGui::BeginGroup();
	
	if (mode == 0x00)
	{
		s_vl3d_eng vl3d_eng;
		
		s_vl3d_obj *obj_list = (s_vl3d_obj *) malloc(sizeof(s_vl3d_obj) * (data->offset*2 + 4096));
		
		s_vl3d_view view = {
				.scale = 1.0,
				.pos = {0.0, 0.0, 0.0},
				
				.tbar_en = 0x01,
				
				.grid_mode = 0x01,
				.grid_pt_size = 2.0,
				.grid_pt_disp = 2.0,
				
				.xyz_en = 0x01,
				.xyz_scale = 0.25
		};
		
		vl3d_view_load(self, &view, view);
		vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) {.obj_list = obj_list});
		
		s_vl3d_line line = {.color = vl3d_col_l};
		
		// pos
		if (data->offset > 10000)
		{
			for (int di = data->offset / 10000, i = 0;
				 i < 10000 - 1; ++i)
			{
				vl_vcopy(line.p0, &data->ecef_pos[(i * di)*3]);
				vl_vcopy(line.p1, &data->ecef_pos[((i + 1) * di)*3]);
				
				vl3d_eng_add_line(&vl3d_eng, line);
			}
		} else
		{
			for (int i = 0; i < data->offset - 1; ++i)
			{
				vl_vcopy(line.p0, &data->ecef_pos[i*3]);
				vl_vcopy(line.p1, &data->ecef_pos[(i + 1)*3]);
				
				vl3d_eng_add_line(&vl3d_eng, line);
			}
		}
		
		s_vl3d_trngl trngl = {.color = vl3d_col_l, .spec = 0x01 };
	
//		vl_vcopy(trngl.p0, &data->data_list[0].pos[0][0]);
//		vl_vcopy(trngl.p1, &data->data_list[*data->data_offset / 2].pos[0][0]);
//		vl_vcopy(trngl.p2, &data->data_list[*data->data_offset - 1].pos[0][0]);
//		vl3d_eng_add_trngl(&vl3d_eng, trngl);
		
		// hpr
		if (data->offset > 20)
		{
			for (int di = data->offset / 20, i = 0;
				 i < 20 - 1; ++i)
			{
				vl_vcopy(trngl.p0, &data->ecef_pos[i * di * 3]);
				vl_vcopy(trngl.p1, &data->ecef_pos[i * di * 3]);
				vl_vcopy(trngl.p2, &data->ecef_pos[i * di * 3]);
				
				vlf_t rot[9];
				vl_tnp(rot, &data->ctn[i * di * 9]);
				vl_mmul_s(rot, rot, 0.025 / view.scale);
				
				// top middle
				
				vl_vmul_s(&rot[0], &rot[0], 3.0);
				vl_vsum(trngl.p0, trngl.p0, &rot[0]);
				
				// left
				vl_vsub(trngl.p1, trngl.p1, &rot[6]);
				
				// right
				vl_vsum(trngl.p2, trngl.p2, &rot[6]);
				
				vl3d_eng_add_trngl(&vl3d_eng, trngl);
			}
		} else
		{
			for (int i = 0; i < data->offset - 1; ++i)
			{
				vl_vcopy(trngl.p0, &data->ecef_pos[i*3]);
				vl_vcopy(trngl.p1, &data->ecef_pos[i*3]);
				vl_vcopy(trngl.p2, &data->ecef_pos[i*3]);
				
				vlf_t rot[9];
				vl_tnp(rot, &data->ctn[i*9]);
				vl_mmul_s(rot, rot, 10.0 / view.scale);
				
				// top middle
				vl_vsum(trngl.p0, trngl.p0, &rot[0]);
				
				// left
				vl_vsub(trngl.p1, trngl.p1, &rot[6]);
				
				// right
				vl_vsum(trngl.p2, trngl.p2, &rot[6]);
				
				vl3d_eng_add_trngl(&vl3d_eng, trngl);
			}
		}
		
		vl3d_view_grid(&view, &vl3d_eng);
		vl3d_view_xyz(&view, &vl3d_eng);
		vl3d_eng_render(&vl3d_eng, &view, "temp", ImVec2(-1, -1));
		vl3d_view_save(self, &view);
		
		free(obj_list);
	}
	
	if (mode == 0x01)
	{
		if (ImPlot::BeginPlot("heading"))
		{
			// hide first and last
			ImPlot::PlotLine("heading", data->time, &data->heading[2], data->offset-3);
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("pitch"))
		{
			// hide first and last
			ImPlot::PlotLine("pitch", data->time, &data->pitch[2], data->offset-3);
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("roll"))
		{
			// hide first and last
			ImPlot::PlotLine("roll", data->time, &data->roll[2], data->offset-3);
			ImPlot::EndPlot();
		}

		if (ImPlot::BeginPlot("tied_acc"))
		{
			// hide first and last
			ImPlot::PlotLine("acc_x", &data->time3[2*3 + 0x00], &data->tied_acc[2*3 + 0], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("acc_y", &data->time3[2*3 + 0x00], &data->tied_acc[2*3 + 1], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("acc_z", &data->time3[2*3 + 0x00], &data->tied_acc[2*3 + 2], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("lla_vel"))
		{
			// hide first and last
			ImPlot::PlotLine("vel_lat", &data->time3[2*3 + 0x00], &data->lla_vel[2*3 + 0], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("vel_lon", &data->time3[2*3 + 0x00], &data->lla_vel[2*3 + 1], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("vel_alt", &data->time3[2*3 + 0x00], &data->lla_vel[2*3 + 2], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("lla_pos"))
		{
			// hide first and last
			ImPlot::PlotLine("pos_lat", &data->time3[2*3 + 0x00], &data->lla_pos[2*3 + 0], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("pos_lon", &data->time3[2*3 + 0x00], &data->lla_pos[2*3 + 1], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("pos_alt", &data->time3[2*3 + 0x00], &data->lla_pos[2*3 + 2], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			
			ImPlot::EndPlot();
		}
		
		
		if (ImPlot::BeginPlot("ecef_acc"))
		{
			// hide first and last
			ImPlot::PlotLine("acc_x", &data->time3[2*3 + 0x00], &data->ecef_acc[2*3 + 0], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("acc_y", &data->time3[2*3 + 0x00], &data->ecef_acc[2*3 + 1], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("acc_z", &data->time3[2*3 + 0x00], &data->ecef_acc[2*3 + 2], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("ecef_vel"))
		{
			// hide first and last
			ImPlot::PlotLine("vel_x", &data->time3[2*3 + 0x00], &data->ecef_vel[2*3 + 0], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("vel_y", &data->time3[2*3 + 0x00], &data->ecef_vel[2*3 + 1], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("vel_z", &data->time3[2*3 + 0x00], &data->ecef_vel[2*3 + 2], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("ecef_pos"))
		{
			// hide first and last
			ImPlot::PlotLine("pos_x", &data->time3[2*3 + 0x00], &data->ecef_pos[2*3 + 0], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("pos_y", &data->time3[2*3 + 0x00], &data->ecef_pos[2*3 + 1], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			ImPlot::PlotLine("pos_z", &data->time3[2*3 + 0x00], &data->ecef_pos[2*3 + 2], data->offset-3, 0x00, 3 * sizeof(vlf_t));
			
			ImPlot::EndPlot();
		}
	}
	
	ImGui::EndGroup();
	
	if (ImGui::BeginPopupContextItem("view"))
	{
		if (ImGui::Selectable("3D POS")) { mode = 0x00; }
		if (ImGui::Selectable("2D INS")) { mode = 0x01; }
		
		ImGui::EndPopup();
	}
	
	window->StateStorage.SetInt(ImGui::GetID("data_ram_view_mode"), mode);
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_DATA__ */
