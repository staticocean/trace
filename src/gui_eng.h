
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_ENG__
#define __GUI_ENG__

//------------------------------------------------------------------------------

#include <libgui/imgui/imgui.h>
#include <libgui/implot/implot.h>

#include <sftlstd/vl.h>
#include <libcommon/imgui_w.h>

#include <sftltrc/trc_api.h>
#include <sftltrc/trceng.h>
#include <sftltrc/trctraj.h>
#include <sftltrc/trcctrl.h>
#include <sftltrc/trcdata.h>
#include <sftltrc/trcellp.h>

#include <libgui/clip/clip.h>

#include "gui_obj.h"
#include "gui_traj.h"
#include "gui_clip.h"

//------------------------------------------------------------------------------

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
    s_gui_clip *gui_clip;
	gui_eng_state_t state;
	
	void* sel_item;
	gui_eng_type_t sel_type;

	s_gui_obj *obj_ls;
	
}	s_gui_eng;

typedef struct gui_eng_init
{
	s_gui_obj *obj_ls;
	s_gui_clip *gui_clip;

} 	s_gui_eng_init;

//------------------------------------------------------------------------------

inline u8_t gui_eng_init(s_gui_eng *gui, s_gui_eng_init attr)
{
    gui->gui_clip = attr.gui_clip;
	gui->state = gui_eng_state_standby;
	
	gui->sel_item = NULL;
	gui->sel_type = gui_eng_type_obj;
	gui->obj_ls = attr.obj_ls;
	
	for (int i = 0; i < 255; ++i)
	{
		gui->obj_ls[i].hide = 0x00;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline void gui_eng_sel_obj(s_gui_eng *gui, s_gui_obj *obj)
{
	gui->sel_type = gui_eng_type_obj;
	gui->sel_item = obj;
}

inline void gui_eng_sel_traj(s_gui_eng *gui, s_trctraj *traj)
{
	gui->sel_type = gui_eng_type_traj;
	gui->sel_item = traj;
}

inline void gui_eng_sel_ctrl(s_gui_eng *gui, s_trcctrl *ctrl)
{
	gui->sel_type = gui_eng_type_ctrl;
	gui->sel_item = ctrl;
}

//void gui_eng_sel_proc(s_gui_eng *gui, void *proc)
//{
//	gui->sel_type = gui_eng_type_proc;
//	gui->sel_item = proc;
//}

inline void gui_eng_sel_data(s_gui_eng *gui, s_trcdata *data)
{
	gui->sel_type = gui_eng_type_data;
	gui->sel_item = data;
}

//------------------------------------------------------------------------------

inline u8_t gui_eng_objlist(s_gui_eng *gui, s_trceng *self)
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
	
	for (int i = 0; i < self->obj_sz; i++)
	{
		ImGui::PushID(&gui->obj_ls[i]);
		
		s_gui_obj *obj_gui = &gui->obj_ls[i];
		s_trcobj *obj = obj_gui->ref;
		
		if (filter.PassFilter(obj->desc))
		{
			bool node_sel = gui->sel_item == &gui->obj_ls[i];
			
			ImGui::AlignTextToFramePadding();
			bool node_open = ImGui::TreeNodeEx((void*) i,
											   (node_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											   | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
											   (char*) obj->desc);
			
			if (ImGui::BeginPopupContextItem("obj_options"))
			{
                // if (ImGui::Selectable("copy"))
                // {
                // }

                // if (ImGui::Selectable("paste"))
                // {
                // }

                if (ImGui::Selectable("duplicate"))
                {
                    s_trcobj_init obj_attr;
                    sprintf(obj_attr.desc, "%s [D]", obj->desc);

                    s_trcobj *obj_copy = trceng_add_obj(self, obj_attr);


                    trcobj_copy(self, obj_copy, obj);
                }

				if (ImGui::Selectable("delete"))
				{
					if (gui->sel_item == obj_gui)
					{ gui->sel_item = NULL; }
					
					trceng_del_obj(self, i);
				}
				
				ImGui::EndPopup();
			}
			
			if(ImGui::IsItemClicked()) { gui_eng_sel_obj(gui, obj_gui); }
			
			if (node_open)
			{
				{
					ImGui::AlignTextToFramePadding();
					bool traj_open = ImGui::TreeNodeEx("traj");
					
					if (self->traj_sz > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_traj"))
						{ ImGui::OpenPopup("add_traj_popup"); }
						
						if (ImGui::BeginPopup("add_traj_popup"))
						{
							for (int j = 0; j < self->traj_sz; ++j)
							{
								ImGui::PushID(&self->traj_ls[j]);
								ImGui::Selectable(self->traj_ls[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trcobj_add_traj(obj, self->traj_ls[j]);
								}
								
								ImGui::PopID();
							}
							
							ImGui::EndPopup();
						}
					}
					
					if (traj_open)
					{
						if (obj->traj_sz == 0x00)
						{ ImGui::Text("[no items]"); }
						
						for (int j = 0; j < obj->traj_sz; ++j)
						{
							ImGui::PushID(&obj->traj_ls[j]);
							
							bool traj_sel = gui->sel_item == &obj->traj_ls[j];
							
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx(obj->traj_ls[j].name,
											  (traj_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->traj_ls[j].name);
							
							if (ImGui::IsItemClicked())
							{ gui_eng_sel_traj(gui, &obj->traj_ls[j]); }
							
							if (ImGui::BeginPopupContextItem("traj_menu"))
							{
                                if (ImGui::Selectable("copy ")) { gui_clip_set_traj(gui->gui_clip, self, &obj->traj_ls[j]); }
                                if (ImGui::Selectable("paste")) { gui_clip_get_traj(gui->gui_clip, self, &obj->traj_ls[j]); }

								if (ImGui::Selectable("delete"))
								{
									if (gui->sel_item == &obj->traj_ls[j])
									{ gui->sel_item = NULL; }
									
									trcobj_del_traj(obj, &obj->traj_ls[j]);
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
					
					if (self->ctrl_sz > 0x00)
					{
						ImGui::SameLine();
						
						if (ImGui::SmallButton("add##add_ctrl"))
						{ ImGui::OpenPopup("add_ctrl_popup"); }
						
						if (ImGui::BeginPopup("add_ctrl_popup"))
						{
							for (int j = 0; j < self->ctrl_sz; ++j)
							{
								ImGui::PushID(&self->ctrl_ls[j]);
								ImGui::Selectable(self->ctrl_ls[j].desc);
								
								if (ImGui::IsItemClicked())
								{
									trcobj_add_ctrl(obj, self->ctrl_ls[j]);
								}
								
								ImGui::PopID();
							}
							
							ImGui::EndPopup();
						}
					}
					
					if (ctrl_open)
					{
						if (obj->ctrl_sz == 0x00)
						{ ImGui::Text("[no items]"); }
						
						for (int j = 0; j < obj->ctrl_sz; ++j)
						{
							ImGui::PushID(&obj->ctrl_ls[j]);
							
							bool ctrl_sel = gui->sel_item == &obj->ctrl_ls[j];
							
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx(obj->ctrl_ls[j].name,
											  (ctrl_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->ctrl_ls[j].name);
							
							if (ImGui::IsItemClicked())
							{ gui_eng_sel_ctrl(gui, &obj->ctrl_ls[j]); }
							
							if (ImGui::BeginPopupContextItem("ctrl_menu"))
							{
								if (ImGui::Selectable("delete"))
								{
									if (gui->sel_item == &obj->ctrl_ls[j])
									{ gui->sel_item = NULL; }
									
									trcobj_del_ctrl(obj, &obj->ctrl_ls[j]);
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

					if (self->data_sz > 0x00)
					{
						ImGui::SameLine();

						if (ImGui::SmallButton("add##add_data"))
						{ ImGui::OpenPopup("add_data_popup"); }

						if (ImGui::BeginPopup("add_data_popup"))
						{
							for (int j = 0; j < self->data_sz; ++j)
							{
								ImGui::PushID(&self->data_ls[j]);
								ImGui::Selectable(self->data_ls[j].desc);

								if (ImGui::IsItemClicked())
								{ trcobj_add_data(obj, self->data_ls[j]); }

								ImGui::PopID();
							}

							ImGui::EndPopup();
						}
					}

					if (data_open)
					{
						if (obj->data_sz == 0x00)
						{ ImGui::Text("[no items]"); }

						for (int j = 0; j < obj->data_sz; ++j)
						{
							ImGui::PushID(&obj->data_ls[j]);

							bool data_sel = gui->sel_item == &obj->data_ls[j];
							
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx(obj->data_ls[j].name,
											  (data_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
											  | ImGuiTreeNodeFlags_Leaf, obj->data_ls[j].name);

							if (ImGui::IsItemClicked())
							{ gui_eng_sel_data(gui, &obj->data_ls[j]); }
							
							if (ImGui::BeginPopupContextItem("data_menu"))
							{
								if (ImGui::Selectable("delete"))
								{
									if (gui->sel_item == &obj->data_ls[j])
									{ gui->sel_item = NULL; }

									trcobj_del_data(obj, &obj->data_ls[j]);
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

//------------------------------------------------------------------------------

inline u8_t gui_eng_updateeng(s_gui_eng *gui, s_trceng *self)
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
			
			trceng_reset(self);
			
			self->time_iter = self->time_limit / self->time_step;
			
			for (int i = 0; i < self->obj_sz; ++i)
			{
				if (self->obj_ls[i].log_ls != NULL)
				{ free(self->obj_ls[i].log_ls); }
				
				self->obj_ls[i].log_ls = (s_trcobj_data*) malloc(sizeof(s_trcobj_data) * (self->time_iter+1));
				self->obj_ls[i].log_sz = 0x00;
			}
			
			trceng_log(self);
			
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
				trceng_update(self, self->time_step);
				trceng_log(self);
			}
			
			else { gui->state = gui_eng_state_proc; }
			
			break;
		}
		
		case gui_eng_state_proc:
		{
			if (self->proc_count < self->update_count)
			{
				trceng_proc(self);
			}
			else { gui->state = gui_eng_state_deinit; }
			
			break;
		}
		
		case gui_eng_state_deinit:
		{
			// run data render routines
			trceng_render(self);
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

//------------------------------------------------------------------------------

inline u8_t gui_eng_updategui(s_gui_eng *gui, s_trceng *self)
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
			ImGui::Text("PROC PASS %02.0f%%", 100 * (f64_t) self->proc_count / self->update_count);
			ImGui::SameLine();
			if (ImGui::Button("INTERRUPT", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::Dummy(ImVec2(0, 5));
			imgui_loadbar("##progress", (f64_t) self->proc_count / self->update_count, ImVec2(-1, 6), bg, col);
		}
		
		if (gui->state == gui_eng_state_deinit)
		{
			ImGui::Text("DATA PASS %02.0f%%", 100 * (f64_t) self->proc_count / self->update_count);
			ImGui::SameLine();
			if (ImGui::Button("INTERRUPT", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::Dummy(ImVec2(0, 5));
			imgui_loadbar("##progress", (f64_t) self->proc_count / self->update_count, ImVec2(-1, 6), bg, col);
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
				for (int i = 0; i < self->obj_sz; ++i)
				{
					s_trcobj *obj = &self->obj_ls[i];
					
					ImGui::PushID(obj);
					
					if (ImGui::CollapsingHeader(obj->desc))
					{
						if (ImPlot::BeginPlot(obj->desc))
						{
							int stride = 10;
							
							ImPlot::PlotLine("pos_error [m]  ",
									&obj->log_ls[0].time[0], &obj->log_ls[0].pos_error,
									obj->log_sz / stride, 0x00, stride*sizeof(s_trcobj_data));
							
							ImPlot::PlotLine("rot_error [rad]",
									&obj->log_ls[0].time[0], &obj->log_ls[0].rot_error,
									obj->log_sz / stride, 0x00, stride*sizeof(s_trcobj_data));
							
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

//------------------------------------------------------------------------------

#endif /* __GUI_ENG__ */
