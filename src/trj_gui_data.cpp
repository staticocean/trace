
#include "trj_gui_data.h"

//------------------------------------------------------------------------------

void trj_gui_data_edit(s_trj_data *self)
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

void trj_gui_data_text_edit(s_trj_data *self)
{
	return;
}

void trj_gui_data_text_view(s_trj_data *self)
{
	return;
}

//------------------------------------------------------------------------------

void trj_gui_data_ram_edit(s_trj_data *self)
{
	return;
}

void trj_gui_data_ram_view(s_trj_data *self)
{
	s_trj_data_ram *data = (s_trj_data_ram*) self->data;

	if (data->data_list == NULL || data->data_offset == NULL)
	{
		ImGui::Text("Object data is not available. \r\nRunning the simulation may fix the problem.");
		return;
	}
	
	s_vl3d_eng vl3d_eng;
	s_vl3d_obj *obj_list = (s_vl3d_obj*) malloc(sizeof(s_vl3d_obj) * (*data->data_offset + 128));
	s_vl3d_view view = { .scale = 1.0, .pos = { 0.0, 0.0, 0.0 } };
	
	vl3d_view_load(self, &view, view);
	
	vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) {
			.obj_list = obj_list,
	});
	
	vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { -0.25/view.scale, +0.0, +0.0 }, (float64_t[]) { +0.25/view.scale, +0.0, +0.0 } );
	vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { +0.0, -0.25/view.scale, +0.0 }, (float64_t[]) { +0.0, +0.25/view.scale, +0.0 } );
	vl3d_eng_draw_arrow(&vl3d_eng, (float64_t[]) { +0.0, +0.0, -0.25/view.scale }, (float64_t[]) { +0.0, +0.0, +0.25/view.scale } );
	
	vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { 0.25/view.scale, 0.0, 0.0 }, .data = "X" } );
	vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { 0.0, 0.25/view.scale, 0.0 }, .data = "Y" } );
	vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .p0 = { 0.0, 0.0, 0.25/view.scale }, .data = "Z" } );
	
	s_vl3d_line line;
	
	for (int i = 1; i < *data->data_offset; ++i)
	{
		vl_vcopy(line.p0, &data->data_list[i-1].pos[0][0]);
		vl_vcopy(line.p1, &data->data_list[i-0].pos[0][0]);
		
		vl3d_eng_add_line(&vl3d_eng, line);
	}
	
	vl3d_eng_render(&vl3d_eng, &view, "temp", ImVec2(-1, -1));
	
	vl3d_view_save(self, &view);
	
	free(obj_list);
	
	return;
}

//------------------------------------------------------------------------------
