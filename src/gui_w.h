
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_W__
#define __GUI_W__

//------------------------------------------------------------------------------

#include <nfd.h>

#include <sftlstd/vld3.h>
#include <sftlstd/vl3d.h>
#include <sftlstd/vl3d_imgui.h>

#include <sftlgui/imgui_custom.h>

#include <sftltrc/trc.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

//------------------------------------------------------------------------------

void gui_sel_obj (char *label, s_trceng *eng, s_trcobj **obj)
{
    // to prevent corrupted data accessing wrong mem address
    // first check if an object belongs to the list

    t_u8 obj_exists = 0x00;

    for (int i = 0; i < eng->obj_sz; ++i)
    {
    	if (*obj == eng->obj_ls[i])
        {
            obj_exists = 0x01;
        }
    }

    if (obj_exists == 0x00 || *obj == NULL)
    {
    	*obj = eng->obj_ls[0];
    }

	if (ImGui::BeginCombo(label, (*obj)->name, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < eng->obj_sz; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = ((*obj) == eng->obj_ls[i]);
			
			if (ImGui::Selectable(eng->obj_ls[i]->name, is_selected))
			{
				*obj = eng->obj_ls[i];
			}
			
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
}

//------------------------------------------------------------------------------

void gui_sel_refs (char *label, s_trceng *eng, s_trcrefs **refs)
{
//	bool is_none = *ellp == NULL;
//
//	if (ImGui::BeginCombo(label,
//						  is_none ? "none" : (*ellp)->desc,
//						  ImGuiComboFlags_NoArrowButton))
//	{
//		if (ImGui::Selectable("none", is_none)) { *ellp = NULL; }
//		if (is_none) { ImGui::SetItemDefaultFocus(); }
//
//		// is_none can change after previous 2 lines
//		is_none = *ellp == NULL;
//
//		for (int i = 0; i < ellp_offset; ++i)
//		{
//			ImGui::PushID(i);
//
//			bool is_selected = false;
//
//			if (!is_none && ((*ellp)->hash == ellp_ls[i].hash))
//			{ is_selected = true; }
//
//			if (ImGui::Selectable(ellp_ls[i].desc, is_selected))
//			{ *ellp = &ellp_ls[i]; }
//
//			if (is_selected) { ImGui::SetItemDefaultFocus(); }
//
//			ImGui::PopID();
//		}
//
//		ImGui::EndCombo();
//	}
}

//------------------------------------------------------------------------------

void gui_sel_proc (char *label, s_trceng *eng, s_trcproc **proc)
{
	if (ImGui::BeginCombo(label, (*proc)->name, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < eng->proc_sz; ++i)
		{
			ImGui::PushID(i);
			
			bool is_selected = (*proc == eng->proc_ls[i]);
			
			if (ImGui::Selectable(eng->proc_ls[i]->name, is_selected) && !is_selected)
			{
                *proc = eng->proc_ls[i];
			}
			
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
			
			ImGui::PopID();
		}
		
		ImGui::EndCombo();
	}
}

//------------------------------------------------------------------------------

void gui_file_open (char *file_path, t_u32 filter_sz, nfdfilteritem_t *filter_ls, float width = -1)
{
	ImGui::PushID(file_path);
	
	if (width < 0) width = ImGui::GetContentRegionAvail().x;
	
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
}

//------------------------------------------------------------------------------

void gui_file_save (char *file_path, t_u32 filter_sz, nfdfilteritem_t *filter_ls, int width = -1)
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
	
	if(ImGui::Button("SEL", ImVec2(ImGui::GetContentRegionAvail().x,0)))
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
}

//------------------------------------------------------------------------------

void gui_vl3d (s_vl3d *vl3d)
{
	s_vl3d_view view = vl3d_view_init();
	vl3d_view_load(vl3d, &view, view);

	s_vl3d_gridline gridline = vl3d_gridline_init(view.scale, view.pos);
	s_vl3d_griddot  griddot  = vl3d_griddot_init (view.scale, view.pos);
	s_vl3d_xyz      xyz      = vl3d_xyz_init     (view.scale, view.pos);
	
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

//------------------------------------------------------------------------------

#endif /* __GUI_W__ */


