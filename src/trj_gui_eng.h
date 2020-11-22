
#ifndef __TRJ_GUI_ENG__
#define __TRJ_GUI_ENG__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <lib/trj/vl.h>
#include <lib/trj/trj_api.h>
#include <lib/trj/trj_eng.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_ellp.h>

#include "imgui_custom.h"

#include "trj_gui_obj.h"
#include "trj_gui_traj.h"

//------------------------------------------------------------------------------

enum trj_gui_eng_type_t
{
	trj_gui_eng_type_obj,
	trj_gui_eng_type_traj,
	trj_gui_eng_type_ctrl,
	trj_gui_eng_type_data,
	trj_gui_eng_type_proc,
};

typedef enum trj_gui_eng_state_t
{
	trj_gui_eng_state_standby,
	trj_gui_eng_state_init,
	trj_gui_eng_state_update,
	trj_gui_eng_state_deinit,
};

typedef struct trj_gui_eng
{
	float64_t 			time_limit;
	float64_t 			time_step;
	uint32_t 			time_iter;
	
	trj_gui_eng_state_t state;
	
	void* sel_item;
	trj_gui_eng_type_t sel_type;

	s_trj_gui_obj *obj_list;
	
}	s_trj_gui_eng;

typedef struct trj_gui_eng_init
{
	s_trj_gui_obj *obj_list;

} 	s_trj_gui_eng_init;

//------------------------------------------------------------------------------

inline uint8_t trj_gui_eng_init(s_trj_gui_eng *gui, s_trj_gui_eng_init attr)
{
	gui->state = trj_gui_eng_state_standby;
	
	gui->sel_item = NULL;
	gui->sel_type = trj_gui_eng_type_obj;
	gui->obj_list = attr.obj_list;
	
	for (int i = 0; i < 255; ++i)
	{
		gui->obj_list[i].hide = 0x00;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline void trj_gui_eng_sel_obj(s_trj_gui_eng *gui, s_trj_obj *obj)
{
	gui->sel_type = trj_gui_eng_type_obj;
	gui->sel_item = obj;
}

inline void trj_gui_eng_sel_traj(s_trj_gui_eng *gui, s_trj_traj *traj)
{
	gui->sel_type = trj_gui_eng_type_traj;
	gui->sel_item = traj;
}

inline void trj_gui_eng_sel_ctrl(s_trj_gui_eng *gui, s_trj_ctrl *ctrl)
{
	gui->sel_type = trj_gui_eng_type_ctrl;
	gui->sel_item = ctrl;
}

//void trj_gui_eng_sel_proc(s_trj_gui_eng *gui, void *proc)
//{
//	gui->sel_type = trj_gui_eng_type_proc;
//	gui->sel_item = proc;
//}

inline void trj_gui_eng_sel_data(s_trj_gui_eng *gui, s_trj_data *data)
{
	gui->sel_type = trj_gui_eng_type_data;
	gui->sel_item = data;
}

//------------------------------------------------------------------------------

inline uint8_t trj_gui_eng_objlist(s_trj_gui_eng *gui, s_trj_eng *self)
{
	static ImGuiTextFilter filter;
	static void* selected_item = NULL;
	
	char* filter_data[self->obj_count];
	
	for (int i = 0; i < self->obj_count; ++i)
	{
		filter_data[i] = self->obj_list[i].desc;
	}
	
	ImGui::SetNextItemWidth(-1);
	filter.Draw("");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
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
											   (char*) obj->desc);
			
			if(ImGui::IsItemClicked()) { trj_gui_eng_sel_obj(gui, &self->obj_list[i]); }
			
			ImGui::SameLine();
			char *hide_label[2] = { "hide", "show" };
			if (ImGui::SmallButton(hide_label[gui->obj_list[i].hide]))
			{ gui->obj_list[i].hide = !gui->obj_list[i].hide; }
			
			if (node_open)
			{
				{
					bool traj_open = ImGui::TreeNodeEx("traj");
					
					if (self->traj_offset > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_traj"))
						{ ImGui::OpenPopup("add_traj_popup"); }
						
						if (ImGui::BeginPopup("add_traj_popup"))
						{
							for (int j = 0; j < self->traj_offset; ++j)
							{
								ImGui::PushID(j);
								ImGui::Selectable(self->traj_list[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trj_obj_add_traj(obj, self->traj_list[j]);
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
					
					if (self->ctrl_offset > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_ctrl"))
						{ ImGui::OpenPopup("add_ctrl_popup"); }
						
						if (ImGui::BeginPopup("add_ctrl_popup"))
						{
							for (int j = 0; j < self->ctrl_offset; ++j)
							{
								ImGui::PushID(j);
								ImGui::Selectable(self->ctrl_list[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trj_obj_add_ctrl(obj, self->ctrl_list[j]);
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
					
					if (self->data_offset > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_data"))
						{ ImGui::OpenPopup("add_data_popup"); }
						
						if (ImGui::BeginPopup("add_data_popup"))
						{
							for (int j = 0; j < self->data_offset; ++j)
							{
								ImGui::PushID(j);
								ImGui::Selectable(self->data_list[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trj_obj_add_data(obj, self->data_list[j]);
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

//------------------------------------------------------------------------------

inline uint8_t trj_gui_eng_updateeng(s_trj_gui_eng *gui, s_trj_eng *self)
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
			
			// run data render routines
			trj_eng_render(self);
			
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_gui_eng_updategui(s_trj_gui_eng *gui, s_trj_eng *self)
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

#endif /* __TRJ_GUI_ENG__ */
