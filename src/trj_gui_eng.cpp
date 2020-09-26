
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
			
			bool node_sel = gui->sel_item == &self->obj_list[i];
			
			bool node_open = ImGui::TreeNodeEx((void*) i,
					(node_sel ? ImGuiTreeNodeFlags_Selected : 0x00)
					| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick,
					(char*) obj->name);
			
			if(ImGui::IsItemClicked()) { __sel_object__(gui, &self->obj_list[i]); }
			
			ImGui::SameLine();
			char *hide_label[2] = { "hide", "show" };
			if (ImGui::SmallButton(hide_label[gui->obj_hide[i]]))
			{ gui->obj_hide[i] = !gui->obj_hide[i]; }

			if (node_open)
			{
				if (ImGui::TreeNodeEx("traj"))
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
				
				if (ImGui::TreeNodeEx("ctrl"))
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
				
				if (ImGui::TreeNodeEx("proc"))
				{
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("data"))
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
