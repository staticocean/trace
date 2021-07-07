
#ifndef __GUI_ENG__
#define __GUI_ENG__

//----------------------------------------------------------------

#include <lib/imgui/imgui.h>
#include <lib/implot/implot.h>

#include <lib_internal/vl.h>
#include <lib_internal/imgui_w.h>

#include <lib/trj/trj_api.h>
#include <lib/trj/trj_eng.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_ellp.h>

#include <lib_gui/lib/clip/clip.h>

#include "gui_obj.h"
#include "gui_traj.h"

//----------------------------------------------------------------

enum gui_eng_type_t
{
	gui_eng_type_obj,
	gui_eng_type_traj,
	gui_eng_type_ctrl,
	gui_eng_type_data,
	gui_eng_type_proc,
};

enum gui_eng_state_t
{
	gui_eng_state_standby,
	gui_eng_state_init,
	gui_eng_state_update,
	gui_eng_state_proc,
	gui_eng_state_deinit,
	
};

typedef struct gui_eng
{
	gui_eng_state_t state;
	
	void* sel_item;
	gui_eng_type_t sel_type;

	s_gui_obj *obj_list;
	
}	s_gui_eng;

typedef struct gui_eng_init
{
	s_gui_obj *obj_list;

} 	s_gui_eng_init;

//----------------------------------------------------------------

inline uint8_t gui_eng_init(s_gui_eng *gui, s_gui_eng_init attr)
{
	gui->state = gui_eng_state_standby;
	
	gui->sel_item = NULL;
	gui->sel_type = gui_eng_type_obj;
	gui->obj_list = attr.obj_list;
	
	for (int i = 0; i < 255; ++i)
	{
		gui->obj_list[i].hide = 0x00;
	}
	
	return 0x00;
}

//----------------------------------------------------------------

inline void gui_eng_sel_obj(s_gui_eng *gui, s_gui_obj *obj)
{
	gui->sel_type = gui_eng_type_obj;
	gui->sel_item = obj;
}

inline void gui_eng_sel_traj(s_gui_eng *gui, s_trj_traj *traj)
{
	gui->sel_type = gui_eng_type_traj;
	gui->sel_item = traj;
}

inline void gui_eng_sel_ctrl(s_gui_eng *gui, s_trj_ctrl *ctrl)
{
	gui->sel_type = gui_eng_type_ctrl;
	gui->sel_item = ctrl;
}

//void gui_eng_sel_proc(s_gui_eng *gui, void *proc)
//{
//	gui->sel_type = gui_eng_type_proc;
//	gui->sel_item = proc;
//}

inline void gui_eng_sel_data(s_gui_eng *gui, s_trj_data *data)
{
	gui->sel_type = gui_eng_type_data;
	gui->sel_item = data;
}

//----------------------------------------------------------------

inline uint8_t gui_eng_objlist(s_gui_eng *gui, s_trj_eng *self)
{
	ImGui::PushID(self);
	
	static ImGuiTextFilter filter;
	static void* selected_item = NULL;
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	filter.Draw("");
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::BeginChild("##tree_view");
	
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);
//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
	
	for (int i = 0; i < self->obj_count; i++)
	{
		ImGui::PushID(&gui->obj_list[i]);
		
		s_gui_obj *obj_gui = &gui->obj_list[i];
		s_trj_obj *obj = obj_gui->ref;
		
		if (filter.PassFilter(obj->desc))
		{
			bool node_sel = gui->sel_item == &gui->obj_list[i];
			
			ImGui::AlignTextToFramePadding();
			bool node_open = ImGui::TreeNodeEx((void*) i,
											   (node_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											   | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
											   (char*) obj->desc);
			
			if (ImGui::BeginPopupContextItem("obj_options"))
			{
                if (ImGui::Selectable("copy"))
                {
                }

                if (ImGui::Selectable("paste"))
                {
                }

				if (ImGui::Selectable("delete"))
				{
					if (gui->sel_item == obj_gui)
					{ gui->sel_item = NULL; }
					
					trj_eng_del_obj(self, i);
				}
				
				ImGui::EndPopup();
			}
			
			if(ImGui::IsItemClicked()) { gui_eng_sel_obj(gui, obj_gui); }
			
			if (node_open)
			{
				{
					ImGui::AlignTextToFramePadding();
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
								ImGui::PushID(&self->traj_list[j]);
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
							ImGui::PushID(&obj->traj_list[j]);
							
							bool traj_sel = gui->sel_item == &obj->traj_list[j];
							
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx(obj->traj_list[j].name,
											  (traj_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->traj_list[j].name);
							
							if (ImGui::IsItemClicked())
							{ gui_eng_sel_traj(gui, &obj->traj_list[j]); }
							
							if (ImGui::BeginPopupContextItem("traj_menu"))
							{
								if (ImGui::Selectable("delete"))
								{
									if (gui->sel_item == &obj->traj_list[j])
									{ gui->sel_item = NULL; }
									
									trj_obj_del_traj(obj, &obj->traj_list[j]);
								}
								
								ImGui::EndPopup();
							}
							
							ImGui::TreePop();
							
							ImGui::PopID();
						}
						
						ImGui::TreePop();
					}
				}
				
				{
					ImGui::AlignTextToFramePadding();
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
								ImGui::PushID(&self->ctrl_list[j]);
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
							ImGui::PushID(&obj->ctrl_list[j]);
							
							bool ctrl_sel = gui->sel_item == &obj->ctrl_list[j];
							
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx(obj->ctrl_list[j].name,
											  (ctrl_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->ctrl_list[j].name);
							
							if (ImGui::IsItemClicked())
							{ gui_eng_sel_ctrl(gui, &obj->ctrl_list[j]); }
							
							if (ImGui::BeginPopupContextItem("ctrl_menu"))
							{
								if (ImGui::Selectable("delete"))
								{
									if (gui->sel_item == &obj->ctrl_list[j])
									{ gui->sel_item = NULL; }
									
									trj_obj_del_ctrl(obj, &obj->ctrl_list[j]);
								}
								
								ImGui::EndPopup();
							}
							
							ImGui::TreePop();
							
							ImGui::PopID();
						}
						
						ImGui::TreePop();
					}
				}
				{
					ImGui::AlignTextToFramePadding();
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
								ImGui::PushID(&self->data_list[j]);
								ImGui::Selectable(self->data_list[j].desc);

								if (ImGui::IsItemClicked())
								{ trj_obj_add_data(obj, self->data_list[j]); }

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
							ImGui::PushID(&obj->data_list[j]);

							bool data_sel = gui->sel_item == &obj->data_list[j];
							
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx(obj->data_list[j].name,
											  (data_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->data_list[j].name);

							if (ImGui::IsItemClicked())
							{ gui_eng_sel_data(gui, &obj->data_list[j]); }
							
							if (ImGui::BeginPopupContextItem("data_menu"))
							{
								if (ImGui::Selectable("delete"))
								{
									if (gui->sel_item == &obj->data_list[j])
									{ gui->sel_item = NULL; }

									trj_obj_del_data(obj, &obj->data_list[j]);
								}

								ImGui::EndPopup();
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
	
	ImGui::PopStyleVar(2);

	ImGui::EndChild();
	
	ImGui::PopID();
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t gui_eng_updateeng(s_gui_eng *gui, s_trj_eng *self)
{
	switch (gui->state)
	{
		case gui_eng_state_standby:
		{
			break;
		}
		
		case gui_eng_state_init:
		{
			if (self->time_step < 1E-6)
			{
				gui->state = gui_eng_state_standby;
				return 0x01;
			}
			
			trj_eng_reset(self);
			
			self->time_iter = self->time_limit / self->time_step;
			
			for (int i = 0; i < self->obj_count; ++i)
			{
				if (self->obj_list[i].log_list != NULL)
				{ free(self->obj_list[i].log_list); }
				
				self->obj_list[i].log_list = (s_trj_obj_data*) malloc(sizeof(s_trj_obj_data) * (self->time_iter+1));
				self->obj_list[i].log_offset = 0x00;
			}
			
			trj_eng_log(self);
			
			gui->state = gui_eng_state_update;
			
			break;
		}
		
		case gui_eng_state_update:
		{
			if (self->time_step < 1E-6)
			{
				gui->state = gui_eng_state_standby;
				return 0x01;
			}
			
			// prevent overflowing buffers and time_limit
			if ((self->time[0]+self->time_step) < (self->time_limit+1E-9))
			{
				trj_eng_update(self, self->time_step);
				trj_eng_log(self);
			}
			
			else { gui->state = gui_eng_state_proc; }
			
			break;
		}
		
		case gui_eng_state_proc:
		{
			if (self->proc_count < self->update_count)
			{
				trj_eng_proc(self);
			}
			else { gui->state = gui_eng_state_deinit; }
			
			break;
		}
		
		case gui_eng_state_deinit:
		{
			// run data render routines
			trj_eng_render(self);
			gui->state = gui_eng_state_standby;
			
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t gui_eng_updategui(s_gui_eng *gui, s_trj_eng *self)
{
	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);
	
	const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
	const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);
	
	char window_name[255] = "RENDERING";
	
//	if      (gui->state == gui_eng_state_update ) 	{ sprintf(window_name, "RENDERING - CTRL PASS"); }
//	else if (gui->state == gui_eng_state_proc   ) 	{ sprintf(window_name, "RENDERING - PROC PASS"); }
//	else if (gui->state == gui_eng_state_deinit ) 	{ sprintf(window_name, "RENDERING - DATA PASS"); }
//	else if (gui->state == gui_eng_state_standby) 	{ sprintf(window_name, "RENDERING - SUMMARY"); }
	
	if (ImGui::BeginPopupModal(window_name, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Dummy(ImVec2(0, 5));
		
		ImGui::AlignTextToFramePadding();
		
		if (gui->state == gui_eng_state_update)
		{
			ImGui::Text("CTRL PASS %02.0f%%", 100 * self->time[0] / self->time_limit);
			ImGui::SameLine();
			if (ImGui::Button("INTERRUPT", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::Dummy(ImVec2(0, 5));
			imgui_loadbar("##progress", self->time[0] / self->time_limit, ImVec2(-1, 6), bg, col);
		}
		
		if (gui->state == gui_eng_state_proc)
		{
			ImGui::Text("PROC PASS %02.0f%%", 100 * (vlf_t) self->proc_count / self->update_count);
			ImGui::SameLine();
			if (ImGui::Button("INTERRUPT", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::Dummy(ImVec2(0, 5));
			imgui_loadbar("##progress", (vlf_t) self->proc_count / self->update_count, ImVec2(-1, 6), bg, col);
		}
		
		if (gui->state == gui_eng_state_deinit)
		{
			ImGui::Text("DATA PASS %02.0f%%", 100 * (vlf_t) self->proc_count / self->update_count);
			ImGui::SameLine();
			if (ImGui::Button("INTERRUPT", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::Dummy(ImVec2(0, 5));
			imgui_loadbar("##progress", (vlf_t) self->proc_count / self->update_count, ImVec2(-1, 6), bg, col);
		}
		
		if (gui->state == gui_eng_state_standby)
		{
			ImGui::Text("COMPLETE");
			ImGui::SameLine();
			if (ImGui::Button("CLOSE", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		}
		
		ImGui::Dummy(ImVec2(0, 5));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 5));
		
		ImGui::BeginChild("##scroll_view");
		{
			if (gui->state == gui_eng_state_standby)
			{
				for (int i = 0; i < self->obj_count; ++i)
				{
					s_trj_obj *obj = &self->obj_list[i];
					
					ImGui::PushID(obj);
					
					if (ImGui::CollapsingHeader(obj->desc))
					{
						if (ImPlot::BeginPlot(obj->desc))
						{
							int stride = 10;
							
							ImPlot::PlotLine("pos_error [m]  ",
									&obj->log_list[0].time[0], &obj->log_list[0].pos_error,
									obj->log_offset / stride, 0x00, stride*sizeof(s_trj_obj_data));
							
							ImPlot::PlotLine("rot_error [rad]",
									&obj->log_list[0].time[0], &obj->log_list[0].rot_error,
									obj->log_offset / stride, 0x00, stride*sizeof(s_trj_obj_data));
							
							ImPlot::EndPlot();
						}
					}
					
					ImGui::PopID();
				}
			}
		}
		ImGui::EndChild();
		
		ImGui::EndPopup();
	}
	
	// Always center this window when appearing
	switch (gui->state)
	{
		case gui_eng_state_init:
		{
			ImGui::OpenPopup(window_name);
			
			break;
		}
		
		default: { break; }
	}
	
	return 0x00;
}

//----------------------------------------------------------------

#endif /* __GUI_ENG__ */
