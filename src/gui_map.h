
#ifndef __GUI_MAP__
#define __GUI_MAP__

//----------------------------------------------------------------

#include <fstream>

#include <lib_internal/vl.h>
#include <lib_internal/vl3d.h>
#include <lib_internal/gjson.h>

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>
#include <lib/json11/json11.hpp>

//----------------------------------------------------------------

typedef struct gui_map
{
	std::vector<s_gjson_obj> obj_list;
	
} 	s_gui_map;

//----------------------------------------------------------------

inline ImVec2 __transform__(s_view_data *view, ImVec2 pos)
{
	float32_t x = view->rect.GetWidth()  * (pos.x - view->p0.x) / (view->width);
	float32_t y = view->rect.GetHeight() - view->rect.GetHeight() * (pos.y - view->p0.y) / (view->height);
	
	return ImVec2(x + view->rect.Min.x, y + view->rect.Min.y);
}

inline ImVec2 __inv_transform__(s_view_data *view, ImVec2 pos)
{
	ImVec2 temp = ImVec2(pos.x - view->rect.Min.x, pos.y - view->rect.Min.y);
	
	float32_t x = temp.x * view->width / view->rect.GetWidth()  + view->p0.x;
	float32_t y = (- temp.y + view->rect.GetHeight()) * view->height / view->rect.GetHeight() + view->p0.y;
	
	return ImVec2(x, y);
}

inline uint8_t gui_map_load(s_gui_map *map, char *file_name)
{
	std::ifstream file_stream(file_name);
	std::string file_data((std::istreambuf_iterator<char>(file_stream)),
						  (std::istreambuf_iterator<char>()));
	
	std::string err;
	json11::Json json = json11::Json::parse(file_data.c_str(), err);
	printf(err.c_str());
//	print_json(ml0);
	
//	printf("[type][%s] \r\n", json["type"].string_value().c_str());
//	printf("[size][%d] \r\n", json["features"].array_items().size());

	for (int i = 0; i < json["features"].array_items().size(); ++i)
	{
		map->obj_list.push_back((s_gjson_obj) {});
		s_gjson_obj *obj = &map->obj_list.back();
		
		json11::Json prop = json["features"][i]["properties"];
		json11::Json geom = json["features"][i]["geometry"];
		
		strcpy(obj->name, prop["name"].string_value().c_str());
//		printf("[name][%s] \r\n", obj->name);
//		printf("[gtype][%s] \r\n", geom["type"].string_value().c_str());
//
		if (geom["type"] == "Polygon")
		{
			s_gjson_poly *poly = new s_gjson_poly();
			obj->poly = *poly;
			obj->type = gjson_geom_type_poly;
			free(poly);
//
//			printf("[poly_size][%d]\r\n", geom["coordinates"][0].array_items().size());
//
			for (int k = 0; k < geom["coordinates"].array_items().size(); ++k)
			{
				for (int j = 0; j < geom["coordinates"][k].array_items().size(); ++j)
				{
					obj->poly.pt_list.push_back((s_gjson_pt) {.vec = {
							geom["coordinates"][k][j][0].number_value(),
							geom["coordinates"][k][j][1].number_value(),
							0.0
					}});
				}
			}
			
//			poly->pt_list.push_back((s_gjson_pt) { .vec = (s_gjson_poly)});
		}
	}
	
//	for (int i = 0; i < map->obj_list.size(); ++i)
//	{
//		printf("[name][%s]\r\n", map->obj_list[i].name);
//		printf("[size][%d]\r\n", map->obj_list[i].poly.pt_list.size());
//
//		for (int j = 0; j < map->obj_list[i].poly.pt_list.size(); ++j)
//		{
//			printf("[pt][%f][%f] \r\n",
//					map->obj_list[i].poly.pt_list[j].vec[0],
//				   	map->obj_list[i].poly.pt_list[j].vec[1]
//				   	);
//		}
//	}
	
	//	printf("%d\r\n", ml0.is_object());
	//	printf("%d\r\n", ml0.is_string());
	//	printf("%d\r\n", ml0.is_array());
	//	printf("%d\r\n", ml0.is_bool());
	//	printf("%d\r\n", ml0.is_null());
	//	printf("%d\r\n", ml0.is_number());
	
	fflush(stdout);
	
	return 0x00;
}

inline void gui_map_view(s_gui_map *self, char* label, ImVec2 size)
{
	s_view_data view_top;
	s_view_data view_bot;
	
	ImGuiContext& context = *ImGui::GetCurrentContext();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	
	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
	
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	
	size.x = size.x < 0 ? ImGui::GetContentRegionAvail().x : size.x;
	size.y = size.y < 0 ? ImGui::GetContentRegionAvail().y : size.y;
	
	{
		ImGui::PushID("view_top");
		
		ImGuiID id = parent_window->GetID(label);
		
		ImGui::BeginChildFrame(id, ImVec2(size.x, size.y * 0.8),
							   ImGuiWindowFlags_NoScrollbar |
							   ImGuiWindowFlags_NoScrollWithMouse);
		
		ImGuiWindow *window = ImGui::GetCurrentWindow();
		
		bool view_hovered = ImGui::IsWindowHovered();
		
		ImVec2 points_min = ImVec2(-1.0, -1.0);
		ImVec2 points_max = ImVec2(+1.0, +1.0);
		
		points_max.y = ImMax(points_max.y, points_min.y + 0.0001f);
		
		view_top.int_drag = window->StateStorage.GetBool((ImGuiID) st_int_drag, false);
		view_top.int_drag_d.x = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dx, 0.0);
		view_top.int_drag_d.y = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dy, 0.0);
		
		view_top.p0.x = window->StateStorage.GetFloat((ImGuiID) st_view_p0_x, points_min.x);
		view_top.p0.y = window->StateStorage.GetFloat((ImGuiID) st_view_p0_y, points_min.y);
		
		view_top.p1.x = window->StateStorage.GetFloat((ImGuiID) st_view_p1_x, points_max.x);
		view_top.p1.y = window->StateStorage.GetFloat((ImGuiID) st_view_p1_y, points_max.y);
		
		view_top.rect = window->InnerRect;
		
		view_top.width = (view_top.p1 - view_top.p0).x;
		view_top.height = (view_top.p1 - view_top.p0).y;
		
		if (view_top.width < 1E-6)
		{
			view_top.p0.x -= 1.0;
			view_top.p1.x += 1.0;
		}
		
		if (view_top.height < 1E-6)
		{
			view_top.p0.y -= 1.0;
			view_top.p1.y += 1.0;
		}
		
		view_top.width = (view_top.p1 - view_top.p0).x;
		view_top.height = (view_top.p1 - view_top.p0).y;
		
		if (io.MouseWheel != 0x00 && view_hovered && !view_top.int_drag)
		{
			ImVec2 scale_point = __inv_transform__(&view_top, ImGui::GetMousePos());
			float32_t scroll_delta = io.MouseWheel * 0.1f;
			
			if (scroll_delta > +0.5) scroll_delta = +0.5;
			if (scroll_delta < -0.5) scroll_delta = -0.5;
			
			view_top.p0 = scale_point + (view_top.p0 - scale_point) * (1.0f - scroll_delta);
			view_top.p1 = scale_point + (view_top.p1 - scale_point) * (1.0f - scroll_delta);
		}
		
		if (view_hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			ImVec2 mouse_drag_delta = ImVec2(
					io.MouseDelta.x * view_top.width / view_top.rect.GetWidth(),
					io.MouseDelta.y * view_top.height / view_top.rect.GetHeight()
			);
			
			mouse_drag_delta.y = -mouse_drag_delta.y;
			
			view_top.p0 -= mouse_drag_delta;
			view_top.p1 -= mouse_drag_delta;
		}
		
		view_top.width = (view_top.p1 - view_top.p0).x;
		view_top.height = (view_top.p1 - view_top.p0).y;
		view_top.rect = window->InnerRect;
		
		// draw grid TODO:: replace with rectangles coz they are drawn faster on GPUs
		{
			char axis_label[64];
			int32_t exp;
			
			frexp(view_top.width / 5, &exp);
			float step_x = (float) ldexp(1.0, exp);
			
			for (int i = 0; i < int(view_top.width / step_x) + 2; ++i)
			{
				ImVec2 a = __transform__(&view_top, ImVec2(view_top.p0.x + i * step_x, view_top.p0.y));
				ImVec2 b = __transform__(&view_top, ImVec2(view_top.p0.x + i * step_x, view_top.p1.y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (exp > 0)
				{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_top.p0.x + i * step_x)); }
				else
				{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_top.p0.x + i * step_x); }
				
				window->DrawList->AddText(b, col_text_u32, axis_label);
			}
			
			frexp(view_top.height / 10, &exp);
			float step_y = (float) ldexp(1.0, exp);
			
			for (int i = 0; i < int(view_top.height / step_y) + 2; ++i)
			{
				ImVec2 a = __transform__(&view_top, ImVec2(view_top.p0.x, view_top.p0.y + i * step_y));
				ImVec2 b = __transform__(&view_top, ImVec2(view_top.p1.x, view_top.p0.y + i * step_y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (exp > 0)
				{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_top.p0.y + i * step_y)); }
				else
				{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_top.p0.y + i * step_y); }
				
				window->DrawList->AddText(a, col_text_u32, axis_label);
			}
		}
		
		// draw trajectory
		for (int i = 0; i < self->obj_list.size(); ++i)
		{
			uint32_t size = self->obj_list[i].poly.pt_list.size();
			
			for (int j = 0; j < size; ++j)
			{
				ImVec2 p0 = ImVec2(	self->obj_list[i].poly.pt_list[j % size].vec[0],
									self->obj_list[i].poly.pt_list[j % size].vec[1]);
				
				ImVec2 p1 = ImVec2(	self->obj_list[i].poly.pt_list[(j+1) % size].vec[0],
									   self->obj_list[i].poly.pt_list[(j+1) % size].vec[1]);
				
				window->DrawList->AddLine(__transform__(&view_top, p0), __transform__(&view_top, p1), col_text_u32);
			}
		}
		
		window->StateStorage.SetFloat((ImGuiID) st_view_p0_x, view_top.p0.x);
		window->StateStorage.SetFloat((ImGuiID) st_view_p0_y, view_top.p0.y);
		
		window->StateStorage.SetFloat((ImGuiID) st_view_p1_x, view_top.p1.x);
		window->StateStorage.SetFloat((ImGuiID) st_view_p1_y, view_top.p1.y);
		
		window->StateStorage.SetBool((ImGuiID) st_int_drag_dx, view_top.int_drag_d.x);
		window->StateStorage.SetBool((ImGuiID) st_int_drag_dy, view_top.int_drag_d.y);
		
		ImGui::EndChildFrame();
		
		ImGui::PopID();
	}
	
	return;
}

//----------------------------------------------------------------

#endif /* __GUI_MAP__ */
