
#include "trj_gui_tbar.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_tbar_main(s_trj_gui_tbar *gui)
{
	static float64_t time_limit_min = 0.0;
	static float64_t time_step_min = 0.001;
	static float64_t time_step_max = 100.0;
	static uint32_t  time_iter_min = 0x00;
	
	ImGui::PushID("##tbar");
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
	
	ImGui::Text("TSI"); ImGui::SameLine();

//	ImGui::Text("time_limit"); ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::DragScalar("##time_limit", ImGuiDataType_Double, &gui->eng_gui->time_limit, 1.0, &time_limit_min, NULL, "T:%.0f");
	ImGui::SameLine();
	
//	ImGui::Text("time_step"); ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::DragScalar("##time_step", ImGuiDataType_Double, &gui->eng_gui->time_step, 0.001, &time_step_min, &time_step_max, "S:%.3f");
	ImGui::SameLine();
	
	gui->eng_gui->time_iter = gui->eng_gui->time_limit / gui->eng_gui->time_step;
	
//	ImGui::Text("time_step"); ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::DragScalar("##time_iter", ImGuiDataType_U32, &gui->eng_gui->time_iter, 1.0, &time_iter_min, NULL, "I:%d");
	ImGui::SameLine();
	
//	*self->time_limit = *self->time_iter * *self->time_step;
	
	if(ImGui::Button("RUN"))
	{
		gui->eng_gui->state = trj_gui_eng_state_init;
	}

	ImGui::PopStyleVar();
	ImGui::PopID();
	
	return 0x00;
}

//------------------------------------------------------------------------------