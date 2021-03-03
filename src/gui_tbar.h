
#ifndef __GUI_TBAR__
#define __GUI_TBAR__

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <softael_lib/vl.h>

#include <lib/imgui/imgui.h>
#include <lib/trj/trj_eng.h>

#include "gui_eng.h"

//----------------------------------------------------------------

typedef struct gui_tbar
{
	int height;
	
	s_trj_eng *eng;
	s_gui_eng *eng_gui;
	
	char file_path[512];
	
} 	s_gui_tbar;

//----------------------------------------------------------------

inline uint8_t gui_tbar_main(s_gui_tbar *gui)
{
	static float64_t time_limit_min = 0.0;
	static float64_t time_step_min = 0.001;
	static float64_t time_step_max = 100.0;
	static uint32_t  time_iter_min = 0x00;
	
//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("TSI"); ImGui::SameLine();

//	ImGui::Text("time_limit"); ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::DragScalar("##time_limit", ImGuiDataType_Double, &gui->eng->time_limit, 1.0, &time_limit_min, NULL, "T:%.0f");
	ImGui::SameLine();

//	ImGui::Text("time_step"); ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::DragScalar("##time_step", ImGuiDataType_Double, &gui->eng->time_step, 0.001, &time_step_min, &time_step_max, "S:%.3f");
	ImGui::SameLine();
	
	gui->eng->time_iter = gui->eng->time_limit / gui->eng->time_step;

//	ImGui::Text("time_step"); ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	ImGui::DragScalar("##time_iter", ImGuiDataType_U32, &gui->eng->time_iter, 1.0, &time_iter_min, NULL, "I:%d");
	ImGui::SameLine();

//	*self->time_limit = *self->time_iter * *self->time_step;
	
	ImGui::SetNextItemWidth(200);
	gui_procsel("##proc", gui->eng);
	ImGui::SameLine();
	
	if(ImGui::Button("RENDER"))
	{ gui->eng_gui->state = gui_eng_state_init; }

//	ImGui::SameLine();
//	ImGui::Dummy(ImVec2(-1, -1));

//	ImGui::PopStyleVar();

//	ImGui::Separator();

	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	
	{
		ImGui::PushID(gui->file_path);
		
		ImGui::SetNextItemWidth(160);
		char *file_name = gui->file_path + strlen(gui->file_path);
		while (file_name > gui->file_path && *(file_name-1) != '\\' && *(file_name-1) != '/')
		{ --file_name; }
		
		ImGui::InputText("##file_path", file_name, 512, ImGuiInputTextFlags_ReadOnly);
		if (ImGui::IsItemHovered())
		{ ImGui::SetTooltip(gui->file_path); }
		
		ImGui::SameLine(0.0, 0.0);
		
		if(ImGui::Button("SEL", ImVec2(40,0)))
		{ __file_browser_open__.Open(); }
		
		__file_browser_open__.Display();
		
		if(__file_browser_open__.HasSelected())
		{
			strcpy(gui->file_path, __file_browser_open__.GetSelected().string().c_str());
			__file_browser_open__.ClearSelected();
			trj_eng_load(gui->eng, gui->file_path);
		}
		
		ImGui::PopID();
	}
	{
		ImGui::PushID(gui->file_path);
		
		ImGui::SameLine(0.0, 0.0);
		
		if(ImGui::Button("SAVE", ImVec2(40,0)))
		{ trj_eng_save(gui->eng, gui->file_path); }
		
		ImGui::SameLine(0.0, 0.0);
		
		if(ImGui::Button("SAVE AS", ImVec2(60,0)))
		{ __file_browser_save__.Open(); }
		
		__file_browser_save__.Display();
		
		if(__file_browser_save__.HasSelected())
		{
			strcpy(gui->file_path, __file_browser_save__.GetSelected().string().c_str());
			__file_browser_save__.ClearSelected();
			trj_eng_save(gui->eng, gui->file_path);
		}
		
		ImGui::PopID();
	}
//			trj_eng_load(self->eng, (char*) __file_browser_open__.GetSelected().string().c_str());

	return 0x00;
}

//----------------------------------------------------------------

#endif /* __GUI_TBAR__ */




