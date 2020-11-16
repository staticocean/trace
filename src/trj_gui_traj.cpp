
#include "trj_gui_traj.h"

//------------------------------------------------------------------------------

void trj_gui_traj_edit(s_trj_traj *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(-1);
	ImGui::InputText("##name", self->desc, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Columns(2);
	
	ImGui::Text("id");
	ImGui::NextColumn();
	ImGui::Text("[%06d]", self->id);
	ImGui::NextColumn();
	
	ImGui::Columns(1);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

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

void trj_gui_traj_bz(s_trj_traj_bz *self, const char* label, ImVec2 size, bool view_res)
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
		
		ImVec2 points_min = (self->pts_offset == 0x00) ? ImVec2(-1.0, -1.0) : ImVec2(+FLT_MAX, +FLT_MAX);
		ImVec2 points_max = (self->pts_offset == 0x00) ? ImVec2(+1.0, +1.0) : ImVec2(-FLT_MAX, -FLT_MAX);
		
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImVec2 point;
			
			point = ImVec2(self->pts[i].pos_p[0], self->pts[i].pos_p[2]);
			
			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);
			
			point = ImVec2(
					self->pts[i].pos_p[0] + self->pts[i].pos_d[0],
					self->pts[i].pos_p[2] + self->pts[i].pos_d[2]);
			
			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);
		}
		
		points_max.y = ImMax(points_max.y, points_min.y + 0.0001f);
		
		view_top.int_drag = window->StateStorage.GetBool((ImGuiID) st_int_drag, false);
		view_top.int_drag_d.x = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dx, 0.0);
		view_top.int_drag_d.y = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dy, 0.0);
		
		view_top.p0.x = window->StateStorage.GetFloat((ImGuiID) st_view_p0_x, points_min.x);
		view_top.p0.y = window->StateStorage.GetFloat((ImGuiID) st_view_p0_y, points_min.y);
		
		view_top.p1.x = window->StateStorage.GetFloat((ImGuiID) st_view_p1_x, points_max.x);
		view_top.p1.y = window->StateStorage.GetFloat((ImGuiID) st_view_p1_y, points_max.y);
		
		if (view_res == true)
		{
			view_top.p0.x = points_min.x;
			view_top.p0.y = points_min.y;
			
			view_top.p1.x = points_max.x;
			view_top.p1.y = points_max.y;
		}
		
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
			ImVec2 scale_point = inv_transform(&view_top, ImGui::GetMousePos());
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
		
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			trj_traj_bz_compile(self);
		}
		
		if (view_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && io.KeyCtrl)
		{
			ImVec2 pos_p = inv_transform(&view_top, io.MousePos);
			
			s_trj_traj_bz_point point = (s_trj_traj_bz_point)
					{
							.time = 0.0,
							.pos_p = {pos_p[0], 0.0, pos_p[1]},
							.pos_d = {pos_p[0], 0.0, pos_p[1]},
							.pos_t = {0.0, 0.0, 0.0},
							//			.rot_p = {  },
					};
			
			if (self->pts_offset > 0x00)
			{
				point.time = self->pts[self->pts_offset - 1].time + 1.0;
			}
			
			trj_traj_bz_add(self, point);
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
				ImVec2 a = transform(&view_top, ImVec2(view_top.p0.x + i * step_x, view_top.p0.y));
				ImVec2 b = transform(&view_top, ImVec2(view_top.p0.x + i * step_x, view_top.p1.y));
				
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
				ImVec2 a = transform(&view_top, ImVec2(view_top.p0.x, view_top.p0.y + i * step_y));
				ImVec2 b = transform(&view_top, ImVec2(view_top.p1.x, view_top.p0.y + i * step_y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (exp > 0)
				{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_top.p0.y + i * step_y)); }
				else
				{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_top.p0.y + i * step_y); }
				
				window->DrawList->AddText(a, col_text_u32, axis_label);
			}
		}
		
		ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
		
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImGui::PushID(i);
			
			ImVec2 point_pos = ImVec2(self->pts[i].pos_p[0], self->pts[i].pos_p[2]);
			ImGui::SetCursorScreenPos(transform(&view_top, point_pos) - ImVec2(8, 8));
			
			ImGui::InvisibleButton("##pt", ImVec2(16, 16));
			
			ImVec2 center = transform(&view_top, point_pos);
			
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
			{ window->DrawList->AddCircleFilled(center, 4, col_text_u32); }
			
			else
			{ window->DrawList->AddCircle(center, 4, col_text_u32); }
			
			char pt_label[255];
			
			ImFormatString(pt_label, sizeof(pt_label), "%d", i);
			window->DrawList->AddText(center, col_text_u32, pt_label);
			
			if (ImGui::IsItemActive())
			{
				ImVec2 mouse_drag_delta = ImVec2(
						io.MouseDelta.x * view_top.width / view_top.rect.GetWidth(),
						io.MouseDelta.y * view_top.height / view_top.rect.GetHeight()
				);
				
				mouse_drag_delta.y = -mouse_drag_delta.y;
				point_pos += mouse_drag_delta;
			}
			
			self->pts[i].pos_p[0] = point_pos.x;
			self->pts[i].pos_p[2] = point_pos.y;
			
			ImGui::PopID();
		}
		
		// restore ImGui cursor position
		ImGui::SetCursorScreenPos(cursor_pos);
		
		// draw control points and lines
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImGui::PushID(i);
			
			ImVec2 p = transform(&view_top, ImVec2(
					self->pts[i].pos_p[0],
					self->pts[i].pos_p[2]));
			
			ImVec2 d = transform(&view_top, ImVec2(
					self->pts[i].pos_p[0] + self->pts[i].pos_d[0],
					self->pts[i].pos_p[2] + self->pts[i].pos_d[2]));
			
			//		window->DrawList->AddTriangleFilled(
			//				pos_d + ImVec2(-4,0),
			//				pos_d + ImVec2(+4,0),
			//				pos_d + ImVec2(0,-5.6), col_textdis_u32);
			
			window->DrawList->AddLine(
					d + ImVec2(-4, -4),
					d + ImVec2(+4, +4),
					col_textdis_u32);
			
			window->DrawList->AddLine(
					d + ImVec2(+4, -4),
					d + ImVec2(-4, +4),
					col_textdis_u32);
			
			window->DrawList->AddLine(p, d, col_textdis_u32);
			
			ImGui::PopID();
		}
		
		// draw trajectory
		if (self->pts_offset > 0x01)
		{
			vlf_t time_step = (self->pts[self->pts_offset - 1].time - self->pts[0].time) / 1000.0;
			vlf_t time = self->pts[0].time;
			
			vlf_t p0[3];
			vlf_t p1[3];
			
			for (int i = 0; i < 1000; ++i)
			{
				trj_traj_bz_pos(self, time, p0);
				trj_traj_bz_pos(self, time + time_step, p1);
				
				ImVec2 p0_ = ImVec2(p0[0], p0[2]);
				ImVec2 p1_ = ImVec2(p1[0], p1[2]);
				
				window->DrawList->AddLine(transform(&view_top, p0_), transform(&view_top, p1_), col_text_u32);
				
				time += time_step;
			}
		} else
		{
			// TODO draw one dot
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
	{
		ImGui::PushID("view_bot");
		
		ImGuiID id = parent_window->GetID(label);
		
		ImGui::BeginChildFrame(id, ImVec2(size.x, ImGui::GetContentRegionAvail().y),
							   ImGuiWindowFlags_NoScrollbar |
							   ImGuiWindowFlags_NoScrollWithMouse);
		
		ImGuiWindow *window = ImGui::GetCurrentWindow();
		
		bool view_hovered = ImGui::IsWindowHovered();
		
		ImVec2 points_min = (self->pts_offset == 0x00) ? ImVec2(-1.0, -1.0) : ImVec2(+FLT_MAX, +FLT_MAX);
		ImVec2 points_max = (self->pts_offset == 0x00) ? ImVec2(+1.0, +1.0) : ImVec2(-FLT_MAX, -FLT_MAX);
		
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImVec2 point = ImVec2(self->pts[i].time, self->pts[i].pos_p[1]);
			
			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);
			
			point = ImVec2(
					self->pts[i].time + self->pts[i].pos_t[1],
					self->pts[i].pos_p[1] + self->pts[i].pos_d[1]);
			
			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);
		}
		
		points_max.y = ImMax(points_max.y, points_min.y + 0.0001f);
		
		view_bot.int_drag = window->StateStorage.GetBool((ImGuiID) st_int_drag, false);
		view_bot.int_drag_d.x = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dx, 0.0);
		view_bot.int_drag_d.y = window->StateStorage.GetFloat((ImGuiID) st_int_drag_dy, 0.0);
		
		view_bot.p0.x = window->StateStorage.GetFloat((ImGuiID) st_view_p0_x, points_min.x);
		view_bot.p0.y = window->StateStorage.GetFloat((ImGuiID) st_view_p0_y, points_min.y);
		
		view_bot.p1.x = window->StateStorage.GetFloat((ImGuiID) st_view_p1_x, points_max.x);
		view_bot.p1.y = window->StateStorage.GetFloat((ImGuiID) st_view_p1_y, points_max.y);
		
		if (view_res == true)
		{
			view_bot.p0.x = points_min.x;
			view_bot.p0.y = points_min.y;
			
			view_bot.p1.x = points_max.x;
			view_bot.p1.y = points_max.y;
		}
		
		view_bot.rect = window->InnerRect;
		
		view_bot.width = (view_bot.p1 - view_bot.p0).x;
		view_bot.height = (view_bot.p1 - view_bot.p0).y;
		
		if (view_bot.width < 1E-6)
		{
			view_bot.p0.x -= 1.0;
			view_bot.p1.x += 1.0;
		}
		
		if (view_bot.height < 1E-6)
		{
			view_bot.p0.y -= 1.0;
			view_bot.p1.y += 1.0;
		}
		
		view_bot.width = (view_bot.p1 - view_bot.p0).x;
		view_bot.height = (view_bot.p1 - view_bot.p0).y;
		
		if (io.MouseWheel != 0x00 && view_hovered && !view_bot.int_drag)
		{
			ImVec2 scale_point = inv_transform(&view_bot, ImGui::GetMousePos());
			float32_t scroll_delta = io.MouseWheel * 0.1f;
			
			if (scroll_delta > +0.5) scroll_delta = +0.5;
			if (scroll_delta < -0.5) scroll_delta = -0.5;
			
			view_bot.p0 = scale_point + (view_bot.p0 - scale_point) * (1.0f - scroll_delta);
			view_bot.p1 = scale_point + (view_bot.p1 - scale_point) * (1.0f - scroll_delta);
		}
		
		if (view_hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			ImVec2 mouse_drag_delta = ImVec2(
					io.MouseDelta.x * view_bot.width / view_bot.rect.GetWidth(),
					io.MouseDelta.y * view_bot.height / view_bot.rect.GetHeight()
			);
			
			mouse_drag_delta.y = -mouse_drag_delta.y;
			
			view_bot.p0 -= mouse_drag_delta;
			view_bot.p1 -= mouse_drag_delta;
		}
		
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			trj_traj_bz_compile(self);
		}
		
		if (view_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && io.KeyCtrl)
		{
			ImVec2 view_pt = inv_transform(&view_bot, io.MousePos);
			
			vlf_t pt_pos[3];
			
			trj_traj_bz_pos(self, view_pt.x, pt_pos);
			
			s_trj_traj_bz_point point = (s_trj_traj_bz_point)
			{
					.time = view_pt.x,
					.pos_p = { pt_pos[0], view_pt.y, pt_pos[2] },
					.pos_d = { pt_pos[0], view_pt.y, pt_pos[2] },
					.pos_t = { 0.0, 0.0, 0.0 },
					//			.rot_p = {  },
			};
			
			trj_traj_bz_add(self, point);
		}
		
		view_bot.width = (view_bot.p1 - view_bot.p0).x;
		view_bot.height = (view_bot.p1 - view_bot.p0).y;
		view_bot.rect = window->InnerRect;
		
		// draw grid TODO:: replace with rectangles coz they are drawn faster on GPUs
		{
			char axis_label[64];
			int32_t exp;
			
			frexp(view_bot.width / 5, &exp);
			float step_x = (float) ldexp(1.0, exp);
			
			for (int i = 0; i < int(view_bot.width / step_x) + 2; ++i)
			{
				ImVec2 a = transform(&view_bot, ImVec2(view_bot.p0.x + i * step_x, view_bot.p0.y));
				ImVec2 b = transform(&view_bot, ImVec2(view_bot.p0.x + i * step_x, view_bot.p1.y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (exp > 0)
				{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_bot.p0.x + i * step_x)); }
				else
				{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_bot.p0.x + i * step_x); }
				
				window->DrawList->AddText(b, col_text_u32, axis_label);
			}
			
			frexp(view_bot.height / 10, &exp);
			float step_y = (float) ldexp(1.0, exp);
			
			for (int i = 0; i < int(view_bot.height / step_y) + 2; ++i)
			{
				ImVec2 a = transform(&view_bot, ImVec2(view_bot.p0.x, view_bot.p0.y + i * step_y));
				ImVec2 b = transform(&view_bot, ImVec2(view_bot.p1.x, view_bot.p0.y + i * step_y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (exp > 0)
				{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_bot.p0.y + i * step_y)); }
				else
				{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_bot.p0.y + i * step_y); }
				
				window->DrawList->AddText(a, col_text_u32, axis_label);
			}
		}
		
		ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
		
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImGui::PushID(i);
			
			ImVec2 point_pos = ImVec2(self->pts[i].time, self->pts[i].pos_p[1]);
			ImGui::SetCursorScreenPos(transform(&view_bot, point_pos) - ImVec2(8, 8));
			
			ImGui::InvisibleButton("##pt", ImVec2(16, 16));
			
			ImVec2 center = transform(&view_bot, point_pos);
			
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
			{ window->DrawList->AddCircleFilled(center, 4, col_text_u32); }
			
			else
			{ window->DrawList->AddCircle(center, 4, col_text_u32); }
			
			char pt_label[255];
			
			ImFormatString(pt_label, sizeof(pt_label), "%d", i);
			window->DrawList->AddText(center, col_text_u32, pt_label);
			
			if (ImGui::IsItemActive())
			{
				ImVec2 mouse_drag_delta = ImVec2(
						io.MouseDelta.x * view_bot.width / view_bot.rect.GetWidth(),
						io.MouseDelta.y * view_bot.height / view_bot.rect.GetHeight()
				);
				
				mouse_drag_delta.y = -mouse_drag_delta.y;
				point_pos += mouse_drag_delta;
			}
			
			self->pts[i].time = point_pos.x;
			self->pts[i].pos_p[1] = point_pos.y;
			
			ImGui::PopID();
		}
		
		// restore ImGui cursor position
		ImGui::SetCursorScreenPos(cursor_pos);
		
		// draw trajectory
		if (self->pts_offset > 0x01)
		{
			vlf_t time_step = (self->pts[self->pts_offset - 1].time - self->pts[0].time) / 1000.0;
			vlf_t time = self->pts[0].time;
			
			vlf_t p0[3];
			vlf_t p1[3];
			
			for (int i = 0; i < 1000; ++i)
			{
				trj_traj_bz_pos(self, time, p0);
				trj_traj_bz_pos(self, time + time_step, p1);
				
				ImVec2 p0_ = ImVec2(time, p0[1]);
				ImVec2 p1_ = ImVec2(time+time_step, p1[1]);
				
				window->DrawList->AddLine(transform(&view_bot, p0_), transform(&view_bot, p1_), col_text_u32);
				
				time += time_step;
			}
		} else
		{
			// TODO draw one dot
		}
		
		window->StateStorage.SetFloat((ImGuiID) st_view_p0_x, view_bot.p0.x);
		window->StateStorage.SetFloat((ImGuiID) st_view_p0_y, view_bot.p0.y);
		
		window->StateStorage.SetFloat((ImGuiID) st_view_p1_x, view_bot.p1.x);
		window->StateStorage.SetFloat((ImGuiID) st_view_p1_y, view_bot.p1.y);
		
		window->StateStorage.SetBool((ImGuiID) st_int_drag_dx, view_bot.int_drag_d.x);
		window->StateStorage.SetBool((ImGuiID) st_int_drag_dy, view_bot.int_drag_d.y);
		
		ImGui::EndChildFrame();
		
		ImGui::PopID();
	}
	
	return;
}

//------------------------------------------------------------------------------

void trj_gui_traj_forb_edit(s_trj_traj *self)
{
	s_trj_traj_forb *traj = (s_trj_traj_forb*) self->data;
	
	ImGui::PushID(self);

	ImGui::Text("ref   ");
	
//	const char* items[*self->obj_count];
//	for (int i = 0; i < *self->obj_count; ++i)
//	{ items[i] = self->obj_list[i].name; }
//	ImGui::SetNextItemWidth(-1);
//	if (ImGui::BeginCombo("##ref",
//			items[self->ref->id],
//			ImGuiComboFlags_NoArrowButton))
//	{
//		for (int i = 0; i < IM_ARRAYSIZE(items); ++i)
//		{
//			const bool is_selected = (self->ref->id == i);
//
//			if (ImGui::Selectable(items[i], is_selected)) { self->ref = &self->obj_list[i]; }
//			if (is_selected) { ImGui::SetItemDefaultFocus(); }
//		}
//
//		ImGui::EndCombo();
//	}
//
	ImGui::Text("radius");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
	ImGui::SameLine();
	vlf_t radius_min = 0.0;
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##radius", ImGuiDataType_Double, &traj->radius, 1.0, &radius_min, NULL, "%.3f");
	
	ImGui::Text("rate  ");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[deg/s]"); }
	ImGui::SameLine();
	vlf_t rate_deg = vl_deg(traj->rate);
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##rate", ImGuiDataType_Double, &rate_deg, 0.1, NULL, NULL, "%.3f");
	traj->rate = vl_rad(rate_deg);
	
	ImGui::Text("tilt  "); ImGui::SameLine();
	vl_gui_rot("##tilt", traj->tilt);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Text("s_rate");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[deg/s]"); }
	ImGui::SameLine();
	vlf_t s_rate_deg = vl_deg(traj->s_rate);
	ImGui::SetNextItemWidth(-1);
	ImGui::DragScalar("##s_rate", ImGuiDataType_Double, &s_rate_deg, 0.1, NULL, NULL, "%.3f");
	traj->s_rate = vl_rad(s_rate_deg);
	
	ImGui::Text("s_tilt"); ImGui::SameLine();
	vl_gui_rot("##s_tilt", traj->s_tilt);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

void trj_gui_traj_aorb_edit(s_trj_traj *self)
{
	s_trj_traj_aorb *traj = (s_trj_traj_aorb*) self->data;
	

	
	return;
}


//------------------------------------------------------------------------------
























