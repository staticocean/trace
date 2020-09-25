
#include "trj_gui_eng.h"

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
	
	uint8_t* filter_data[self->objects_offset];
	
	for (int i = 0; i < self->objects_offset; ++i)
	{
		filter_data[i] = self->objects[i].name;
	}
	
	filter.Draw("");
	
	ImGui::SameLine();
	ImGui::Button("add");
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10);
	
	for (int i = 0; i < IM_ARRAYSIZE(filter_data); i++)
	{
		ImGui::PushID(i);
		
		if (filter.PassFilter((char*) filter_data[i]))
		{
			s_trj_obj *obj = &self->objects[i];
			bool node_sel = gui->sel_item == &self->objects[i];
			
			bool node_open = ImGui::TreeNodeEx((void*) i,
					(node_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
					| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
					(char*) obj->name);
			
			if(ImGui::IsItemClicked()) { __sel_object__(gui, &self->objects[i]); }
			
			char *hide_label[2] = { "hide", "show" };
			
			ImGui::SameLine();
			if (ImGui::SmallButton(hide_label[gui->obj_hide[i]]))
			{
				gui->obj_hide[i] = !gui->obj_hide[i];
			}

			if (node_open)
			{
				if (ImGui::TreeNodeEx("Traj"))
				{
					if (obj->traj_offset == 0x00)
					{
//						ImGui::
					}
					
					for (int j = 0; j < obj->traj_offset; ++j)
					{
					
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("Ctrl"))
				{
					if (obj->ctrl_offset == 0x00)
					{
//						ImGui::
					}
					
					for (int j = 0; j < obj->ctrl_offset; ++j)
					{
					
					}
					
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("Proc"))
				{
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("Data"))
				{
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


//------------------------------------------------------------------------------
