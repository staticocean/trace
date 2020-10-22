
#include "trj_gui_eng.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_init(s_trj_gui_eng *gui, s_trj_gui_eng_init attr)
{
	gui->sel_item = NULL;
	gui->sel_type = 0x00;
	gui->obj_list = attr.obj_list;
	gui->traj_api_offset = 0x00;
	
	for (int i = 0; i < 255; ++i)
	{
		gui->obj_list[i].hide = 0x00;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_add_trajapi(s_trj_gui_eng *gui, s_trj_traj_api api)
{
	gui->traj_api_list[gui->traj_api_offset] = api;
	++gui->traj_api_offset;
	
	return 0x00;
}

uint8_t trj_gui_eng_add_ctrlapi(s_trj_gui_eng *gui, s_trj_ctrl_api api)
{
	gui->ctrl_api_list[gui->ctrl_api_offset] = api;
	++gui->ctrl_api_offset;
	
	return 0x00;
}

//------------------------------------------------------------------------------

void __sel_object__(s_trj_gui_eng *gui, s_trj_obj *obj)
{
	gui->sel_type = 0x00;
	gui->sel_item = obj;
}

uint8_t trj_gui_eng_objlist(s_trj_gui_eng *gui, s_trj_eng *self)
{
	static ImGuiTextFilter filter;
	static void* selected_item = NULL;
	
	char* filter_data[self->obj_count];
	
	for (int i = 0; i < self->obj_count; ++i)
	{
		filter_data[i] = self->obj_list[i].name;
	}
	
	ImGui::SetNextItemWidth(-40);
	filter.Draw("");
	
	ImGui::SameLine();
	ImGui::Button("add", ImVec2(40, 0));
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);
	
	for (int i = 0; i < IM_ARRAYSIZE(filter_data); i++)
	{
		ImGui::PushID(i);
		
		if (filter.PassFilter((char*) filter_data[i]))
		{
			s_trj_obj *obj = &self->obj_list[i];
			s_trj_gui_obj *obj_gui = &gui->obj_list[i];
			
			bool node_sel = gui->sel_item == &self->obj_list[i];
			
			bool node_open = ImGui::TreeNodeEx((void*) i,
					(node_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
					| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
					(char*) obj->name);
			
			if(ImGui::IsItemClicked()) { __sel_object__(gui, &self->obj_list[i]); }
			
			ImGui::SameLine();
			char *hide_label[2] = { "hide", "show" };
			if (ImGui::SmallButton(hide_label[gui->obj_list[i].hide]))
			{ gui->obj_list[i].hide = !gui->obj_list[i].hide; }

			if (node_open)
			{
				if (ImGui::TreeNodeEx("traj"))
				{
					if (obj->traj_offset == 0x00)
					{
						ImGui::Text("[no items]");
					}
					
					for (int j = 0; j < obj->traj_offset; ++j)
					{
					
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("ctrl"))
				{
					if (obj->ctrl_offset == 0x00)
					{
						ImGui::Text("[no items]");
					}
					
					for (int j = 0; j < obj->ctrl_offset; ++j)
					{
					
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("proc"))
				{
					if (obj->proc_offset == 0x00)
					{
						ImGui::Text("[no items]");
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("data"))
				{
					if (obj->data_offset == 0x00)
					{
						ImGui::Text("[no items]");
					}
					
					ImGui::TreePop();
				}
				
				ImGui::TreePop();
			}
		}
		
		ImGui::PopID();
	}
	
	ImGui::PopStyleVar();
	
	return 0x00;
}

uint8_t trj_gui_eng_addbox(s_trj_gui_eng *gui, s_trj_eng *self)
{
	if (gui->sel_type != 0x00 || gui->sel_item == NULL) return 0x01;
	
	s_trj_obj *obj = (s_trj_obj*) gui->sel_item;
	s_trj_gui_obj *obj_gui = &gui->obj_list[obj->id];
	
	ImGui::PushID(obj);
	
	//--------------------------------------
	
	if (ImGui::Button("add##traj_add"))
	{
//						self->traj_offset++;
	}
	
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	
	if (ImGui::BeginCombo("##traj_type", gui->traj_api_list[obj_gui->traj_sel].desc,
						  ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < gui->traj_api_offset; ++i)
		{
			const bool is_selected = (obj_gui->traj_sel == i);
			
			if (ImGui::Selectable(gui->traj_api_list[obj_gui->traj_sel].desc, is_selected))
			{ obj_gui->traj_sel = i; }
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
		}
		
		ImGui::EndCombo();
	}
	
	//--------------------------------------
	
	if (ImGui::Button("add##ctrl_add"))
	{
//						self->ctrl_offset++;
	}
	
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	
	if (ImGui::BeginCombo("##ctrl_type", gui->ctrl_api_list[obj_gui->ctrl_sel].desc,
						  ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < gui->ctrl_api_offset; ++i)
		{
			const bool is_selected = (obj_gui->traj_sel == i);
			
			if (ImGui::Selectable(gui->ctrl_api_list[obj_gui->ctrl_sel].desc, is_selected))
			{ obj_gui->ctrl_sel = i; }
			
			if (is_selected)
			{ ImGui::SetItemDefaultFocus(); }
		}
		
		ImGui::EndCombo();
	}
	
//	if (ImGui::Button("add##ctrl_add"))
//	{
////						self->traj_offset++;
//	}
//
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(-1);
//
//	if (ImGui::BeginCombo("##ctrl_type", gui->traj_api_list[obj_gui->traj_sel].desc,
//						  ImGuiComboFlags_NoArrowButton))
//	{
//		for (int i = 0; i < gui->traj_api_offset; ++i)
//		{
//			const bool is_selected = (obj_gui->traj_sel == i);
//
//			if (ImGui::Selectable(gui->traj_api_list[obj_gui->traj_sel].desc, is_selected))
//			{ obj_gui->traj_sel = i; }
//
//			if (is_selected)
//			{ ImGui::SetItemDefaultFocus(); }
//		}
//
//		ImGui::EndCombo();
//	}
	
	ImGui::PopID();
	
	return 0x00;
}

//------------------------------------------------------------------------------
