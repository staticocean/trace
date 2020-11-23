
#ifndef __TRJ_GUI_DATA__
#define __TRJ_GUI_DATA__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

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
	ImGui::InputText("##name", self->desc, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Columns(2);
	
	ImGui::Text("id");
	ImGui::NextColumn();
	ImGui::Text("[%06d]", self->id);
	ImGui::NextColumn();
//
//	ImGui::Text("pos_inert");
//	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[kg]"); }
//	ImGui::NextColumn();
//	ImGui::SetNextItemWidth(-1);
//	ImGui::DragScalar("##pos_inert", ImGuiDataType_Double, &self->pos_inert, 0.1, NULL, NULL, "%.3f");
//	ImGui::NextColumn();
	
	ImGui::Columns(1);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_data_text_edit(s_trj_data *self)
{
	return;
}

inline void trj_gui_data_text_view(s_trj_data *self)
{
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_data_ram_edit(s_trj_data *self)
{
	return;
}

inline void trj_gui_data_ram_view(s_trj_data *self)
{
	s_trj_data_ram *data = (s_trj_data_ram*) self->data;
	
	if (data->data_list == NULL || data->data_offset == NULL)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
	s_vl3d_eng vl3d_eng;
	s_vl3d_obj *obj_list = (s_vl3d_obj*) malloc(sizeof(s_vl3d_obj) * (*data->data_offset + 2048));
	s_vl3d_view view = { .scale = 1.0, .pos = { 0.0, 0.0, 0.0 }, .tbar_en = 0x01, .grid_mode = 0x01, .grid_pt_disp = 2.0, .grid_pt_size = 2.0 };
	
	vl3d_view_load(self, &view, view);
	
	vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) {
			.obj_list = obj_list,
	});
	
	s_vl3d_line line = { .color = vl3d_col_l };
	
	if (*data->data_offset > 10000)
	{
		for (int di = *data->data_offset / 10000, i = 0;
			 i < 10000-1; ++i)
		{
			vl_vcopy(line.p0, &data->data_list[i*di].pos[0][0]);
			vl_vcopy(line.p1, &data->data_list[(i+1)*di].pos[0][0]);

			vl3d_eng_add_line(&vl3d_eng, line);
		}
	} else
	{
		for (int i = 0; i < *data->data_offset-1; ++i)
		{
			vl_vcopy(line.p0, &data->data_list[i].pos[0][0]);
			vl_vcopy(line.p1, &data->data_list[i+1].pos[0][0]);

			vl3d_eng_add_line(&vl3d_eng, line);
		}
	}
//
//	for (int i = 0; i < *data->data_offset-1; ++i)
//	{
//		vl_vcopy(line.p0, &data->data_list[i].pos[0][0]);
//		vl_vcopy(line.p1, &data->data_list[i+1].pos[0][0]);
//
//		vl3d_eng_add_line(&vl3d_eng, line);
//	}
	
	vl3d_view_grid(&view, &vl3d_eng);
	vl3d_eng_render(&vl3d_eng, &view, "temp", ImVec2(-1, -1));
	vl3d_view_save(self, &view);
	
	free(obj_list);
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_DATA__ */
