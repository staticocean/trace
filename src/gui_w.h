
#ifndef __GUI_W__
#define __GUI_W__

//----------------------------------------------------------------

#include <libcommon/vl.h>
#include <libcommon/vl3d.h>
#include <libcommon/imgui_w.h>

#include <nfd.h>

#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_proc.h>
#include <lib/trj/trj_eng.h>

#include <libgui/imgui/imgui.h>
#include <libgui/imgui/imgui_internal.h>

//----------------------------------------------------------------

inline void gui_objsel(char *label, uint32_t obj_count, s_trj_obj *obj_list, s_trj_obj **obj)
{
    // to prevent corrupted data accessing wrong mem address
    // first check if an object belongs to the list

    uint8_t obj_exists = 0x00;

    for (int i = 0; i < obj_count; ++i)
    {
        if (*obj == &obj_list[i])
        {
            obj_exists = 0x01;
        }
    }

    if (obj_exists == 0x00 || *obj == NULL)
    {
        *obj = &obj_list[0];
    }

	if (ImGui::BeginCombo(label, (*obj)->desc, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < obj_count; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = ((*obj) == &obj_list[i]);
			
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

//----------------------------------------------------------------

inline void gui_ellpsel(char *label, uint32_t ellp_offset, s_trj_ellp *ellp_list, s_trj_ellp **ellp)
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

//----------------------------------------------------------------

inline void gui_procsel(char *label, s_trj_eng *eng)
{
	if (ImGui::BeginCombo(label, eng->proc.desc, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < eng->proc_offset; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = (eng->proc.hash == eng->proc_list[i].hash);
			
			if (ImGui::Selectable(eng->proc_list[i].desc, is_selected) && is_selected == false)
			{
			    eng->proc.free(&eng->proc.data);
                eng->proc = eng->proc_list[i];
                eng->proc.init(&eng->proc.data, eng->proc.config);
			}
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
	
	return;
}

//----------------------------------------------------------------

inline void gui_fileopen(char *file_path, uint32_t filter_sz, nfdfilteritem_t *filter_ls, float width = -1)
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
	{
		nfdchar_t *user_path;
		nfdresult_t result = NFD_OpenDialog(&user_path, filter_ls, filter_sz, file_path);
		
		if (result == NFD_OKAY)
		{
			strcpy(file_path, user_path);
			NFD_FreePath(user_path);
		}

//		else if (result == NFD_CANCEL)
//		{ puts("User pressed cancel."); }

//		else
//		{ printf("Error: %s\n", NFD_GetError() ); }
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_filesave(char *file_path, uint32_t filter_sz, nfdfilteritem_t *filter_ls, int width = -1)
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
	{
		nfdchar_t *user_path;
		nfdresult_t result = NFD_SaveDialog(&user_path, filter_ls, filter_sz, file_path, NULL);
		
		if (result == NFD_OKAY)
		{
			strcpy(file_path, user_path);
			NFD_FreePath(user_path);
		}

//		else if (result == NFD_CANCEL)
//		{ puts("User pressed cancel."); }

//		else
//		{ printf("Error: %s\n", NFD_GetError() ); }
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_vl3d(s_vl3d *vl3d)
{
	s_vl3d_view view = vl3d_view_init();
	vl3d_view_load(vl3d, &view, view);

	s_vl3d_gridline gridline = vl3d_gridline_init(&view);
	s_vl3d_griddot  griddot  = vl3d_griddot_init (&view);
	s_vl3d_xyz      xyz      = vl3d_xyz_init     (&view);
	
	s_vl3d_tbar tbar = vl3d_tbar_init();
	vl3d_tbar_load(vl3d, &tbar, tbar);
	
	gridline.disp = tbar.grid_disp;
	griddot.pt_disp = tbar.grid_disp;
	
	if (tbar.grid_mode == 0x01) { vl3d_griddot_draw(vl3d, &griddot); }
	if (tbar.grid_mode == 0x02) { vl3d_gridline_draw(vl3d, &gridline); }
	if (tbar.xyz_mode  != 0x00) { vl3d_xyz_draw(vl3d, &xyz); }
	
	vl3d_begin(vl3d);
	vl3d_view_ctrl3d(vl3d, &view);
	vl3d_view_draw(vl3d, &view);
	vl3d_tbar_draw(vl3d, &tbar, &view);
	vl3d_end();
	
	vl3d_view_save(vl3d, &view);
	vl3d_tbar_save(vl3d, &tbar);
}

//----------------------------------------------------------------

#endif /* __GUI_OBJ__ */


