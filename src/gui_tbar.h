
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_TBAR__
#define __GUI_TBAR__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <clip/clip.h>
#include <nfd.h>

#include <sftlstd/vl.h>
#include <sftlstd/vl3d.h>
#include <sftlstd/vl3d_imgui.h>

#include <sftlgui/sftlgui.h>

#include <sftltrc/trc.h>

#include "gui_eng.h"
#include "gui_conf.h"

//------------------------------------------------------------------------------

typedef struct gui_tbar
{
	s_trceng   *eng;
	s_gui_eng   *eng_gui;
	s_gui_conf  *conf;
	
	int height;
	
	char file_path[512];
	
} 	s_gui_tbar;

//------------------------------------------------------------------------------

inline void gui_tbar_menu_file(s_gui_tbar *tbar)
{
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		nfdchar_t *user_path;
		nfdfilteritem_t filterItem[1] = { { "Trajectory Project", "trj" } };
		nfdresult_t result = NFD_OpenDialog(&user_path, filterItem, 1, tbar->file_path);
		
		if (result == NFD_OKAY)
		{
			strcpy(tbar->file_path, user_path);
			trceng_load(tbar->eng, tbar->file_path);
			
			NFD_FreePath(user_path);
		}
	}

	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{ trceng_save(tbar->eng, tbar->file_path); }

	if (ImGui::MenuItem("Save As...", ""))
	{
		nfdchar_t *user_path;
		nfdfilteritem_t filterItem[1] = { { "Trajectory Project", "trj" } };
		nfdresult_t result = NFD_SaveDialog(&user_path, filterItem, 1, tbar->file_path, "project.trj");
		
		if (result == NFD_OKAY)
		{
			strcpy(tbar->file_path, user_path);
			trceng_save(tbar->eng, tbar->file_path);
			
			NFD_FreePath(user_path);
		}

//		else if (result == NFD_CANCEL)
//		{ puts("User pressed cancel."); }

//		else
//		{ printf("Error: %s\n", NFD_GetError() ); }
	}
	
	ImGui::EndMenu();
	
	return;
}

inline void gui_tbar_menu_newobject(s_gui_tbar *tbar)
{
//	s_trctraj *traj_static = trceng_find_traj(tbar->eng, crc32_iso_str("default_traj_static"));
//	s_trcctrl *ctrl_cpos   = trceng_find_ctrl(tbar->eng, crc32_iso_str("default_ctrl_cpos"  ));
//	s_trcctrl *ctrl_crot   = trceng_find_ctrl(tbar->eng, crc32_iso_str("default_ctrl_crot"  ));
//	s_trcdata *data_ram    = trceng_find_data(tbar->eng, crc32_iso_str("default_data_ram"   ));
//
//	s_trcobj *obj = trceng_add_obj(tbar->eng, (s_trcobj_init) { .desc = "object" });
	
//	if (obj && traj_static) { trcobj_add_traj(obj, *traj_static); }
//	if (obj && ctrl_cpos  ) { trcobj_add_ctrl(obj, *ctrl_cpos  ); }
//	if (obj && ctrl_crot  ) { trcobj_add_ctrl(obj, *ctrl_crot  ); }
//	if (obj && data_ram   ) { trcobj_add_data(obj, *data_ram   ); }
//
//	if (obj == NULL) { /* TO-DO Create error popup */ }
}

inline void gui_tbar_menu_terminal(s_gui_tbar *tbar)
{
//	if (ImGui::MenuItem(tbar->cmd->visible ? "Hide" : "Show", "CTRL+P"))
//	{ tbar->cmd->visible = !tbar->cmd->visible; }
//
//	ImGui::Separator();
//
//	if (ImGui::MenuItem("Reset", ""))
//	{ gui_cmd_clearlog(tbar->cmd); gui_env_reset(tbar->env); }
	
	ImGui::EndMenu();
}

inline void gui_tbar_menu_version(s_gui_tbar *tbar)
{
	const char *pn = "SAE-PN-RS0000-00-0000-0000";
	const char *rn = "SAE-RN-0000-0000-0000-0000";
	const char *mn = "SAE-MN-0000-0000-0000-0000";
	const char *sn = "SAE-SN-0000000000000000   ";
	
	if (ImGui::MenuItem("Product Number     ", pn, false, true)) { clip::set_text(pn); }
	if (ImGui::MenuItem("Release Number     ", rn, false, true)) { clip::set_text(rn); }
	if (ImGui::MenuItem("Manufacturer Number", mn, false, true)) { clip::set_text(mn); }
	if (ImGui::MenuItem("Serial Number      ", sn, false, true)) { clip::set_text(sn); }
	
	ImGui::Separator();
	
	ImGui::MenuItem("Click to copy", NULL, false, false);
	
	ImGui::EndMenu();
}

inline bool gui_tbar_button(const char *label, ImVec2 size = ImVec2(0,0))
{
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
	bool res = ImGui::Button(label, size);
	ImGui::PopStyleColor();
	
	return res;
}

inline u8_t gui_tbar_main(s_gui_tbar *tbar)
{
	static f64_t time_limit_min = 0.0;
	static f64_t time_step_min = 0.001;
	static f64_t time_step_max = 100.0;
	static u32_t time_iter_min = 0x00;
	
	{
		ImVec2 popup_pos = ImGui::GetCursorScreenPos() + ImVec2(0, ImGui::GetTextLineHeightWithSpacing());
		
		if(gui_tbar_button("MENU", ImVec2(80,0)))
		{ ImGui::OpenPopup("gui_tbar_menu"); }
		
		ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200,0), ImGuiCond_Always);
		
		ImGui::GetStyle().DisplayWindowPadding = ImVec2(0,0);
		ImGui::GetStyle().DisplaySafeAreaPadding = ImVec2(0,0);
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16, 8));
			
		if (ImGui::BeginPopup("gui_tbar_menu"))
		{
			if (ImGui::MenuItem ("New Object")) { gui_tbar_menu_newobject(tbar); }
			if (ImGui::BeginMenu("File      ")) { gui_tbar_menu_file     (tbar); }
            if (ImGui::BeginMenu("Temrinal  ")) { gui_tbar_menu_terminal (tbar); }
            if (ImGui::MenuItem ("Settings  ")) { gui_conf_show(tbar->conf); }

			ImGui::Separator();
			
			if (ImGui::BeginMenu("Version   ")) { gui_tbar_menu_version  (tbar); }
			
			ImGui::EndPopup();
		}
		
		ImGui::PopStyleVar(1);
		
		ImGui::SameLine();
	}
	
	ImGui::SameLine(0,0);
	{
		char *file_name = tbar->file_path + strlen(tbar->file_path);
		while (file_name > tbar->file_path && *(file_name - 1) != '\\' && *(file_name - 1) != '/')
		{ --file_name; }
		
		char file_preview[256];
		sprintf(file_preview, " FILE: %s", file_name);
		
		ImGui::SetNextItemWidth(160);
		ImGui::InputText("##file_path", file_preview, 256, ImGuiInputTextFlags_ReadOnly);
		gui_hint(tbar->file_path);
		
		if (ImGui::IsItemClicked())
		{
			nfdchar_t *user_path;
			nfdfilteritem_t filterItem[1] = { { "Trajectory Project", "trj" } };
			nfdresult_t result = NFD_OpenDialog(&user_path, filterItem, 1, tbar->file_path);
			
			if (result == NFD_OKAY)
			{
				strcpy(tbar->file_path, user_path);
				trceng_load(tbar->eng, tbar->file_path);
				
				NFD_FreePath(user_path);
			}
		}
	}
	ImGui::SameLine();

	ImGui::AlignTextToFramePadding();

	ImGui::SetNextItemWidth(100);
	ImGui::DragScalar("##time_limit", ImGuiDataType_Double, &tbar->eng->time_limit, 1.0, &time_limit_min, NULL, "TIME: %.0f");
	gui_hint("Time limit [sec]");
	ImGui::SameLine(0,0);

	ImGui::SetNextItemWidth(100);
	ImGui::DragScalar("##time_step", ImGuiDataType_Double, &tbar->eng->time_step, 0.001, &time_step_min, &time_step_max, "STEP: %.3f");
	gui_hint("Time step [sec]");
	ImGui::SameLine();
	
	tbar->eng->time_iter = tbar->eng->time_limit / tbar->eng->time_step;
	
	ImGui::SetNextItemWidth(180);
	gui_sel_proc("##proc", tbar->eng, &tbar->eng->proc);
	ImGui::SameLine(0,0);
	
	if(gui_tbar_button("RENDER", ImVec2(80,0)))
	{ tbar->eng_gui->state = gui_eng_state_init; }
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __GUI_TBAR__ */




