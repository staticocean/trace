
#include "trj_gui_eng.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_init(s_trj_gui_eng *gui, s_trj_gui_eng_init attr)
{
	gui->state = trj_gui_eng_state_standby;
	
	gui->sel_item = NULL;
	gui->sel_type = trj_gui_eng_type_obj;
	gui->obj_list = attr.obj_list;
	
	gui->traj_offset = 0x00;
	gui->ctrl_offset = 0x00;
	gui->data_offset = 0x00;
	
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

uint8_t trj_gui_eng_add_dataapi(s_trj_gui_eng *gui, s_trj_data api)
{
	gui->data_list[gui->data_offset] = api;
	++gui->data_offset;
	
	return 0x00;
}

//------------------------------------------------------------------------------

void trj_gui_eng_sel_obj(s_trj_gui_eng *gui, s_trj_obj *obj)
{
	gui->sel_type = trj_gui_eng_type_obj;
	gui->sel_item = obj;
}

void trj_gui_eng_sel_traj(s_trj_gui_eng *gui, s_trj_traj *traj)
{
	gui->sel_type = trj_gui_eng_type_traj;
	gui->sel_item = traj;
}

void trj_gui_eng_sel_ctrl(s_trj_gui_eng *gui, s_trj_ctrl *ctrl)
{
	gui->sel_type = trj_gui_eng_type_ctrl;
	gui->sel_item = ctrl;
}

//void trj_gui_eng_sel_proc(s_trj_gui_eng *gui, void *proc)
//{
//	gui->sel_type = trj_gui_eng_type_proc;
//	gui->sel_item = proc;
//}

void trj_gui_eng_sel_data(s_trj_gui_eng *gui, s_trj_data *data)
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
			
			if(ImGui::IsItemClicked()) { trj_gui_eng_sel_obj(gui, &self->obj_list[i]); }
			
			ImGui::SameLine();
			char *hide_label[2] = { "hide", "show" };
			if (ImGui::SmallButton(hide_label[gui->obj_list[i].hide]))
			{ gui->obj_list[i].hide = !gui->obj_list[i].hide; }

			if (node_open)
			{
				{
					bool traj_open = ImGui::TreeNodeEx("traj");
					
					if (gui->traj_offset > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_traj"))
						{ ImGui::OpenPopup("add_traj_popup"); }
						
						if (ImGui::BeginPopup("add_traj_popup"))
						{
							for (int j = 0; j < gui->traj_offset; ++j)
							{
								ImGui::PushID(j);
								ImGui::Selectable(gui->traj_list[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trj_obj_add_traj(obj, gui->traj_list[j]);
								}
								
								ImGui::PopID();
							}
							
							ImGui::EndPopup();
						}
					}
					
					if (traj_open)
					{
						if (obj->traj_offset == 0x00)
						{ ImGui::Text("[no items]"); }
						
						for (int j = 0; j < obj->traj_offset; ++j)
						{
							ImGui::PushID(j);
							
							bool traj_sel = gui->sel_item == &obj->traj_list[j];
							
							ImGui::TreeNodeEx(obj->traj_list[j].desc,
											  (traj_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->traj_list[j].desc);
							
							if (ImGui::IsItemClicked())
							{ trj_gui_eng_sel_traj(gui, &obj->traj_list[j]); }
							
							ImGui::SameLine();
							
							if (ImGui::SmallButton("del##del_traj"))
							{
								if (gui->sel_item == &obj->traj_list[j])
								{ gui->sel_item = NULL; }
								
								trj_obj_del_traj(obj, &obj->traj_list[j]);
							}
							
							ImGui::TreePop();
							
							ImGui::PopID();
						}
						
						ImGui::TreePop();
					}
				}
				
				{
					bool ctrl_open = ImGui::TreeNodeEx("ctrl");
					
					if (gui->ctrl_offset > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_ctrl"))
						{ ImGui::OpenPopup("add_ctrl_popup"); }
						
						if (ImGui::BeginPopup("add_ctrl_popup"))
						{
							for (int j = 0; j < gui->ctrl_offset; ++j)
							{
								ImGui::PushID(j);
								ImGui::Selectable(gui->ctrl_list[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trj_obj_add_ctrl(obj, gui->ctrl_list[j]);
								}
								
								ImGui::PopID();
							}
							
							ImGui::EndPopup();
						}
					}
					
					if (ctrl_open)
					{
						if (obj->ctrl_offset == 0x00)
						{ ImGui::Text("[no items]"); }
						
						for (int j = 0; j < obj->ctrl_offset; ++j)
						{
							ImGui::PushID(j);
							
							bool ctrl_sel = gui->sel_item == &obj->ctrl_list[j];
							
							ImGui::TreeNodeEx(obj->ctrl_list[j].desc,
											  (ctrl_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->ctrl_list[j].desc);
							
							if (ImGui::IsItemClicked())
							{ trj_gui_eng_sel_ctrl(gui, &obj->ctrl_list[j]); }
							
							ImGui::SameLine();
							
							if (ImGui::SmallButton("del##del_ctrl"))
							{
								if (gui->sel_item == &obj->ctrl_list[j])
								{ gui->sel_item = NULL; }
								
								trj_obj_del_ctrl(obj, &obj->ctrl_list[j]);
							}
							
							ImGui::TreePop();
							
							ImGui::PopID();
						}
						
						ImGui::TreePop();
					}
				}
				{
					bool data_open = ImGui::TreeNodeEx("data");
					
					if (gui->data_offset > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_data"))
						{ ImGui::OpenPopup("add_data_popup"); }
						
						if (ImGui::BeginPopup("add_data_popup"))
						{
							for (int j = 0; j < gui->data_offset; ++j)
							{
								ImGui::PushID(j);
								ImGui::Selectable(gui->data_list[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trj_obj_add_data(obj, gui->data_list[j]);
								}
								
								ImGui::PopID();
							}
							
							ImGui::EndPopup();
						}
					}
					
					if (data_open)
					{
						if (obj->data_offset == 0x00)
						{ ImGui::Text("[no items]"); }
						
						for (int j = 0; j < obj->data_offset; ++j)
						{
							ImGui::PushID(j);
							
							bool ctrl_sel = gui->sel_item == &obj->data_list[j];
							
							ImGui::TreeNodeEx(obj->data_list[j].desc,
											  (ctrl_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->data_list[j].desc);
							
							if (ImGui::IsItemClicked())
							{ trj_gui_eng_sel_data(gui, &obj->data_list[j]); }
							
							ImGui::SameLine();
							
							if (ImGui::SmallButton("del##del_data"))
							{
								if (gui->sel_item == &obj->data_list[j])
								{ gui->sel_item = NULL; }
								
								trj_obj_del_data(obj, &obj->data_list[j]);
							}
							
							ImGui::TreePop();
							
							ImGui::PopID();
						}
						
						ImGui::TreePop();
					}
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

uint8_t trj_gui_eng_updateeng(s_trj_gui_eng *gui, s_trj_eng *self)
{
	switch (gui->state)
	{
		case trj_gui_eng_state_standby:
		{
			break;
		}
		
		case trj_gui_eng_state_init:
		{
			if (gui->time_step < 1E-9) { return 0x01; }
			
			trj_eng_reset(self);
			
			for (int i = 0; i < self->obj_count; ++i)
			{
				self->obj_list[i].log_list = (s_trj_obj_data*) malloc(sizeof(s_trj_obj_data) * (gui->time_iter+1));
				self->obj_list[i].log_offset = 0x00;
			}
			
			break;
		}
		
		case trj_gui_eng_state_update:
		{
			if (gui->time_step < 1E-9) { return 0x01; }
			
			if (self->time[0] < gui->time_limit)
			{
				trj_eng_update(self, gui->time_step);
				trj_eng_log(self);
			}
			
			break;
		}
		
		case trj_gui_eng_state_deinit:
		{
			if (gui->time_step < 1E-9) { return 0x01; }
			
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	return 0x00;
}


uint8_t trj_gui_eng_updategui(s_trj_gui_eng *gui, s_trj_eng *self)
{
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	
	if (ImGui::BeginPopupModal("RENDERING", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		if (gui->state == trj_gui_eng_state_update)
		{
			const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
			const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);
			
			ImGui::BufferingBar("##progress", self->time[0] / gui->time_limit, ImVec2(400, 6), bg, col);
			
			if (ImGui::Button("INTERRUPT", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		}
		
		if (gui->state == trj_gui_eng_state_standby)
		{
			if (ImGui::Button("CLOSE", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		}
		
//		ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
//		ImGui::Separator();
//
//		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
//		ImGui::PopStyleVar();

		ImGui::EndPopup();
	}
	
	// Always center this window when appearing
	switch (gui->state)
	{
		case trj_gui_eng_state_standby:
		{
			break;
		}
		
		case trj_gui_eng_state_init:
		{
			gui->state = trj_gui_eng_state_update;
			
			ImGui::OpenPopup("RENDERING");
			
			break;
		}
		
		case trj_gui_eng_state_update:
		{
			if (self->time[0] >= gui->time_limit)
			{
				gui->state = trj_gui_eng_state_deinit;
			}
			
			break;
		}
		
		case trj_gui_eng_state_deinit:
		{
			gui->state = trj_gui_eng_state_standby;
			
			break;
		}
		
		default:
		{
			gui->state = trj_gui_eng_state_standby;
			
			break;
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------
