
#include "trj_gui_traj.h"

//------------------------------------------------------------------------------

static const float NODE_SLOT_RADIUS = 4.0f;

ImVec2 transform(s_view_data *view, ImVec2 pos)
{
	float32_t x = view->rect.GetWidth()  * (pos.x - view->p0.x) / (view->width);
	float32_t y = view->rect.GetHeight() - view->rect.GetHeight() * (pos.y - view->p0.y) / (view->height);
	
	return ImVec2(x + view->rect.Min.x, y + view->rect.Min.y);
}

ImVec2 inv_transform(s_view_data *view, ImVec2 pos)
{
	ImVec2 temp = ImVec2(pos.x - view->rect.Min.x, pos.y - view->rect.Min.y);
	
	float32_t x = temp.x * view->width / view->rect.GetWidth()  + view->p0.x;
	float32_t y = (- temp.y + view->rect.GetHeight()) * view->height / view->rect.GetHeight() + view->p0.y;

	return ImVec2(x, y);
}

int CurveEditor(const char* label
		, float* values
		, int points_count
		, ImVec2 size
		, ImU32 flags
		, int* new_count)
{
	s_view_data view;
	
	ImGuiContext& context = *ImGui::GetCurrentContext();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	
	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
	
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	ImGuiID id = parent_window->GetID(label);
	
	ImGui::BeginChildFrame(id, size,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	bool view_hovered = ImGui::IsWindowHovered();
//	bool view_drag = ImGui::IsMouseDragging();
	
	ImVec2 points_min = ImVec2(+FLT_MAX, +FLT_MAX);
	ImVec2 points_max = ImVec2(-FLT_MAX, -FLT_MAX);

	for (int point_idx = 0; point_idx < points_count; ++point_idx)
	{
		ImVec2 point;
		if (flags & (int) CurveEditorFlags::NO_TANGENTS)
		{
			point = ((ImVec2*)values)[point_idx];
		}
		else
		{
			point = ((ImVec2*)values)[1 + point_idx * 3];
		}
		
		points_max = ImMax(points_max, point);
		points_min = ImMin(points_min, point);
	}
	
	points_max.y = ImMax(points_max.y, points_min.y + 0.0001f);
	
	view.int_drag = window->StateStorage.GetBool((ImGuiID) st_int_drag, false);
	view.int_drag_d.x = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dx, 0.0);
	view.int_drag_d.y = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dy, 0.0);
	
	view.p0.x = window->StateStorage.GetFloat((ImGuiID) st_view_p0_x, points_min.x);
	view.p0.y = window->StateStorage.GetFloat((ImGuiID) st_view_p0_y, points_min.y);
	
	view.p1.x = window->StateStorage.GetFloat((ImGuiID) st_view_p1_x, points_max.x);
	view.p1.y = window->StateStorage.GetFloat((ImGuiID) st_view_p1_y, points_max.y);
	
	view.rect = window->InnerRect;

	view.width  = (view.p1 - view.p0).x;
	view.height = (view.p1 - view.p0).y;
	
	if (view.width < 1E-6)
	{
		view.p0.x -= 1.0;
		view.p1.x += 1.0;
	}
	
	if (view.height < 1E-6)
	{
		view.p0.y -= 1.0;
		view.p1.y += 1.0;
	}
	
	view.width  = (view.p1 - view.p0).x;
	view.height = (view.p1 - view.p0).y;
	
//	ImVec2 mouse_point = inv_transform(&view, ImGui::GetMousePos());
//	window->DrawList->AddCircle(transform(&view, mouse_point), 4, 0x55000000);
//	window->DrawList->AddCircle(ImGui::GetMousePos(), 8, 0x55000000);
	
	if (io.MouseWheel != 0x00 && view_hovered && !view.int_drag)
	{
//		ImVec2 scale_point = (view.p0 + view.p1) * 0.5;
		ImVec2 scale_point = inv_transform(&view, ImGui::GetMousePos());
		float32_t scroll_delta = io.MouseWheel * 0.1f;
		
		if (scroll_delta > +0.5) scroll_delta = +0.5;
		if (scroll_delta < -0.5) scroll_delta = -0.5;
		
		view.p0 = scale_point + (view.p0 - scale_point) * (1.0f - scroll_delta);
		view.p1 = scale_point + (view.p1 - scale_point) * (1.0f - scroll_delta);
	}
	
	if (view_hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		ImVec2 mouse_drag_delta = ImVec2(
				io.MouseDelta.x * view.width / view.rect.GetWidth(),
				io.MouseDelta.y * view.height / view.rect.GetHeight()
		);
		
		mouse_drag_delta.y = -mouse_drag_delta.y;
		
		view.p0 -= mouse_drag_delta;
		view.p1 -= mouse_drag_delta;
	}
	
	view.width  = (view.p1 - view.p0).x;
	view.height = (view.p1 - view.p0).y;
	view.rect = window->InnerRect;
	
	if (flags & (int) CurveEditorFlags::SHOW_GRID)
	{
		char axis_label[64];
		int32_t exp;
		
		frexp(view.width / 5, &exp);
		float step_x = (float) ldexp(1.0, exp);
		
		for (int i = 0; i < int(view.width / step_x) + 2; ++i)
		{
			ImVec2 a = transform(&view, ImVec2(view.p0.x + i * step_x, view.p0.y));
			ImVec2 b = transform(&view, ImVec2(view.p0.x + i * step_x, view.p1.y));
			
			window->DrawList->AddLine(a, b, col_textdis_u32);
			
			if (exp > 0) { ImFormatString(axis_label, sizeof(axis_label), " %d", int(view.p0.x + i * step_x)); }
			else { ImFormatString(axis_label, sizeof(axis_label), " %f", view.p0.x + i * step_x); }
			
			window->DrawList->AddText(b, col_text_u32, axis_label);
		}
		
		frexp(view.height / 10, &exp);
		float step_y = (float) ldexp(1.0, exp);
		
		for (int i = 0; i < int(view.height / step_y) + 2; ++i)
		{
			ImVec2 a = transform(&view, ImVec2(view.p0.x, view.p0.y + i * step_y));
			ImVec2 b = transform(&view, ImVec2(view.p1.x, view.p0.y + i * step_y));
			
			window->DrawList->AddLine(a, b, col_textdis_u32);
			
			if (exp > 0) { ImFormatString(axis_label, sizeof(axis_label), " %d", int(view.p0.y + i * step_y)); }
			else { ImFormatString(axis_label, sizeof(axis_label), " %f", view.p0.y + i * step_y); }
			
			window->DrawList->AddText(a, col_text_u32, axis_label);
		}
	}
	
	ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

	for (int i = 0; i < points_count; ++i)
	{
		ImGui::PushID(i);
		
		ImVec2 point_pos = ImVec2(values[2*i+0], values[2*i+1]);
		ImGui::SetCursorScreenPos(transform(&view, point_pos) - ImVec2(8, 8));
		
		ImGui::InvisibleButton("##pt", ImVec2(16, 16));
		
		ImVec2 center = transform(&view, point_pos);
		
		if (ImGui::IsItemActive() || ImGui::IsItemHovered())
		{
			window->DrawList->AddCircle(center, 4, col_textdis_u32);
		}
		
		else
		{
			window->DrawList->AddCircle(center, 4, col_text_u32);
		}
		
		if (ImGui::IsItemActive())
		{
			ImVec2 mouse_drag_delta = ImVec2(
					io.MouseDelta.x * view.width / view.rect.GetWidth(),
					io.MouseDelta.y * view.height / view.rect.GetHeight()
			);
			
			mouse_drag_delta.y = -mouse_drag_delta.y;
			point_pos += mouse_drag_delta;
		}
		
		values[2*i+0] = point_pos.x;
		values[2*i+1] = point_pos.y;
		
		ImGui::PopID();
	}
	
	ImGui::SetCursorScreenPos(cursor_pos);
	

//	int changed_idx = -1;
//
//	auto invTransform = [&](const ImVec2& pos) -> ImVec2
//	{
//		float x = (pos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x);
//		float y = (inner_bb.Max.y - pos.y) / (inner_bb.Max.y - inner_bb.Min.y);
//
//		return ImVec2(
//				from_x + width * x,
//				from_y + height * y
//		);
//	};
//

//	if (ImGui::IsMouseReleased(1))
//	{
//		window->StateStorage.SetBool((ImGuiID)StorageValues::IS_PANNING, false);
//	}
//	if (window->StateStorage.GetBool((ImGuiID)StorageValues::IS_PANNING, false))
//	{
//		ImVec2 drag_offset = ImGui::GetMouseDragDelta(1);
//		from_x = start_pan.x;
//		from_y = start_pan.y;
//		from_x -= drag_offset.x * width / (inner_bb.Max.x - inner_bb.Min.x);
//		from_y += drag_offset.y * height / (inner_bb.Max.y - inner_bb.Min.y);
//		window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_X, from_x);
//		window->StateStorage.SetFloat((ImGuiID)StorageValues::FROM_Y, from_y);
//	}
//	else if (ImGui::IsMouseDragging(1) && ImGui::IsItemHovered())
//	{
//		window->StateStorage.SetBool((ImGuiID)StorageValues::IS_PANNING, true);
//		start_pan.x = from_x;
//		start_pan.y = from_y;
//	}
//
//	for (int point_idx = points_count - 2; point_idx >= 0; --point_idx)
//	{
//		ImVec2* points;
//		if (flags & (int)CurveEditorFlags::NO_TANGENTS)
//		{
//			points = ((ImVec2*)values) + point_idx;
//		}
//		else
//		{
//			points = ((ImVec2*)values) + 1 + point_idx * 3;
//		}
//
//		ImVec2 p_prev = points[0];
//		ImVec2 tangent_last;
//		ImVec2 tangent;
//		ImVec2 p;
//		if (flags & (int)CurveEditorFlags::NO_TANGENTS)
//		{
//			p = points[1];
//		}
//		else
//		{
//			tangent_last = points[1];
//			tangent = points[2];
//			p = points[3];
//		}
//
//		auto handlePoint = [&](ImVec2& p, int idx) -> bool
//		{
//			static const float SIZE = 3;
//
//			ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
//			ImVec2 pos = transform(p);
//
//			ImGui::SetCursorScreenPos(pos - ImVec2(SIZE, SIZE));
//			ImGui::PushID(idx);
//			ImGui::InvisibleButton("", ImVec2(2 * NODE_SLOT_RADIUS, 2 * NODE_SLOT_RADIUS));
//
//			ImU32 col = ImGui::IsItemActive() || ImGui::IsItemHovered() ? ImGui::GetColorU32(ImGuiCol_PlotLinesHovered) : ImGui::GetColorU32(ImGuiCol_PlotLines);
//
//			window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, SIZE), col);
//			window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, SIZE), col);
//			window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, -SIZE), col);
//			window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, -SIZE), col);
//
//			if (ImGui::IsItemHovered()) hovered_idx = point_idx + idx;
//
//			bool changed = false;
//			if (ImGui::IsItemActive() && ImGui::IsMouseClicked(0))
//			{
//				window->StateStorage.SetFloat((ImGuiID)StorageValues::POINT_START_X, pos.x);
//				window->StateStorage.SetFloat((ImGuiID)StorageValues::POINT_START_Y, pos.y);
//			}
//
//			if (ImGui::IsItemHovered() || ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
//			{
//				char tmp[64];
//				ImFormatString(tmp, sizeof(tmp), "%0.2f, %0.2f", p.x, p.y);
//				window->DrawList->AddText({ pos.x, pos.y - ImGui::GetTextLineHeight() }, 0xff000000, tmp);
//			}
//
//			if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
//			{
//				pos.x = window->StateStorage.GetFloat((ImGuiID)StorageValues::POINT_START_X, pos.x);
//				pos.y = window->StateStorage.GetFloat((ImGuiID)StorageValues::POINT_START_Y, pos.y);
//				pos += ImGui::GetMouseDragDelta();
//				ImVec2 v = invTransform(pos);
//
//				p = v;
//				changed = true;
//			}
//			ImGui::PopID();
//
//			ImGui::SetCursorScreenPos(cursor_pos);
//			return changed;
//		};
//
//		auto handleTangent = [&](ImVec2& t, const ImVec2& p, int idx) -> bool
//		{
//			static const float SIZE = 2;
//			static const float LENGTH = 18;
//
//			auto normalized = [](const ImVec2& v) -> ImVec2
//			{
//				float len = 1.0f / sqrtf(v.x *v.x + v.y * v.y);
//				return ImVec2(v.x * len, v.y * len);
//			};
//
//			ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
//			ImVec2 pos = transform(p);
//			ImVec2 tang = pos + normalized(ImVec2(t.x, -t.y)) * LENGTH;
//
//			ImGui::SetCursorScreenPos(tang - ImVec2(SIZE, SIZE));
//			ImGui::PushID(-idx);
//			ImGui::InvisibleButton("", ImVec2(2 * NODE_SLOT_RADIUS, 2 * NODE_SLOT_RADIUS));
//
//			window->DrawList->AddLine(pos, tang, ImGui::GetColorU32(ImGuiCol_PlotLines));
//
//			ImU32 col = ImGui::IsItemHovered() ? ImGui::GetColorU32(ImGuiCol_PlotLinesHovered) : ImGui::GetColorU32(ImGuiCol_PlotLines);
//
//			window->DrawList->AddLine(tang + ImVec2(-SIZE, SIZE), tang + ImVec2(SIZE, SIZE), col);
//			window->DrawList->AddLine(tang + ImVec2(SIZE, SIZE), tang + ImVec2(SIZE, -SIZE), col);
//			window->DrawList->AddLine(tang + ImVec2(SIZE, -SIZE), tang + ImVec2(-SIZE, -SIZE), col);
//			window->DrawList->AddLine(tang + ImVec2(-SIZE, -SIZE), tang + ImVec2(-SIZE, SIZE), col);
//
//			bool changed = false;
//			if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
//			{
//				tang = ImGui::GetIO().MousePos - pos;
//				tang = normalized(tang);
//				tang.y *= -1;
//
//				t = tang;
//				changed = true;
//			}
//			ImGui::PopID();
//
//			ImGui::SetCursorScreenPos(cursor_pos);
//			return changed;
//		};
//
//		ImGui::PushID(point_idx);
//		if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
//		{
//			window->DrawList->AddBezierCurve(
//					transform(p_prev),
//					transform(p_prev + tangent_last),
//					transform(p + tangent),
//					transform(p),
//					ImGui::GetColorU32(ImGuiCol_PlotLines),
//					1.0f,
//					20);
//			if (handleTangent(tangent_last, p_prev, 0))
//			{
//				points[1] = ImClamp(tangent_last, ImVec2(0, -1), ImVec2(1, 1));
//				changed_idx = point_idx;
//			}
//			if (handleTangent(tangent, p, 1))
//			{
//				points[2] = ImClamp(tangent, ImVec2(-1, -1), ImVec2(0, 1));
//				changed_idx = point_idx + 1;
//			}
//			if (handlePoint(p, 1))
//			{
//				if (p.x <= p_prev.x) p.x = p_prev.x + 0.001f;
//				if (point_idx < points_count - 2 && p.x >= points[6].x)
//				{
//					p.x = points[6].x - 0.001f;
//				}
//				points[3] = p;
//				changed_idx = point_idx + 1;
//			}
//
//		}
//		else
//		{
//			window->DrawList->AddLine(transform(p_prev), transform(p), ImGui::GetColorU32(ImGuiCol_PlotLines), 1.0f);
//			if (handlePoint(p, 1))
//			{
//				if (p.x <= p_prev.x) p.x = p_prev.x + 0.001f;
//				if (point_idx < points_count - 2 && p.x >= points[2].x)
//				{
//					p.x = points[2].x - 0.001f;
//				}
//				points[1] = p;
//				changed_idx = point_idx + 1;
//			}
//		}
//		if (point_idx == 0)
//		{
//			if (handlePoint(p_prev, 0))
//			{
//				if (p.x <= p_prev.x) p_prev.x = p.x - 0.001f;
//				points[0] = p_prev;
//				changed_idx = point_idx;
//			}
//		}
//		ImGui::PopID();
//	}
//
//	ImGui::SetCursorScreenPos(inner_bb.Min);
//
//	ImGui::InvisibleButton("bg", inner_bb.Max - inner_bb.Min);
//
//	if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0) && new_count)
//	{
//		ImVec2 mp = ImGui::GetMousePos();
//		ImVec2 new_p = invTransform(mp);
//		ImVec2* points = (ImVec2*)values;
//
//		if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
//		{
//			points[points_count * 3 + 0] = ImVec2(-0.2f, 0);
//			points[points_count * 3 + 1] = new_p;
//			points[points_count * 3 + 2] = ImVec2(0.2f, 0);;
//			++*new_count;
//
//			auto compare = [](const void* a, const void* b) -> int
//			{
//				float fa = (((const ImVec2*)a) + 1)->x;
//				float fb = (((const ImVec2*)b) + 1)->x;
//				return fa < fb ? -1 : (fa > fb) ? 1 : 0;
//			};
//
//			qsort(values, points_count + 1, sizeof(ImVec2) * 3, compare);
//
//		}
//		else
//		{
//			points[points_count] = new_p;
//			++*new_count;
//
//			auto compare = [](const void* a, const void* b) -> int
//			{
//				float fa = ((const ImVec2*)a)->x;
//				float fb = ((const ImVec2*)b)->x;
//				return fa < fb ? -1 : (fa > fb) ? 1 : 0;
//			};
//
//			qsort(values, points_count + 1, sizeof(ImVec2), compare);
//		}
//	}
//
//	if (hovered_idx >= 0 && ImGui::IsMouseDoubleClicked(0) && new_count && points_count > 2)
//	{
//		ImVec2* points = (ImVec2*)values;
//		--*new_count;
//		if ((flags & (int)CurveEditorFlags::NO_TANGENTS) == 0)
//		{
//			for (int j = hovered_idx * 3; j < points_count * 3 - 3; j += 3)
//			{
//				points[j + 0] = points[j + 3];
//				points[j + 1] = points[j + 4];
//				points[j + 2] = points[j + 5];
//			}
//		}
//		else
//		{
//			for (int j = hovered_idx; j < points_count - 1; ++j)
//			{
//				points[j] = points[j + 1];
//			}
//		}
//	}

//	if (!view.int_drag)
	window->StateStorage.SetFloat((ImGuiID) st_view_p0_x, view.p0.x);
	window->StateStorage.SetFloat((ImGuiID) st_view_p0_y, view.p0.y);
	
	window->StateStorage.SetFloat((ImGuiID) st_view_p1_x, view.p1.x);
	window->StateStorage.SetFloat((ImGuiID) st_view_p1_y, view.p1.y);
	
	window->StateStorage.SetBool((ImGuiID) st_int_drag_dx, view.int_drag_d.x);
	window->StateStorage.SetBool((ImGuiID) st_int_drag_dy, view.int_drag_d.y);
	
//	ImGui::Text("%f,%f %f,%f",
//				view.p0.x, view.p0.y,
//				view.p1.x, view.p1.y);
//
//	ImVec2 p0 = transform(&view, view.p0);
//	ImVec2 p1 = transform(&view, view.p1);
//
//	ImGui::Text("%f,%f %f,%f",
//				p0.x, p0.y,
//				p1.x, p1.y);
//
//	ImGui::Text("%f,%f %f,%f",
//				view.rect.Min.x, view.rect.Min.y,
//				view.rect.Max.x, view.rect.Max.y);
//
//	ImGui::Text("%f", io.MouseWheel);
	
	ImGui::EndChildFrame();
	
//	const ImRect frame_bb(inner_bb.Min - style.FramePadding, inner_bb.Max + style.FramePadding);

	//	ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
	
	return 0x00;
}














//------------------------------------------------------------------------------
