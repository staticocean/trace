
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

uint8_t trj_gui_traj_bz(s_trj_traj_bz *self, const char* label, ImVec2 size, bool view_res)
{
	s_view_data view;
	
	ImGuiContext& context = *ImGui::GetCurrentContext();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	
	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
	
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	ImGuiID id = parent_window->GetID(label);
	
	size.x = size.x < 0 ? ImGui::GetContentRegionAvailWidth() : size.x;
//	size.y = size.y < 0 ? HEIGHT : size.y;
	
	ImGui::BeginChildFrame(id, size,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	bool view_hovered = ImGui::IsWindowHovered();
//	bool view_drag = ImGui::IsMouseDragging();
	
	ImVec2 points_min = ImVec2(+FLT_MAX, +FLT_MAX);
	ImVec2 points_max = ImVec2(-FLT_MAX, -FLT_MAX);

	for (int i = 0; i < self->pts_offset; ++i)
	{
		ImVec2 point = ImVec2(self->pts[i].time , self->pts[i].pos_p[0]);
		
		points_max = ImMax(points_max, point);
		points_min = ImMin(points_min, point);
		
		point = ImVec2(
				self->pts[i].pos_p[0] + self->pts[i].pos_d[0],
				self->pts[i].pos_p[2] + self->pts[i].pos_d[2]);
		
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
	
	if (view_res == true)
	{
		view.p0.x = points_min.x;
		view.p0.y = points_min.y;
		
		view.p1.x = points_max.x;
		view.p1.y = points_max.y;
	}
	
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
	
	if (io.MouseWheel != 0x00 && view_hovered && !view.int_drag)
	{
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
	
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		trj_traj_bz_compile(self);
	}
	
	view.width  = (view.p1 - view.p0).x;
	view.height = (view.p1 - view.p0).y;
	view.rect = window->InnerRect;
	
	// draw grid TODO:: replace with rectangles coz they are drawn faster on GPUs
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

	for (int i = 0; i < self->pts_offset; ++i)
	{
		ImGui::PushID(i);
		
		ImVec2 point_pos = ImVec2(self->pts[i].pos_p[0], self->pts[i].pos_p[2]);
		ImGui::SetCursorScreenPos(transform(&view, point_pos) - ImVec2(8, 8));
		
		ImGui::InvisibleButton("##pt", ImVec2(16, 16));
		
		ImVec2 center = transform(&view, point_pos);
		
		if (ImGui::IsItemActive() || ImGui::IsItemHovered())
		{
			window->DrawList->AddCircleFilled(center, 4, col_text_u32);
		}
		
		else
		{
			window->DrawList->AddCircle(center, 4, col_text_u32);
		}
		
		char pt_label[255];
		
		ImFormatString(pt_label, sizeof(pt_label), "%d", i);
		window->DrawList->AddText(center, col_text_u32, pt_label);
		
		if (ImGui::IsItemActive())
		{
			ImVec2 mouse_drag_delta = ImVec2(
					io.MouseDelta.x * view.width / view.rect.GetWidth(),
					io.MouseDelta.y * view.height / view.rect.GetHeight()
			);
			
			mouse_drag_delta.y = -mouse_drag_delta.y;
			point_pos += mouse_drag_delta;
		}
		
		self->pts[i].pos_p[0] = point_pos.x;
		self->pts[i].pos_p[2] = point_pos.y;
		
		ImGui::PopID();
	}
	
	ImGui::SetCursorScreenPos(cursor_pos);
	
	for (int i = 0; i < self->pts_offset; ++i)
	{
		ImGui::PushID(i);
		
		ImVec2 p = transform(&view, ImVec2(
				self->pts[i].pos_p[0],
				self->pts[i].pos_p[2]));
		
		ImVec2 d = transform(&view, ImVec2(
				self->pts[i].pos_p[0] + self->pts[i].pos_d[0],
				self->pts[i].pos_p[2] + self->pts[i].pos_d[2]));
		
//		window->DrawList->AddTriangleFilled(
//				pos_d + ImVec2(-4,0),
//				pos_d + ImVec2(+4,0),
//				pos_d + ImVec2(0,-5.6), col_textdis_u32);
		
		window->DrawList->AddLine(
				d + ImVec2(-4,-4),
				d + ImVec2(+4,+4),
				col_textdis_u32);
		
		window->DrawList->AddLine(
				d + ImVec2(+4,-4),
				d + ImVec2(-4,+4),
				col_textdis_u32);
		
		window->DrawList->AddLine(p, d, col_textdis_u32);
		
		ImGui::PopID();
	}
	
//	vlf_t time_step = (self->pts[self->pts_offset-1].pos_p[0] - self->pts[0].pos_p[0]) / 1000;
//	vlf_t time_step = (view.p1[0] - view.p0[0]) / 1000;
	vlf_t time_step = (self->pts[self->pts_offset-1].time - self->pts[0].time) / 1000.0;
	vlf_t time = self->pts[0].time;
//	vlf_t time = view.p0[0];
	
	for (int i = 0; i < 1000-1; ++i)
	{
		vlf_t p0[3];
		vlf_t p1[3];
		
		trj_traj_bz_pos(self, time, p0);
		trj_traj_bz_pos(self, time + time_step, p1);
		
		ImVec2 p0_ = ImVec2(time, p0[0]);
		ImVec2 p1_ = ImVec2(time + time_step, p1[0]);
		
		window->DrawList->AddLine(transform(&view, p0_), transform(&view, p1_), col_text_u32);
	
		time += time_step;
	}

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
