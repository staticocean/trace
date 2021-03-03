
#ifndef __GUI_TRAJ__
#define __GUI_TRAJ__

//----------------------------------------------------------------

#include <softael_lib/vl.h>
#include <softael_lib/vl3d.h>

#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

#include "gui_w.h"

//----------------------------------------------------------------

typedef struct view_data
{
	ImVec2 	  p0;
	ImVec2 	  p1;
	
	float32_t width;
	float32_t height;
	
	ImRect 	  rect;
	
	bool 	  int_drag;
	ImVec2    int_drag_d;
	
} 	s_view_data;

//----------------------------------------------------------------

enum st
{
	st_view_p0_x = 100, // range point 0
	st_view_p0_y,
	st_view_p1_x,
	st_view_p1_y,
	st_int_drag,
	st_int_drag_dx,
	st_int_drag_dy,
};

//----------------------------------------------------------------

inline void gui_traj_edit(s_trj_traj *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::InputText("##name", self->name, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_traj_edit_static(s_trj_traj *self)
{
	ImGui::PushID(self);
	
	s_trj_traj_static *traj = (s_trj_traj_static*) self->data;
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
//
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) traj->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", traj->eng->obj_count, traj->eng->obj_list, &traj->ref);
	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ellp  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-40);
	gui_ellpsel("##ellp", traj->eng->ellp_offset, traj->eng->ellp_list, &traj->ellp);
	ImGui::SameLine(0.0, 0.0);
	imgui_bool("##ellp_en", ImVec2(-1, 0), &traj->ellp_en);
	if (traj->ellp == NULL) { traj->ellp_en = 0x00; }
	if (traj->ellp != NULL) { traj->ellp_hash = traj->ellp->hash; }
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	if (traj->ellp_en == 0x00)
	{
		ImGui::Text("pos   "); ImGui::SameLine();
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
		imgui_vec("##pos", traj->pos, 1.0, NULL, NULL, "%.3f");
	}
	else
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("lat   "); ImGui::SameLine();
		ImGui::SameLine();
		imgui_lat("##lat", &traj->pos[0], ImGui::GetContentRegionAvailWidth());
		
		ImGui::AlignTextToFramePadding();
		ImGui::Text("lon   "); ImGui::SameLine();
		ImGui::SameLine();
		imgui_lon("##lon", &traj->pos[1], ImGui::GetContentRegionAvailWidth());
		
		ImGui::AlignTextToFramePadding();
		ImGui::Text("alt   "); ImGui::SameLine();
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImGui::DragScalar("##alt", ImGuiDataType_Double, &traj->pos[2], 1.0, NULL, NULL, "%.3f");
	}
	
	ImGui::Text("rot   "); ImGui::SameLine();
	imgui_rot("##rot", traj->rot);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_traj_view_static(s_trj_traj *self)
{
	ImGui::PushID(self);
	
	ImGui::Text("[Plugin does not support view]");
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_traj_edit_orb(s_trj_traj *self)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) self->data;
	
	ImGui::PushID(self);
	
	// !!! UPDATE HASHES !!!
	// if ref name was changed we must recalc hash
	// to retain save/load and gui objsel functionality
	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
//
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) traj->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-60);
	gui_objsel("##ref", traj->eng->obj_count, traj->eng->obj_list, &traj->ref);
	ImGui::SameLine(0.0, 0.0);
	imgui_switch("##sync_en", (char*[2]) { "SYNC", "FLOAT" }, ImVec2(-1, 0), &traj->sync_en);
	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Text("radius");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
	ImGui::SameLine();
	vlf_t radius_min = 0.0;
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::DragScalar("##radius", ImGuiDataType_Double, &traj->radius, 1.0, &radius_min, NULL, "%.3f");
	
	ImGui::Text("rate  ");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[deg/s]"); }
	ImGui::SameLine();
	vlf_t rate_deg = vl_deg(traj->rate);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::DragScalar("##rate", ImGuiDataType_Double, &rate_deg, 0.1, NULL, NULL, "%.9f");
	traj->rate = vl_rad(rate_deg);
	
	ImGui::Text("tilt  "); ImGui::SameLine();
	imgui_rot("##tilt", traj->tilt);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::Text("s_rate");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[deg/s]"); }
	ImGui::SameLine();
	vlf_t s_rate_deg = vl_deg(traj->s_rate);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::DragScalar("##s_rate", ImGuiDataType_Double, &s_rate_deg, 0.1, NULL, NULL, "%.9f");
	traj->s_rate = vl_rad(s_rate_deg);
	
	ImGui::Text("s_tilt"); ImGui::SameLine();
	imgui_rot("##s_tilt", traj->s_tilt);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_traj_view_orb(s_trj_traj *self)
{
	s_trj_traj_orb *traj = (s_trj_traj_orb*) self->data;
	
	s_vl3d_eng vl3d_eng;
	s_vl3d_obj *obj_list = (s_vl3d_obj*) malloc(sizeof(s_vl3d_obj) * 4096 * 2);
	s_vl3d_view view = { .pos = { 0.0, 0.0, 0.0 }, .scale = 1.0, .tbar_en = 0x01, .xyz_en = 0x01, .xyz_scale = 0.25, .grid_mode = 0x01, .grid_pt_size = 2.0, .grid_pt_disp = 2.0,  };
	
	vl3d_view_load(self, &view, view);
	
	vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) {
			.obj_list = obj_list,
	});
	
	if (fabs(traj->rate) > 1E-9)
	{
		vlf_t time = 0.0;
		vlf_t time_limit = vl_2pi / fabs(traj->rate);
		vlf_t time_step = time_limit / 1000;
		int time_iter = time_limit / time_step;
		
		vlf_t p0[3];
		vlf_t p1[3];
		vlf_t rot[9];
		
		trj_traj_orb_pos(traj, time, p0);
		trj_traj_orb_pos(traj, time, p1);
		
		for (int i = 0; i < time_iter; ++i)
		{
			time += time_step;
			
			vl_vcopy(p0, p1);
			trj_traj_orb_pos(traj, time, p1);
			
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) {
					.color = vl3d_col_legacy,
					.p0 = { p0[0], p0[1], p0[2] },
					.p1 = { p1[0], p1[1], p1[2] },
			});
		}
	}
	
	vl3d_view_grid(&view, &vl3d_eng);
	vl3d_view_xyz(&view, &vl3d_eng);
	vl3d_eng_render(&vl3d_eng, &view, "temp", ImVec2(-1, -1));
	vl3d_view_save(self, &view);
	
	free(obj_list);
}

//----------------------------------------------------------------

inline void gui_traj_edit_bz(s_trj_traj *self)
{
	ImGui::PushID(self);
	
	s_trj_traj_bz *traj = (s_trj_traj_bz*) self->data;
	
	// !!! UPDATE HASHES !!!
	// if ref name was changed we must recalc hash
	// to retain save/load and gui objsel functionality
	if (traj->ref  != NULL) { traj->ref_hash  = traj->ref->hash ; }
	if (traj->ellp != NULL) { traj->ellp_hash = traj->ellp->hash; }
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
//
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) traj->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", traj->eng->obj_count, traj->eng->obj_list, &traj->ref);
	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ellp  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-40);
	uint8_t ellp_en = traj->ellp_en;
	s_trj_ellp *ellp_ref = traj->ellp;
	gui_ellpsel("##ellp", traj->eng->ellp_offset, traj->eng->ellp_list, &traj->ellp);
	ImGui::SameLine(0.0, 0.0);
	imgui_bool("##ellp_en", ImVec2(-1, 0), &traj->ellp_en);
	if (traj->ellp == NULL) { traj->ellp_en = 0x00; }
	if (traj->ellp != NULL) { traj->ellp_hash = traj->ellp->hash; }
	
	if (ellp_en != traj->ellp_en) // support conversion when changing ellp support
    {
	    // if we turn off ellp support then convert degrees to m
	    if (traj->ellp_en == 0x00)
        {
            vlf_t pos_ecef[3];

            for (int i = 0; i < traj->pts_offset; ++i)
            {
                trj_ellp_ecef(ellp_ref, pos_ecef, traj->pts[i].pos_p);
                vl_vcopy(traj->pts[i].pos_p, pos_ecef);
            }
        }

	    // if we turn on ellp support then convert m to lla
	    if (traj->ellp_en != 0x00)
        {
            vlf_t pos_lla[3];

            for (int i = 0; i < traj->pts_offset; ++i)
            {
                trj_ellp_lla(traj->ellp, pos_lla, traj->pts[i].pos_p);
                vl_vcopy(traj->pts[i].pos_p, pos_lla);
            }
        }
	    
	    trj_traj_bz_compile(traj);
    }

	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	if (traj->pts_offset == 0x00)
	{
		ImGui::TextWrapped("No points added. To add a point press CTRL+LM.");
	}
	
	for (int i = 0; i < traj->pts_offset; ++i)
	{
		ImGui::PushID(i);

        ImGui::BeginGroup();

        vlf_t time_min = 0.0;
        ImGui::SetNextItemWidth(50);
        ImGui::DragScalar("##time", ImGuiDataType_Double, &traj->pts[i].time, 1.0, &time_min, NULL, "%.0f");
        ImGui::SameLine();

        if (traj->ellp_en == 0x00)
        {
            imgui_vec("point", traj->pts[i].pos_p, 1.0, NULL, NULL, "%.3f");
        }

        else
        {
            vlf_t lla_deg[3] = {vl_deg(traj->pts[i].pos_p[0]), vl_deg(traj->pts[i].pos_p[1]), traj->pts[i].pos_p[2]};
            imgui_vec("##point", lla_deg, 0.001, NULL, NULL, "%.3f");

            traj->pts[i].pos_p[0] = vl_rad(lla_deg[0]);
            traj->pts[i].pos_p[1] = vl_rad(lla_deg[1]);
            traj->pts[i].pos_p[2] = lla_deg[2];
        }

        ImGui::EndGroup();

        if (ImGui::BeginPopupContextItem("edit"))
        {
            if (ImGui::Selectable("delete")) {
                trj_traj_bz_rem(traj, i);
                trj_traj_bz_compile(traj);
            }

            ImGui::EndPopup();
        }
	
		ImGui::PopID();
	}
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline ImVec2 transform(s_view_data *view, ImVec2 pos)
{
	float32_t x = view->rect.GetWidth()  * (pos.x - view->p0.x) / (view->width);
	float32_t y = view->rect.GetHeight() - view->rect.GetHeight() * (pos.y - view->p0.y) / (view->height);
	
	return ImVec2(x + view->rect.Min.x, y + view->rect.Min.y);
}

inline ImVec2 inv_transform(s_view_data *view, ImVec2 pos)
{
	ImVec2 temp = ImVec2(pos.x - view->rect.Min.x, pos.y - view->rect.Min.y);
	
	float32_t x = temp.x * view->width / view->rect.GetWidth()  + view->p0.x;
	float32_t y = (- temp.y + view->rect.GetHeight()) * view->height / view->rect.GetHeight() + view->p0.y;
	
	return ImVec2(x, y);
}

inline void gui_traj_view_bz(s_trj_traj_bz *self, const char* label, ImVec2 size, bool view_res)
{
	uint32_t del_index = 0x00;
	uint8_t del_req = 0x00;
	
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

			if (self->ellp_en == 0x00) { point = ImVec2(self->pts[i].pos_p[2], self->pts[i].pos_p[0]); }
			else { point = ImVec2(self->pts[i].pos_p[1], self->pts[i].pos_p[0]); }

			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);

            if (self->ellp_en == 0x00)
            {
                point = ImVec2(
                        self->pts[i].pos_p[2] + self->pts[i].pos_d[2],
                        self->pts[i].pos_p[0] + self->pts[i].pos_d[0]);
            }
            else
            {
                point = ImVec2(
                        self->pts[i].pos_p[1] + self->pts[i].pos_d[1],
                        self->pts[i].pos_p[0] + self->pts[i].pos_d[0]);
            }

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

			s_trj_traj_bz_point point;

            if (self->ellp_en == 0x00)
			{
				point = (s_trj_traj_bz_point)
				{
						.time = 0.0,
						.pos_p = {pos_p.y, 0.0, pos_p.x},
						.pos_t = {0.0, 0.0, 0.0},
						.pos_d = {pos_p.y, 0.0, pos_p.x},
						//			.rot_p = {  },
				};
			}
            
            else
			{
				point = (s_trj_traj_bz_point)
				{
						.time = 0.0,
						.pos_p = {pos_p.y, pos_p.x, 0.0},
						.pos_t = {0.0, 0.0, 0.0},
						.pos_d = {pos_p.y, pos_p.x, 0.0},
						//			.rot_p = {  },
				};
			}
   
   
			if (self->pts_offset > 0x00)
			{
				point.time = self->pts[self->pts_offset - 1].time + 1.0;
			}
			
			trj_traj_bz_add(self, point);
		}
		
		view_top.width = (view_top.p1 - view_top.p0).x;
		view_top.height = (view_top.p1 - view_top.p0).y;
		view_top.rect = window->InnerRect;
		
		// draw toolbar
		{
			ImGui::BeginGroup();
			ImGui::AlignTextToFramePadding();
			
			if (ImGui::Button("AUTO"))
			{ view_top.p0 = points_min; view_top.p1 = points_max; }
			
			ImGui::EndGroup();
		}
		
		// draw grid TODO:: replace with rectangles coz they are drawn faster on GPUs
		{
			char axis_label[64];
			int32_t exp;
			
			frexp(view_top.width / 10, &exp);
			float step_x = (float) ldexp(1.0, exp);
			
			for (int i = 0; i < int(view_top.width / step_x) + 2; ++i)
			{
				ImVec2 a = transform(&view_top, ImVec2(view_top.p0.x + i * step_x, view_top.p0.y));
				ImVec2 b = transform(&view_top, ImVec2(view_top.p0.x + i * step_x, view_top.p1.y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (self->ellp_en == 0x00)
				{
					if (exp > 0)
					{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_top.p0.x + i * step_x)); }
					else
					{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_top.p0.x + i * step_x); }
				} else
				{
					ImFormatString(axis_label, sizeof(axis_label), " %f", vl_deg(view_top.p0.x + i * step_x));
				}
			
				window->DrawList->AddText(b, col_text_u32, axis_label);
			}
			
			frexp(view_top.height / 10, &exp);
			float step_y = (float) ldexp(1.0, exp);
			
			for (int i = 0; i < int(view_top.height / step_y) + 2; ++i)
			{
				ImVec2 a = transform(&view_top, ImVec2(view_top.p0.x, view_top.p0.y + i * step_y));
				ImVec2 b = transform(&view_top, ImVec2(view_top.p1.x, view_top.p0.y + i * step_y));
				
				window->DrawList->AddLine(a, b, col_textdis_u32);
				
				if (self->ellp_en == 0x00)
				{
					if (exp > 0)
					{ ImFormatString(axis_label, sizeof(axis_label), " %d", int(view_top.p0.y + i * step_y)); }
					else
					{ ImFormatString(axis_label, sizeof(axis_label), " %f", view_top.p0.y + i * step_y); }
				} else
				{
					ImFormatString(axis_label, sizeof(axis_label), " %f", vl_deg(view_top.p0.y + i * step_y));
				}
				
				window->DrawList->AddText(a, col_text_u32, axis_label);
			}
		}
		
		// draw points
		ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
		
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImGui::PushID(i);
			
			ImVec2 point_pos;
			
			if (self->ellp_en == 0x00) { point_pos = ImVec2(self->pts[i].pos_p[2], self->pts[i].pos_p[0]); }
			else { point_pos = ImVec2(self->pts[i].pos_p[1], self->pts[i].pos_p[0]); }
			
			ImGui::SetCursorScreenPos(transform(&view_top, point_pos) - ImVec2(8, 8));
			
			ImGui::InvisibleButton("##pt", ImVec2(16, 16));
			
			ImVec2 center = transform(&view_top, point_pos);
			
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
			{ window->DrawList->AddCircleFilled(center, 4, vl3d_col_legacy); }
			
			else
			{ window->DrawList->AddCircle(center, 4, vl3d_col_legacy); }
			
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

			if (self->ellp_en == 0x00)
			{
				self->pts[i].pos_p[2] = point_pos.x;
				self->pts[i].pos_p[0] = point_pos.y;
			}
			
			else
			{
				self->pts[i].pos_p[1] = point_pos.x;
				self->pts[i].pos_p[0] = point_pos.y;
			}
			
            if (ImGui::BeginPopupContextItem("view"))
            {
                ImGui::Text("Point %d", i);

                ImGui::Separator();

                ImGui::Text("time");
				if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[sec]"); }
                const vlf_t time_min = 0.0;
                ImGui::SameLine();
                ImGui::SetNextItemWidth(160);
                ImGui::DragScalar("##time", ImGuiDataType_Double, &self->pts[i].time,
                                  1, &time_min, NULL, "%.3f");

                if (self->ellp_en == 0x00)
                {
                    ImGui::Text("x   ");
					if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(160);
                    ImGui::DragScalar("##x", ImGuiDataType_Double, &self->pts[i].pos_p[0],
                                      1, NULL, NULL, "%.3f");

                    ImGui::Text("y   ");
					if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(160);
                    ImGui::DragScalar("##y", ImGuiDataType_Double, &self->pts[i].pos_p[1],
                                      1, NULL, NULL, "%.3f");

                    ImGui::Text("z   ");
					if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(160);
                    ImGui::DragScalar("##z", ImGuiDataType_Double, &self->pts[i].pos_p[2],
                                      1, NULL, NULL, "%.3f");
                }

                else
                {
//                    vlf_t lla_deg[3] = {vl_deg(),
//                                        vl_deg(self->pts[i].pos_p[1]),
//                                        self->pts[i].pos_p[2]};

                    ImGui::Text("lat ");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(160);
                    imgui_lat("##lat", &self->pts[i].pos_p[0]);

                    ImGui::Text("lon ");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(160);
					imgui_lon("##lon", &self->pts[i].pos_p[1]);

                    ImGui::Text("alt ");
					if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(160);
                    ImGui::DragScalar("##alt", ImGuiDataType_Double, &self->pts[i].pos_p[2],
                                      1, NULL, NULL, "%.3f");
                    
//                    self->pts[i].pos_p[0] = vl_rad(lla_deg[0]);
//                    self->pts[i].pos_p[1] = vl_rad(lla_deg[1]);
//                    self->pts[i].pos_p[2] = lla_deg[2];
                }

                ImGui::Separator();

                if (ImGui::Selectable("delete"))
                {
                    del_req = 0x01;
                    del_index = i;
                }

                ImGui::EndPopup();
            }

            ImGui::PopID();
		}
		
		// restore ImGui cursor position
		ImGui::SetCursorScreenPos(cursor_pos);
		
		// draw control points and lines
		for (int i = 0; i < self->pts_offset; ++i)
		{
			ImGui::PushID(i);
			
			ImVec2 p;
			
			if (self->ellp_en == 0x00)
			{
				p = transform(&view_top, ImVec2(
						self->pts[i].pos_p[2],
						self->pts[i].pos_p[0]));
			}
			
			else
			{
				p = transform(&view_top, ImVec2(
						self->pts[i].pos_p[1],
						self->pts[i].pos_p[0]));
			}
			
			ImVec2 d;
			
			if (self->ellp_en == 0x00)
			{
				d = transform(&view_top, ImVec2(
						self->pts[i].pos_p[2] + self->pts[i].pos_d[2],
						self->pts[i].pos_p[0] + self->pts[i].pos_d[0]));
			}
			
			else
			{
				d = transform(&view_top, ImVec2(
						self->pts[i].pos_p[1] + self->pts[i].pos_d[1],
						self->pts[i].pos_p[0] + self->pts[i].pos_d[0]));
			}
			
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
		// tmeporarily disable ellipsoid projection if enabled to cast to plane
		uint8_t ellp_en_temp = self->ellp_en;
		self->ellp_en = 0x00;

		s_trj_obj *ref_temp = self->ref;
		self->ref = &self->eng->obj_list[0x00];

		if (self->pts_offset > 0x01)
		{
			vlf_t time_step = (self->pts[self->pts_offset - 1].time - self->pts[0].time) / 1000.0;
			vlf_t time = self->pts[0].time;
			
			vlf_t p0[3];
			vlf_t p1[3];
			
			for (int i = 0; i < 1000; ++i)
			{
				trj_traj_bz_pos_local(self, time, p0);
				trj_traj_bz_pos_local(self, time + time_step, p1);
				
				if (ellp_en_temp == 0x00)
				{
					ImVec2 p0_ = ImVec2(p0[2], p0[0]);
					ImVec2 p1_ = ImVec2(p1[2], p1[0]);
					
					window->DrawList->AddLine(transform(&view_top, p0_), transform(&view_top, p1_), vl3d_col_legacy);
				}
				
				else
				{
					ImVec2 p0_ = ImVec2(p0[1], p0[0]);
					ImVec2 p1_ = ImVec2(p1[1], p1[0]);
					
					window->DrawList->AddLine(transform(&view_top, p0_), transform(&view_top, p1_), vl3d_col_legacy);
				}
				
				time += time_step;
			}
		}
		
		else
		{
			// TODO draw one dot
		}

		self->ref = ref_temp;

//		// heading
//		if (self->pts_offset > 0x01)
//		{
//			vlf_t time_step = (self->pts[self->pts_offset - 1].time - self->pts[0].time) / 100.0;
//			vlf_t time = self->pts[0].time;
//
//			vlf_t p0[3];
//			vlf_t r0[9];
//
//			for (int i = 0; i < 100; ++i)
//			{
//				self->ellp_en = 0x00;
//				trj_traj_bz_pos(self, time, p0);
//				self->ellp_en = 0x01;
//				trj_traj_bz_rot(self, time, r0);
//				self->ellp_en = 0x00;
//
//				s_trj_rot_hpr hpr;
//				trj_ctn_to_hpr(&hpr, r0);
//
//				ImVec2 p0_ = transform(&view_top, ImVec2(p0[0], p0[2]));
//				ImVec2 p1_ = transform(&view_top, ImVec2(p0[0] + 100000*cos(hpr.heading), p0[2] + 100000*sin(hpr.heading)));
//
//				window->DrawList->AddLine(p0_, p1_, col_text_u32);
//
//				time += time_step;
//			}
//		}
		
//		// velocity
//		if (self->pts_offset > 0x01)
//		{
//			vlf_t time_step = (self->pts[self->pts_offset - 1].time - self->pts[0].time) / 100.0;
//			vlf_t time = self->pts[0].time;
//
//			vlf_t p0[3];
//			vlf_t v0[3];
//
//			for (int i = 0; i < 100; ++i)
//			{
//				self->ellp_en = 0x00;
//				trj_traj_bz_pos(self, time, p0);
//				self->ellp_en = 0x01;
//				trj_traj_bz_vel(self, time, v0);
//				self->ellp_en = 0x00;
//
//				ImVec2 p0_ = transform(&view_top, ImVec2(p0[0], p0[2]));
//				ImVec2 p1_ = transform(&view_top, ImVec2(p0[0] + v0[1], p0[2] + v0[0]));
//
//				vl_vmul_s(v0, v0, 100);
//
//				window->DrawList->AddLine(p0_, p1_, col_text_u32);
//
//				time += time_step;
//			}
//		}
		
		self->ellp_en = ellp_en_temp;
		
		// TODO We can calculate traj one time and then use on top and bot views
		
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
			ImVec2 point;
			
			if (self->ellp_en == 0x00)
			{ point = ImVec2(self->pts[i].time, self->pts[i].pos_p[1]); }
			else
			{ point = ImVec2(self->pts[i].time, self->pts[i].pos_p[2]); }

			points_max = ImMax(points_max, point);
			points_min = ImMin(points_min, point);
			
			if (self->ellp_en == 0x00)
			{
				point = ImVec2(
						self->pts[i].time + self->pts[i].pos_t[1],
						self->pts[i].pos_p[1] + self->pts[i].pos_d[1]);
			}
			else
			{
				point = ImVec2(
						self->pts[i].time + self->pts[i].pos_t[2],
						self->pts[i].pos_p[2] + self->pts[i].pos_d[2]);
			}
			
			
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
		
		// draw toolbar
		{
			ImGui::BeginGroup();
			ImGui::AlignTextToFramePadding();
			
			if (ImGui::Button("AUTO"))
			{ view_bot.p0 = points_min; view_bot.p1 = points_max; }
			
			ImGui::EndGroup();
		}
		
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
			
			s_trj_traj_bz_point point;
			
			if (self->ellp_en == 0x00)
			{
				point = (s_trj_traj_bz_point)
				{
						.time = view_pt.x,
						.pos_p = { pt_pos[0], view_pt.y, pt_pos[2] },
						.pos_t = { 0.0, 0.0, 0.0 },
						.pos_d = { pt_pos[0], view_pt.y, pt_pos[2] },
						//			.rot_p = {  },
				};
			}
			else
			{
				point = (s_trj_traj_bz_point)
				{
						.time = view_pt.x,
						.pos_p = { pt_pos[0], pt_pos[1], view_pt.y },
						.pos_t = { 0.0, 0.0, 0.0 },
						.pos_d = { pt_pos[0], pt_pos[1], view_pt.y },
						//			.rot_p = {  },
				};
			}
			
			trj_traj_bz_add(self, point);
		}
		
		view_bot.width = (view_bot.p1 - view_bot.p0).x;
		view_bot.height = (view_bot.p1 - view_bot.p0).y;
		view_bot.rect = window->InnerRect;
		
		// draw grid TODO:: replace with rectangles coz they are drawn faster on GPUs
		{
			char axis_label[64];
			int32_t exp;
			
			frexp(view_bot.width / 10, &exp);
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
			
			ImVec2 point_pos;
			
			if (self->ellp_en == 0x00) { point_pos = ImVec2(self->pts[i].time, self->pts[i].pos_p[1]); }
			else { point_pos = ImVec2(self->pts[i].time, self->pts[i].pos_p[2]); }
			
			ImGui::SetCursorScreenPos(transform(&view_bot, point_pos) - ImVec2(8, 8));
			
			ImGui::InvisibleButton("##pt", ImVec2(16, 16));
			
			ImVec2 center = transform(&view_bot, point_pos);
			
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
			{ window->DrawList->AddCircleFilled(center, 4, vl3d_col_legacy); }
			
			else
			{ window->DrawList->AddCircle(center, 4, vl3d_col_legacy); }
			
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
			
			if (self->ellp_en == 0x00)
			{ self->pts[i].pos_p[1] = point_pos.y; }
			else
			{ self->pts[i].pos_p[2] = point_pos.y; }
			
			ImGui::PopID();
		}
		
		// restore ImGui cursor position
		ImGui::SetCursorScreenPos(cursor_pos);
		
		// draw trajectory
		// tmeporarily disable ellipsoid projection if enabled to cast to plane
		uint8_t ellp_en_temp = self->ellp_en;
		self->ellp_en = 0x00;
		
		if (self->pts_offset > 0x01)
		{
			vlf_t time_step = (self->pts[self->pts_offset - 1].time - self->pts[0].time) / 1000.0;
			vlf_t time = self->pts[0].time;
			
			vlf_t p0[3];
			vlf_t p1[3];
			
			for (int i = 0; i < 1000; ++i)
			{
				trj_traj_bz_pos_local(self, time, p0);
				trj_traj_bz_pos_local(self, time + time_step, p1);
				
				if (ellp_en_temp == 0x00)
				{
					ImVec2 p0_ = ImVec2(time, p0[1]);
					ImVec2 p1_ = ImVec2(time+time_step, p1[1]);
					
					window->DrawList->AddLine(transform(&view_bot, p0_), transform(&view_bot, p1_), vl3d_col_legacy);
				}
				else
				{
					ImVec2 p0_ = ImVec2(time, p0[2]);
					ImVec2 p1_ = ImVec2(time+time_step, p1[2]);
					
					window->DrawList->AddLine(transform(&view_bot, p0_), transform(&view_bot, p1_), vl3d_col_legacy);
				}
				
				time += time_step;
			}
		} else
		{
			// TODO draw one dot
		}
		
		self->ellp_en = ellp_en_temp;
		
		window->StateStorage.SetFloat((ImGuiID) st_view_p0_x, view_bot.p0.x);
		window->StateStorage.SetFloat((ImGuiID) st_view_p0_y, view_bot.p0.y);
		
		window->StateStorage.SetFloat((ImGuiID) st_view_p1_x, view_bot.p1.x);
		window->StateStorage.SetFloat((ImGuiID) st_view_p1_y, view_bot.p1.y);
		
		window->StateStorage.SetBool((ImGuiID) st_int_drag_dx, view_bot.int_drag_d.x);
		window->StateStorage.SetBool((ImGuiID) st_int_drag_dy, view_bot.int_drag_d.y);
		
		ImGui::EndChildFrame();
		
		ImGui::PopID();
	}
	
	if (del_req != 0x00)
	{
		trj_traj_bz_rem(self, del_index);
		// MUS RECOMPILE coz we lost 1 point
		trj_traj_bz_compile(self);
	}
	
	return;
}

//----------------------------------------------------------------

inline void gui_traj_edit_navsat(s_trj_traj *self)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) self->data;
	
	ImGui::PushID(self);
	
	// !!! UPDATE HASHES !!!
	// if ref name was changed we must recalc hash
	// to retain save/load and gui objsel functionality
	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
//
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) traj->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", traj->eng->obj_count, traj->eng->obj_list, &traj->ref);
	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("file  ");
	if (ImGui::IsItemHovered())
	{ ImGui::SetTooltip("Path to almanac file"); }
	ImGui::SameLine();
	gui_fileopen(traj->file_path);
	
	ImGui::Text("type  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	int file_type = traj->file_type;
	char *file_types[] = {
			"none",
			".AGP [glonass-iac.ru]",
			".AGL [glonass-iac.ru]",
	};
	
	ImGui::Combo("##file_type", &file_type, file_types, IM_ARRAYSIZE(file_types), 10);
	traj->file_type = (trj_traj_navsat_filetype_t) file_type;
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("resol ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::SliderInt("##resolution", &traj->resolution, 1, 1000);
	
	if (traj->data_offset > 0x00)
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("satnum");
		ImGui::SameLine();
		
		char satnum[32];
		sprintf(satnum, "SAT %d", traj->data_list[traj->sat_offset].satnum);
		
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		if (ImGui::BeginCombo("##satnum", satnum, ImGuiComboFlags_None))
		{
			for (int i = 0; i < traj->data_offset; ++i)
			{
				const bool is_selected = (i == traj->sat_offset);
				sprintf(satnum, "SAT %d", traj->data_list[i].satnum);
				if (ImGui::Selectable(satnum, is_selected))
				{ traj->sat_offset = i; }
				
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("time  ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	imgui_datetime("##datetime",
		 &traj->day, &traj->month, &traj->year,
		 &traj->hour, &traj->min, &traj->sec);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_traj_view_navsat(s_trj_traj *self)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) self->data;
	
	ImGui::PushID(self);
	
	if (ImGui::BeginTabBar("##data_view", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("3D view"))
		{
			s_vl3d_eng vl3d_eng;
			s_vl3d_obj *obj_list = (s_vl3d_obj*) malloc(sizeof(s_vl3d_obj) * 4096 * 4);
			s_vl3d_view view = { .pos = { 0.0, 0.0, 0.0 }, .scale = 1.0, .tbar_en = 0x01, .xyz_en = 0x01, .xyz_scale = 0.25, .grid_mode = 0x01, .grid_pt_size = 2.0, .grid_pt_disp = 2.0 };
			
			vl3d_view_load(self, &view, view);

			vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) {
					.obj_list = obj_list,
			});
			
			for (int i = 0; i < traj->data_offset; ++i)
			{
				vlf_t time = 0.0;
				vlf_t time_limit = 3600*12;
				vlf_t time_step = time_limit / 64;
				int time_iter = time_limit / time_step;
				
				vlf_t p0[3];
				vlf_t p1[3];
				vlf_t rot[9];
				
				trj_traj_navsat_pos_local(traj, time, p0, i);
				trj_traj_navsat_pos_local(traj, time, p1, i);
				
				for (int j = 0; j < time_iter; ++j)
				{
					time += time_step;

					vl_vcopy(p0, p1);
					trj_traj_navsat_pos_local(traj, time, p1, i);
					
					if (i == traj->sat_offset)
					{
						vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) {
								.color = vl3d_col_legacy,
								.p0 = { p0[0], p0[1], p0[2] },
								.p1 = { p1[0], p1[1], p1[2] },
						});
					}
					
					else
					{
						vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) {
								.color = vl3d_col_d,
								.p0 = { p0[0], p0[1], p0[2] },
								.p1 = { p1[0], p1[1], p1[2] },
						});
					}
				}
			}

			vl3d_view_grid(&view, &vl3d_eng);
			vl3d_view_xyz(&view, &vl3d_eng);
			vl3d_eng_render(&vl3d_eng, &view, "temp", ImVec2(-1, -1));
			vl3d_view_save(self, &view);

			free(obj_list);
		
			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Almanac"))
		{
			if (ImGui::Button("load"))
			{ trj_traj_navsat_compile(traj); }
			
			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 5));
			
			if (ImGui::BeginTable("table1", 17, ImGuiTableFlags_None))
			{
				ImGui::TableSetupColumn("satnum", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("health", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("week  ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("tow   ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("day   ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("month ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("year  ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("atime ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("tcorr ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("dtcorr", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("domg  ", ImGuiTableColumnFlags_WidthFixed);
				
				ImGui::TableSetupColumn("omg0  ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("i     ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("w     ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("e     ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("sqrta ", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("m0    ", ImGuiTableColumnFlags_WidthFixed);
				
				ImGui::TableHeadersRow();
				
				for (int row = 0; row < traj->data_offset; row++)
				{
					ImGui::TableNextRow();
					
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].satnum);
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].health);
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].week);
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].tow);
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].day);
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].month);
					ImGui::TableNextColumn(); ImGui::Text("%d" , traj->data_list[row].year);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].atime);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].tcorr);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].dtcorr);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].domg);
					
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].omg0);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].i);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].w);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].e);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].sqrta);
					ImGui::TableNextColumn(); ImGui::Text("%lf", traj->data_list[row].m0);
				}
				ImGui::EndTable();
			}
			
			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

#endif /* __GUI_TRAJ__ */
