
#ifndef __TRJ_GUI_W__
#define __TRJ_GUI_W__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <isss_lib/vl.h>
#include <isss_lib/vl3d.h>
#include <isss_lib/imgui_w.h>

#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_proc.h>
#include <lib/trj/trj_eng.h>

#include <lib/imgui/imgui_internal.h>

#include <lib/imfilebrowser/imfilebrowser.h>

//------------------------------------------------------------------------------

inline void trj_gui_objsel(char *label, uint32_t obj_count, s_trj_obj *obj_list, s_trj_obj **obj)
{
	if (ImGui::BeginCombo(label, (*obj)->desc, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < obj_count; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = ((*obj)->hash == obj_list[i].hash);
			
			if (ImGui::Selectable(obj_list[i].desc, is_selected))
			{ *obj = &obj_list[i]; }
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_ellpsel(char *label, uint32_t ellp_offset, s_trj_ellp *ellp_list, s_trj_ellp **ellp)
{
	bool is_none = *ellp == NULL;
	
	if (ImGui::BeginCombo(label,
						  is_none ? "none" : (*ellp)->desc,
						  ImGuiComboFlags_NoArrowButton))
	{
		if (ImGui::Selectable("none", is_none)) { *ellp = NULL; }
		if (is_none) { ImGui::SetItemDefaultFocus(); }
		
		// is_none can change after previous 2 lines
		is_none = *ellp == NULL;
		
		for (int i = 0; i < ellp_offset; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = false;
			
			if (!is_none && ((*ellp)->hash == ellp_list[i].hash))
			{ is_selected = true; }
			
			if (ImGui::Selectable(ellp_list[i].desc, is_selected))
			{ *ellp = &ellp_list[i]; }
			
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void trj_gui_procsel(char *label, s_trj_eng *eng)
{
	if (ImGui::BeginCombo(label, eng->proc->desc, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < eng->proc_offset; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = (eng->proc->hash == eng->proc_list[i].hash);
			
			if (ImGui::Selectable(eng->proc_list[i].desc, is_selected))
			{ eng->proc = &eng->proc_list[i]; }
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//------------------------------------------------------------------------------

inline ImGui::FileBrowser __file_browser_open__ = ImGui::FileBrowser();
inline ImGui::FileBrowser __file_browser_save__ = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);

inline void trj_gui_fileopen(char *file_path, float width = -1)
{
	ImGui::PushID(file_path);
	
	if (width < 0) width = ImGui::GetContentRegionAvailWidth();
	
	ImGui::SetNextItemWidth(width-40);
	char *file_name = file_path + strlen(file_path);
	while (file_name > file_path && *(file_name-1) != '\\' && *(file_name-1) != '/')
	{ --file_name; }
	
	ImGui::InputText("##file_path", file_name, 512, ImGuiInputTextFlags_ReadOnly);
	if (ImGui::IsItemHovered())
	{ ImGui::SetTooltip(file_path); }

	ImGui::SameLine(0.0, 0.0);
	
	if(ImGui::Button("SEL", ImVec2(40,0)))
	{ __file_browser_open__.Open(); }
	
	__file_browser_open__.Display();
	
	if(__file_browser_open__.HasSelected())
	{
		strcpy(file_path, __file_browser_open__.GetSelected().string().c_str());
		__file_browser_open__.ClearSelected();
	}
	
	ImGui::PopID();
	
	return;
}

inline void trj_gui_filesave(char *file_path)
{
	ImGui::PushID(file_path);
	
	ImGui::SetNextItemWidth(-40);
	char *file_name = file_path + strlen(file_path);
	while (file_name > file_path && *(file_name-1) != '\\' && *(file_name-1) != '/')
	{ --file_name; }
	
	ImGui::InputText("##file_path", file_name, 512, ImGuiInputTextFlags_ReadOnly);
	if (ImGui::IsItemHovered())
	{ ImGui::SetTooltip(file_path); }
	
	ImGui::SameLine(0.0, 0.0);
	
	if(ImGui::Button("SEL", ImVec2(ImGui::GetContentRegionAvailWidth(),0)))
	{ __file_browser_save__.Open(); }
	
	__file_browser_save__.Display();
	
	if(__file_browser_save__.HasSelected())
	{
		strcpy(file_path, __file_browser_save__.GetSelected().string().c_str());
		__file_browser_save__.ClearSelected();
	}
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_OBJ__ */


