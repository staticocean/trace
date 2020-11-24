
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
	ImGui::InputText("##name", self->desc, 255);
	
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
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	uint8_t mode = window->StateStorage.GetInt(ImGui::GetID("data_ram_view_mode"), 0x00);
	
	s_trj_data_ram *data = (s_trj_data_ram*) self->data;
	
	if (data->data_list == NULL || data->data_offset == NULL)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
	if (mode == 0x00)
	{
		s_vl3d_eng vl3d_eng;
		
		s_vl3d_obj *obj_list = (s_vl3d_obj *) malloc(sizeof(s_vl3d_obj) * (*data->data_offset + 4096));
		
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
		
		if (*data->data_offset > 10000)
		{
			for (int di = *data->data_offset / 10000, i = 0;
				 i < 10000 - 1; ++i)
			{
				vl_vcopy(line.p0, &data->data_list[i * di].pos[0][0]);
				vl_vcopy(line.p1, &data->data_list[(i + 1) * di].pos[0][0]);
				
				vl3d_eng_add_line(&vl3d_eng, line);
			}
		} else
		{
			for (int i = 0; i < *data->data_offset - 1; ++i)
			{
				vl_vcopy(line.p0, &data->data_list[i].pos[0][0]);
				vl_vcopy(line.p1, &data->data_list[i + 1].pos[0][0]);
				
				vl3d_eng_add_line(&vl3d_eng, line);
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
		float *pos[3] = {
				(float *) malloc(sizeof(float) * *data->data_offset),
				(float *) malloc(sizeof(float) * *data->data_offset),
				(float *) malloc(sizeof(float) * *data->data_offset),
		};
		
		float *vel[3] = {
				(float *) malloc(sizeof(float) * *data->data_offset),
				(float *) malloc(sizeof(float) * *data->data_offset),
				(float *) malloc(sizeof(float) * *data->data_offset),
		};
		
		float *acc[3] = {
				(float *) malloc(sizeof(float) * *data->data_offset),
				(float *) malloc(sizeof(float) * *data->data_offset),
				(float *) malloc(sizeof(float) * *data->data_offset),
		};
		
		float *time = (float*) malloc(sizeof(float) * *data->data_offset);
		
		for (int i = 0; i < *data->data_offset; ++i)
		{
			time[i] = data->data_list[i].time[0];
			
			for (int j = 0; j < 3; ++j)
			{
				pos[j][i] = data->data_list[i].pos[0][j];
				vel[j][i] = data->data_list[i].pos[1][j];
				acc[j][i] = data->data_list[i].pos[2][j];
			}
		}
		
//		ImGui::PushStyleVar(ImGuiStyleVar_)
		
//		ImGui::PlotLines("pos_0", pos[0], *data->data_offset);
//		ImGui::PlotLines("pos_1", pos[1], *data->data_offset);
//		ImGui::PlotLines("pos_2", pos[2], *data->data_offset);
//
//		ImGui::PlotLines("vec_0", vec[0], *data->data_offset);
//		ImGui::PlotLines("vec_1", vec[1], *data->data_offset);
//		ImGui::PlotLines("vec_2", vec[2], *data->data_offset);
//
//		ImGui::PlotLines("acc_0", acc[0], *data->data_offset);
//		ImGui::PlotLines("acc_1", acc[1], *data->data_offset);
//		ImGui::PlotLines("acc_2", acc[2], *data->data_offset);
		if (ImPlot::BeginPlot("acc"))
		{
			// hide first and last
			ImPlot::PlotLine("acc_0", time, &acc[0][1], *data->data_offset-2);
			ImPlot::PlotLine("acc_1", time, &acc[1][1], *data->data_offset-2);
			ImPlot::PlotLine("acc_2", time, &acc[2][1], *data->data_offset-2);
			
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("vel"))
		{
			// hide first and last
			ImPlot::PlotLine("vel_0", time, &vel[0][1], *data->data_offset-2);
			ImPlot::PlotLine("vel_1", time, &vel[1][1], *data->data_offset-2);
			ImPlot::PlotLine("vel_2", time, &vel[2][1], *data->data_offset-2);
			
			ImPlot::EndPlot();
		}
		
		if (ImPlot::BeginPlot("pos"))
		{
			// hide first and last
			ImPlot::PlotLine("pos_0", time, &pos[0][1], *data->data_offset-2);
			ImPlot::PlotLine("pos_1", time, &pos[1][1], *data->data_offset-2);
			ImPlot::PlotLine("pos_2", time, &pos[2][1], *data->data_offset-2);
			
			ImPlot::EndPlot();
		}
		
		for (int j = 0; j < 3; ++j)
		{
			free(pos[j]);
			free(vel[j]);
			free(acc[j]);
		}
		
		free(time);
	}
	
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
