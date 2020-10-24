
#include "trj_gui_eng.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_init(s_trj_gui_eng *gui, s_trj_gui_eng_init attr)
{
	gui->sel_item = NULL;
	gui->sel_type = trj_gui_eng_type_obj;
	gui->obj_list = attr.obj_list;
	gui->traj_offset = 0x00;
	
	for (int i = 0; i < 255; ++i)
	{
		gui->obj_list[i].hide = 0x00;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_add_trajapi(s_trj_gui_eng *gui, s_trj_traj api)
{
	gui->traj_list[gui->traj_offset] = api;
	++gui->traj_offset;
	
	return 0x00;
}

uint8_t trj_gui_eng_add_ctrlapi(s_trj_gui_eng *gui, s_trj_ctrl api)
{
	gui->ctrl_list[gui->ctrl_offset] = api;
	++gui->ctrl_offset;
	
	return 0x00;
}

//------------------------------------------------------------------------------

void __sel_obj__(s_trj_gui_eng *gui, void *obj)
{
	gui->sel_type = trj_gui_eng_type_obj;
	gui->sel_item = obj;
}

void __sel_traj__(s_trj_gui_eng *gui, void *traj)
{
	gui->sel_type = trj_gui_eng_type_traj;
	gui->sel_item = traj;
}

void __sel_ctrl__(s_trj_gui_eng *gui, void *ctrl)
{
	gui->sel_type = trj_gui_eng_type_ctrl;
	gui->sel_item = ctrl;
}

void __sel_proc__(s_trj_gui_eng *gui, void *proc)
{
	gui->sel_type = trj_gui_eng_type_proc;
	gui->sel_item = proc;
}

void __sel_data__(s_trj_gui_eng *gui, void *data)
{
	gui->sel_type = trj_gui_eng_type_data;
	gui->sel_item = data;
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
			
			if(ImGui::IsItemClicked()) { __sel_obj__(gui, &self->obj_list[i]); }
			
			ImGui::SameLine();
			char *hide_label[2] = { "hide", "show" };
			if (ImGui::SmallButton(hide_label[gui->obj_list[i].hide]))
			{ gui->obj_list[i].hide = !gui->obj_list[i].hide; }

			if (node_open)
			{
				if (ImGui::TreeNodeEx("traj"))
				{
					if (obj->traj_offset == 0x00) { ImGui::Text("[no items]"); }
					
					for (int j = 0; j < obj->traj_offset; ++j)
					{
						ImGui::PushID(j);
						
						bool traj_sel = gui->sel_item == &obj->traj_list[j];
						
						ImGui::TreeNodeEx((const char*) &obj->traj_list[j],
							   (traj_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
							   | ImGuiTreeNodeFlags_Leaf, obj->traj_list[j].desc);
						
						if (ImGui::IsItemClicked())
						{ __sel_traj__(gui, &obj->traj_list[j]); }
						
						ImGui::TreePop();
						
						ImGui::PopID();
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("ctrl"))
				{
					if (obj->ctrl_offset == 0x00) { ImGui::Text("[no items]"); }
					
					for (int j = 0; j < obj->ctrl_offset; ++j)
					{
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("proc"))
				{
					if (obj->proc_offset == 0x00) { ImGui::Text("[no items]"); }
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("data"))
				{
					if (obj->data_offset == 0x00) { ImGui::Text("[no items]"); }
					
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
	if (gui->sel_type != trj_gui_eng_type_obj || gui->sel_item == NULL) return 0x01;
	
	s_trj_obj *obj = (s_trj_obj*) gui->sel_item;
	s_trj_gui_obj *obj_gui = &gui->obj_list[obj->id];
	
	s_trj_traj *traj_api = &gui->traj_list[obj_gui->traj_sel];
	s_trj_ctrl *ctrl_api = &gui->ctrl_list[obj_gui->ctrl_sel];
//	s_trj_gui_traj_api *proc_api = &gui->traj_list[obj_gui->proc_sel];
//	s_trj_gui_traj_api *data_api = &gui->traj_list[obj_gui->data_sel];
	
	ImGui::PushID(obj);
	
	//--------------------------------------
	
	if (ImGui::Button("add##traj_add"))
	{ trj_obj_add_traj(obj, *traj_api); }
	
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1);
	
	if (ImGui::BeginCombo("##traj_type", gui->traj_list[obj_gui->traj_sel].desc,
						  ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < gui->traj_offset; ++i)
		{
			const bool is_selected = (obj_gui->traj_sel == i);
			
			if (ImGui::Selectable(gui->traj_list[obj_gui->traj_sel].desc, is_selected))
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
	
	if (ImGui::BeginCombo("##ctrl_type", gui->ctrl_list[obj_gui->ctrl_sel].desc,
						  ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 0; i < gui->ctrl_offset; ++i)
		{
			const bool is_selected = (obj_gui->ctrl_sel == i);
			
			if (ImGui::Selectable(gui->ctrl_list[obj_gui->ctrl_sel].desc, is_selected))
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
