
#ifndef __GUI_DATA__
#define __GUI_DATA__

//----------------------------------------------------------------

#include <libcommon/vl.h>
#include <libcommon/vl3d.h>

#include <libgui/imgui/imgui.h>
#include <libgui/imgui/imgui_internal.h>
#include <libgui/implot/implot.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_data_.h>

//----------------------------------------------------------------

inline void gui_data_edit(s_trj_data *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::InputText("##name", self->name, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_data_edit_text(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_text *data = (s_trj_data_text*) self->data;
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
	
	ImGui::Text("file  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::InputText("##file", data->file_name, 256);
	
	ImGui::Text("size  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::Text("%.1f kb", (float) data->file_size / 1024);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

inline void gui_data_view_text(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_text *data = (s_trj_data_text*) self->data;
	
	if (data->file_data == NULL || data->file_data == 0x00)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
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

//----------------------------------------------------------------

inline void gui_data_edit_ram(s_trj_data *self)
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
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) data->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", data->eng->obj_count, data->eng->obj_list, &data->ref);
	if (data->ref != NULL) { data->ref_hash = data->ref->hash; }
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ellp  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-40);
	gui_ellpsel("##ellp", data->eng->ellp_offset, data->eng->ellp_list, &data->ellp);
	ImGui::SameLine(0.0, 0.0);
	imgui_bool("##ellp_en", ImVec2(-1, 0), &data->ellp_en);
	if (data->ellp == NULL) { data->ellp_en = 0x00; }
	if (data->ellp != NULL) { data->ellp_hash = data->ellp->hash; }
	
	ImGui::PopID();
	
	return;
}

inline uint8_t gui_data_view_ram(s_trj_data *self)
{
	s_trj_data_ram *data = (s_trj_data_ram*) self->data;
	uint8_t mode = 0x00;
	
	// DO NOT MOVE DOWN because return will break IMGUI ID stack without ImGui::PopID();
	if (data->offset == 0x00)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return mode;
	}
	
	ImGui::PushID(self);
	
	if (ImGui::BeginTabBar("##data_view", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("3D view"))
		{
			mode = 0x00;
			
			static s_vl3d vl3d;
			s_vl3d_obj *obj_list = (s_vl3d_obj *) malloc(sizeof(s_vl3d_obj) * (data->offset*2 + 2*4096));
			
			s_vl3d_view view = vl3d_view_init();
			vl3d_view_load(&vl3d, &view, view);
			
			vl3d_init(&vl3d, (s_vl3d_attr) {
				.obj_sz = data->offset*2 + 2*4096,
				.obj_ls = obj_list
			});
			
			s_vl3d_line line = {.color = vl3d_col_legacy};
			
			// pos
			if (data->offset > 10000)
			{
				for (int di = data->offset / 10000, i = 0;
					 i < 10000 - 1; ++i)
				{
					vl3_vcopy(line.p0, &data->ecef_pos[(i * di)*3]);
					vl3_vcopy(line.p1, &data->ecef_pos[((i + 1) * di)*3]);
					
					vl3d_add_line(&vl3d, line);
				}
			} else
			{
				for (int i = 0; i < data->offset - 1; ++i)
				{
					vl3_vcopy(line.p0, &data->ecef_pos[i*3]);
					vl3_vcopy(line.p1, &data->ecef_pos[(i + 1)*3]);
					
					vl3d_add_line(&vl3d, line);
				}
			}
			
			s_vl3d_trngl trngl = { .flags = vl3d_obj_flags_spec, .color = vl3d_col_legacy, };
			
			//		vl_vcopy(trngl.p0, &data->data_list[0].pos[0][0]);
			//		vl_vcopy(trngl.p1, &data->data_list[*data->data_offset / 2].pos[0][0]);
			//		vl_vcopy(trngl.p2, &data->data_list[*data->data_offset - 1].pos[0][0]);
			//		vl3d_add_trngl(&vl3d_eng, trngl);
			
			// hpr
			if (data->offset > 20)
			{
				for (int di = data->offset / 20, i = 0;
					 i < 20 - 1; ++i)
				{
					vl3_vcopy(trngl.p0, &data->ecef_pos[i * di * 3]);
					vl3_vcopy(trngl.p1, &data->ecef_pos[i * di * 3]);
					vl3_vcopy(trngl.p2, &data->ecef_pos[i * di * 3]);
					
					vlf_t rot[9];
					vl3_tnp(rot, &data->ecef_rot[i * di * 9]);
					vl3_mmul_s(rot, rot, 0.015 / view.scale);
					
					// top middle
					
					vl3_vmul_s(&rot[0], &rot[0], 3.0);
					vl3_vsum(trngl.p0, trngl.p0, &rot[0]);
					
					// left
					vl3_vsub(trngl.p1, trngl.p1, &rot[6]);
					
					// right
					vl3_vsum(trngl.p2, trngl.p2, &rot[6]);
					
					vl3d_add_trngl(&vl3d, trngl);
				}
			} else
			{
				for (int i = 0; i < data->offset - 1; ++i)
				{
					vl3_vcopy(trngl.p0, &data->ecef_pos[i*3]);
					vl3_vcopy(trngl.p1, &data->ecef_pos[i*3]);
					vl3_vcopy(trngl.p2, &data->ecef_pos[i*3]);
					
					vlf_t rot[9];
					vl3_tnp(rot, &data->ecef_rot[i*9]);
					vl3_mmul_s(rot, rot, 10.0 / view.scale);
					
					// top middle
					vl3_vsum(trngl.p0, trngl.p0, &rot[0]);
					
					// left
					vl3_vsub(trngl.p1, trngl.p1, &rot[6]);
					
					// right
					vl3_vsum(trngl.p2, trngl.p2, &rot[6]);
					
					vl3d_add_trngl(&vl3d, trngl);
				}
			}
			
			gui_vl3d(&vl3d);
			
			free(obj_list);
			
			ImGui::EndTabItem();
		}
		
		int offset = 0x00;
		int stride = 10*sizeof(vlf_t);
		
		if (ImGui::BeginTabItem("details"))
		{
			mode = 0x01;
			
			if (ImGui::CollapsingHeader("heading"))
			{
				if (ImPlot::BeginPlot("heading"))
				{
					ImPlot::PlotLine("heading", data->time, data->heading, data->offset / stride, offset, stride*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (ImGui::CollapsingHeader("pitch"))
			{
				if (ImPlot::BeginPlot("pitch"))
				{
					ImPlot::PlotLine("pitch", data->time, data->pitch, data->offset / stride, offset, stride*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (ImGui::CollapsingHeader("roll"))
			{
				if (ImPlot::BeginPlot("roll"))
				{
					ImPlot::PlotLine("roll", data->time, data->roll, data->offset / stride, offset, stride*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (ImGui::CollapsingHeader("tied_acc"))
			{
				if (ImPlot::BeginPlot("tied_acc"))
				{
					ImPlot::PlotLine("acc_x", data->time3, &data->tied_acc[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("acc_y", data->time3, &data->tied_acc[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("acc_z", data->time3, &data->tied_acc[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (ImGui::CollapsingHeader("tied_grs"))
			{
				if (ImPlot::BeginPlot("tied_grs"))
				{
					ImPlot::PlotLine("grs_x", data->time3, &data->tied_grs[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("grs_y", data->time3, &data->tied_grs[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("grs_z", data->time3, &data->tied_grs[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (data->ellp != NULL && data->ellp_en)
			{
				if (ImGui::CollapsingHeader("lla_pos"))
				{
					if (ImPlot::BeginPlot("lla_pos lat,lon"))
					{
						ImPlot::PlotLine("lla_pos_lat", data->time3, &data->lla_pos[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
						ImPlot::PlotLine("lla_pos_lon", data->time3, &data->lla_pos[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
						ImPlot::EndPlot();
					}
					
					if (ImPlot::BeginPlot("lla_pos alt"))
					{
						ImPlot::PlotLine("lla_pos_alt", data->time3, &data->lla_pos[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
						ImPlot::EndPlot();
					}
				}
				
				if (ImGui::CollapsingHeader("lla_vel"))
				{
					if (ImPlot::BeginPlot("lla_vel"))
					{
						ImPlot::PlotLine("vel_lat", data->time3, &data->lla_vel[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
						ImPlot::PlotLine("vel_lon", data->time3, &data->lla_vel[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
						ImPlot::PlotLine("vel_alt", data->time3, &data->lla_vel[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
						ImPlot::EndPlot();
					}
				}
			}
			
			
			if (ImGui::CollapsingHeader("ecef_pos"))
			{
				if (ImPlot::BeginPlot("ecef_pos"))
				{
					ImPlot::PlotLine("ecef_pos_x", data->time3, &data->ecef_pos[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("ecef_pos_y", data->time3, &data->ecef_pos[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("ecef_pos_z", data->time3, &data->ecef_pos[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (ImGui::CollapsingHeader("ecef_vel"))
			{
				if (ImPlot::BeginPlot("ecef_vel"))
				{
					ImPlot::PlotLine("ecef_vel_x", data->time3, &data->ecef_vel[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("ecef_vel_y", data->time3, &data->ecef_vel[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("ecef_vel_z", data->time3, &data->ecef_vel[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			if (ImGui::CollapsingHeader("ecef_acc"))
			{
				if (ImPlot::BeginPlot("ecef_acc"))
				{
					ImPlot::PlotLine("ecef_acc_x", data->time3, &data->ecef_acc[0], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("ecef_acc_y", data->time3, &data->ecef_acc[1], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::PlotLine("ecef_acc_z", data->time3, &data->ecef_acc[2], data->offset / stride, offset, stride * 3*sizeof(vlf_t));
					ImPlot::EndPlot();
				}
			}
			
			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
	}
	
	ImGui::PopID();
	
	// for using in data_mat
	return mode;
}

//----------------------------------------------------------------

inline void gui_data_edit_ramld(s_trj_data *self)
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
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) data->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", data->eng->obj_count, data->eng->obj_list, &data->ref);
	if (data->ref != NULL) { data->ref_hash = data->ref->hash; }
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ellp  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-40);
	gui_ellpsel("##ellp", data->eng->ellp_offset, data->eng->ellp_list, &data->ellp);
	ImGui::SameLine(0.0, 0.0);
	imgui_bool("##ellp_en", ImVec2(-1, 0), &data->ellp_en);
	if (data->ellp == NULL) { data->ellp_en = 0x00; }
	if (data->ellp != NULL) { data->ellp_hash = data->ellp->hash; }
	
	ImGui::PopID();
	
	return;
}

inline void gui_data_view_ramld(s_trj_data *self)
{
	s_trj_data_ramld *data = (s_trj_data_ramld*) self->data;
	
	// DO NOT MOVE DOWN because return will break IMGUI ID stack without ImGui::PopID();
	if (data->offset == 0x00)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
	ImGui::PushID(self);
	
	if (ImGui::CollapsingHeader("lateral deviation"))
	{
		if (ImPlot::BeginPlot("lateral deviation"))
		{
			ImPlot::PlotLine("lateral deviation", data->time, data->ld, data->offset / 10, 0, 10*sizeof(vlf_t));
			ImPlot::EndPlot();
		}
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_data_edit_mat(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_mat *data = (s_trj_data_mat*) self->data;
	
	s_trj_data ram = *self;
	ram.data = &data->ram;
	
	s_trj_data ramld = *self;
	ramld.data = &data->ramld;
	
	gui_data_edit_ram(&ram);
	// no need to edit ramld coz theris only ellp which we use from mat plugin
//	gui_data_edit_ramld(&ramld);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("file  ");
	if (ImGui::IsItemHovered())
	{ ImGui::SetTooltip("Path to almanac file"); }
	ImGui::SameLine();
	gui_filesave(data->file_name, 0x00, NULL);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("hpr   ");
	ImGui::SameLine();
	imgui_bool("##hpr", ImVec2(ImGui::GetContentRegionAvailWidth(),0), &data->hpr_en);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("lla   ");
	ImGui::SameLine();
	imgui_bool("##lla", ImVec2(ImGui::GetContentRegionAvailWidth(),0), &data->lla_en);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ecef  ");
	ImGui::SameLine();
	imgui_bool("##ecef", ImVec2(ImGui::GetContentRegionAvailWidth(),0), &data->ecef_en);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("tied  ");
	ImGui::SameLine();
	imgui_bool("##tied", ImVec2(ImGui::GetContentRegionAvailWidth(),0), &data->tied_en);

    ImGui::AlignTextToFramePadding();
    ImGui::Text("abs   ");
    ImGui::SameLine();
    imgui_bool("##abs", ImVec2(ImGui::GetContentRegionAvailWidth(),0), &data->abs_en);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("latdev");
	ImGui::SameLine();
	imgui_bool("##latdev", ImVec2(ImGui::GetContentRegionAvailWidth(),0), &data->ld_en);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	// copy data from ram which user can edit
	// to ramld which user cant edit but we assume it uses same parameters
	
	data->ramld.ellp_en = data->ram.ellp_en;
	data->ramld.ellp = data->ram.ellp;
	data->ramld.ref = data->ram.ref;
	data->ramld.ellp_hash = data->ram.ellp_hash;
	data->ramld.ref_hash = data->ram.ref_hash;
	
	return;
}

inline void gui_data_view_mat(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_mat *data = (s_trj_data_mat*) self->data;
	
	s_trj_data ram = *self;
	ram.data = &data->ram;
	
	s_trj_data ramld = *self;
	ramld.data = &data->ramld;
	
	if (gui_data_view_ram(&ram) != 0x00)
	{
		gui_data_view_ramld(&ramld);
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------


inline void gui_data_edit_bin(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_bin *data = (s_trj_data_bin*) self->data;
	
	s_trj_data ram = *self;
	ram.data = &data->ram;
	
	gui_data_edit_ram(&ram);
	// no need to edit ramld coz theris only ellp which we use from mat plugin
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("file  ");
	if (ImGui::IsItemHovered())
	{ ImGui::SetTooltip("Path to output file"); }
	ImGui::SameLine();
	gui_filesave(data->file_name, 0x00, NULL);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	// copy data from ram which user can edit
	// to ramld which user cant edit but we assume it uses same parameters
	
	return;
}

inline void gui_data_view_bin(s_trj_data *self)
{
	ImGui::PushID(self);
	
	s_trj_data_bin *data = (s_trj_data_bin*) self->data;
	
	s_trj_data ram = *self;
	ram.data = &data->ram;
	
	if (gui_data_view_ram(&ram) != 0x00)
	{
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

#endif /* __GUI_DATA__ */
