
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __VL3D__
#define __VL3D__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

extern "C"
{
	#include "vl.h"
	#include "vl_svd.h"
	#include "trj_utils.h"
}

//------------------------------------------------------------------------------

inline uint32_t __vl3d_col_l__(void)
{
	ImGuiStyle& style = ImGui::GetStyle();
	return ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
}

inline uint32_t __vl3d_col_d__(void)
{
	ImGuiStyle& style = ImGui::GetStyle();
	return ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
}

#define vl3d_col_l (__vl3d_col_l__())
#define vl3d_col_d (__vl3d_col_d__())

//------------------------------------------------------------------------------

enum vl3d_obj_type
{
	vl3d_obj_type_line  = 0x01,
	vl3d_obj_type_point = 0x02,
	vl3d_obj_type_trngl = 0x03,
	vl3d_obj_type_text  = 0x04,
};


typedef struct vl3d_line
{
	uint8_t  type;
	uint8_t  spec;
	uint16_t id;
	uint32_t color;
	
	vlf_t p0[3];
	vlf_t p1[3];
	
}	s_vl3d_line;

typedef struct vl3d_point
{
	uint8_t  type;
	uint8_t  spec;
	uint16_t id;
	uint32_t color;
	
	vlf_t p0[3];
	vlf_t size;
	
}	s_vl3d_point;

typedef struct vl3d_trngl
{
	uint8_t  type;
	uint8_t  spec;
	uint16_t id;
	uint32_t color;
	
	vlf_t p0[3];
	vlf_t p1[3];
	vlf_t p2[3];
	
}	s_vl3d_trngl;

typedef struct vl3d_text
{
	uint8_t  type;
	uint8_t  spec;
	uint16_t id;
	uint32_t color;
	
	vlf_t p0[3];
	vlf_t size;
	char data[255];
	
}	s_vl3d_text;

typedef union vl3d_obj
{
	struct {
		uint8_t  type;
		uint8_t  spec;
		uint16_t id;
	};
	
	s_vl3d_line  line;
	s_vl3d_point point;
	s_vl3d_text  text;
	s_vl3d_trngl trngl;
	
} 	s_vl3d_obj;


typedef struct vl3d_eng
{
	uint32_t obj_offset;
	s_vl3d_obj *obj_list;
	
}	s_vl3d_eng;

typedef struct vl3d_eng_init
{
	s_vl3d_obj *obj_list;
	
}	s_vl3d_eng_init;

typedef struct vl3d_view
{
	vlf_t pos[3];
	vlf_t rot[3][3];
	vlf_t scale;
	
	ImRect rect;
	uint8_t tbar_en;
	
	uint8_t xyz_en;
	vlf_t xyz_scale;
	
	uint8_t grid_mode;
	vlf_t grid_pt_size;
	vlf_t grid_pt_disp;
	
} 	s_vl3d_view;

//------------------------------------------------------------------------------

inline void vl3d_view_load(void *ptr, s_vl3d_view *view, s_vl3d_view def)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	s_trj_rot_hpr hpr = {
			.heading = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), vl_rad(45)),
			.pitch 	 = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), vl_rad(45)),
			.roll    = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), vl_rad( 0)),
	};
	
	trj_hpr_to_ctn(&view->rot[0][0], hpr);
	
	view->scale = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x03)), def.scale);
	
	view->xyz_en       = window->StateStorage.GetInt  (ImGui::GetID((void*) ((uintptr_t) ptr + 0x04)), def.xyz_en);
	view->grid_mode    = window->StateStorage.GetInt  (ImGui::GetID((void*) ((uintptr_t) ptr + 0x05)), def.grid_mode);
	view->grid_pt_size = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x06)), def.grid_pt_size);
	view->grid_pt_disp = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x07)), def.grid_pt_disp);
	
	return;
}

inline void vl3d_view_save(void *ptr, s_vl3d_view *view)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	s_trj_rot_hpr hpr;
	
	trj_ctn_to_hpr(&hpr, &view->rot[0][0]);
	
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), (float) hpr.heading);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), (float) hpr.pitch);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), (float) hpr.roll);
	
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x03)), (float) view->scale);
	
	window->StateStorage.SetInt  (ImGui::GetID((void*) ((uintptr_t) ptr + 0x04)), (float) view->xyz_en);
	window->StateStorage.SetInt  (ImGui::GetID((void*) ((uintptr_t) ptr + 0x05)), (float) view->grid_mode);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x06)), (float) view->grid_pt_size);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x07)), (float) view->grid_pt_disp);
	
	return;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_init(s_vl3d_eng *self, s_vl3d_eng_init attr)
{
	self->obj_list = attr.obj_list;
	self->obj_offset = 0x00;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_add_line(s_vl3d_eng *self, s_vl3d_line obj)
{
	self->obj_list[self->obj_offset].line = obj;
	self->obj_list[self->obj_offset].type = vl3d_obj_type_line;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_add_point(s_vl3d_eng *self, s_vl3d_point obj)
{
	self->obj_list[self->obj_offset].point = obj;
	self->obj_list[self->obj_offset].type = vl3d_obj_type_point;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_add_trngl(s_vl3d_eng *self, s_vl3d_trngl obj)
{
	self->obj_list[self->obj_offset].trngl = obj;
	self->obj_list[self->obj_offset].type = vl3d_obj_type_trngl;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_add_text(s_vl3d_eng *self, s_vl3d_text obj)
{
	self->obj_list[self->obj_offset].text = obj;
	self->obj_list[self->obj_offset].type = vl3d_obj_type_text;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_draw_arrow(s_vl3d_eng *self, uint32_t color, float64_t p0[3], float64_t p1[3])
{
	vl3d_eng_add_line(self, (s_vl3d_line) {
			.p0 = { p0[0], p0[1], p0[2] },
			.p1 = { p1[0], p1[1], p1[2] },
			.color = color,
	});
	
	vl3d_eng_add_point(self, (s_vl3d_point) {
			.p0 = { p1[0], p1[1], p1[2] },
			.size = 3,
			.color = color,
	});
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline ImVec2 __vl3d_view_tf__(s_vl3d_view *view, ImVec2 pos)
{
	float32_t x = 0.5 * view->rect.GetWidth() + 0.5 * view->scale * view->rect.GetHeight() * pos.x;
	float32_t y = view->rect.GetHeight() - (0.5 * view->rect.GetHeight() + 0.5 * view->scale * view->rect.GetHeight() * pos.y);
	
	return ImVec2(x + view->rect.Min.x, y + view->rect.Min.y);
}

//inline ImVec2 __vl3d_view_tfsi__(s_vl3d_view *view, ImVec2 pos)
//{
//	float32_t x = 0.5 * view->rect.GetWidth() + 0.5 * view->scale * view->rect.GetHeight() * pos.x;
//	float32_t y = view->rect.GetHeight() - (0.5 * view->rect.GetHeight() + 0.5 * view->scale * view->rect.GetHeight() * pos.y);
//
//	return ImVec2(x + view->rect.Min.x, y + view->rect.Min.y);
//}

inline ImVec2 __vl3d_view_inv__(s_vl3d_view *view, ImVec2 pos)
{
	ImVec2 temp = ImVec2(pos.x - view->rect.Min.x, pos.y - view->rect.Min.y);
	
	float32_t x = temp.x * view->scale * 0.5 * view->rect.GetHeight();
	float32_t y = (- temp.y + view->rect.GetHeight()) * view->scale * 0.5 * view->rect.GetHeight();
	
	return ImVec2(x, y);
}

//------------------------------------------------------------------------------

inline void vl3d_view_reset(s_vl3d_eng *self, s_vl3d_view *view)
{
	if (self->obj_offset < 2)
	{
		view->scale = 1.0;
		return;
	}
	
//	vlf_t min[3] = { +INFINITY, +INFINITY, +INFINITY };
//	vlf_t max[3] = { -INFINITY, -INFINITY, -INFINITY };
//
	vlf_t min[3] = { 0.0, 0.0, 0.0 };
	vlf_t max[3] = { 0.0, 0.0, 0.0 };
	
	for (int i = 0; i < self->obj_offset; ++i)
	{
		s_vl3d_obj *obj = &self->obj_list[i];
		
		if (obj->spec != 0x00) { continue; }
		
		switch (self->obj_list[i].type)
		{
			case vl3d_obj_type_line:
			{
				if (obj->line.p0[0] < min[0]) min[0] = obj->line.p0[0];
				if (obj->line.p0[1] < min[1]) min[1] = obj->line.p0[1];
				if (obj->line.p0[2] < min[2]) min[2] = obj->line.p0[2];
				
				if (obj->line.p1[0] < min[0]) min[0] = obj->line.p1[0];
				if (obj->line.p1[1] < min[1]) min[1] = obj->line.p1[1];
				if (obj->line.p1[2] < min[2]) min[2] = obj->line.p1[2];
				
				if (obj->line.p0[0] > max[0]) max[0] = obj->line.p0[0];
				if (obj->line.p0[1] > max[1]) max[1] = obj->line.p0[1];
				if (obj->line.p0[2] > max[2]) max[2] = obj->line.p0[2];
				
				if (obj->line.p1[0] > max[0]) max[0] = obj->line.p1[0];
				if (obj->line.p1[1] > max[1]) max[1] = obj->line.p1[1];
				if (obj->line.p1[2] > max[2]) max[2] = obj->line.p1[2];
				
				break;
			}
			
			case vl3d_obj_type_point:
			{
				if (obj->point.p0[0] < min[0]) min[0] = obj->point.p0[0];
				if (obj->point.p0[1] < min[1]) min[1] = obj->point.p0[1];
				if (obj->point.p0[2] < min[2]) min[2] = obj->point.p0[2];
				
				if (obj->point.p0[0] > max[0]) max[0] = obj->point.p0[0];
				if (obj->point.p0[1] > max[1]) max[1] = obj->point.p0[1];
				if (obj->point.p0[2] > max[2]) max[2] = obj->point.p0[2];
				
				break;
			}
			
			case vl3d_obj_type_trngl:
			{
				if (obj->trngl.p0[0] < min[0]) min[0] = obj->trngl.p0[0];
				if (obj->trngl.p0[1] < min[1]) min[1] = obj->trngl.p0[1];
				if (obj->trngl.p0[2] < min[2]) min[2] = obj->trngl.p0[2];
				
				if (obj->trngl.p1[0] < min[0]) min[0] = obj->trngl.p1[0];
				if (obj->trngl.p1[1] < min[1]) min[1] = obj->trngl.p1[1];
				if (obj->trngl.p1[2] < min[2]) min[2] = obj->trngl.p1[2];
				
				if (obj->trngl.p2[0] < min[0]) min[0] = obj->trngl.p2[0];
				if (obj->trngl.p2[1] < min[1]) min[1] = obj->trngl.p2[1];
				if (obj->trngl.p2[2] < min[2]) min[2] = obj->trngl.p2[2];
				
				if (obj->trngl.p0[0] > max[0]) max[0] = obj->trngl.p0[0];
				if (obj->trngl.p0[1] > max[1]) max[1] = obj->trngl.p0[1];
				if (obj->trngl.p0[2] > max[2]) max[2] = obj->trngl.p0[2];
				
				if (obj->trngl.p1[0] > max[0]) max[0] = obj->trngl.p1[0];
				if (obj->trngl.p1[1] > max[1]) max[1] = obj->trngl.p1[1];
				if (obj->trngl.p1[2] > max[2]) max[2] = obj->trngl.p1[2];
				
				if (obj->trngl.p2[0] > max[0]) max[0] = obj->trngl.p2[0];
				if (obj->trngl.p2[1] > max[1]) max[1] = obj->trngl.p2[1];
				if (obj->trngl.p2[2] > max[2]) max[2] = obj->trngl.p2[2];
				
				break;
			}
			
			default: { break; }
		}
	}
	
//	printf(" min "); vl_vprint(min);
//	printf(" max "); vl_vprint(max);
//	fflush(stdout);
	
	vlf_t dist = vl_dist(min, max);
	view->scale = 1 / dist;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_render(s_vl3d_eng *self, s_vl3d_view *view, char *label, ImVec2 size)
{
	ImGuiContext& context = *ImGui::GetCurrentContext();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	ImGuiID id = parent_window->GetID(label);
	
	size.x = size.x < 0 ? ImGui::GetContentRegionAvail().x : size.x;
	size.y = size.y < 0 ? ImGui::GetContentRegionAvail().y : size.y;
	
	ImGui::BeginChildFrame(id, size,
						   ImGuiWindowFlags_NoScrollbar |
						   ImGuiWindowFlags_NoScrollWithMouse);
	if (view->tbar_en != 0x00)
	{
		// Toolbar
		
		ImGui::BeginGroup();
		ImGui::AlignTextToFramePadding();
		
		{
			ImGui::Dummy(ImVec2(5, 0));
			ImGui::SameLine();
			ImGui::Text("XYZ");
			
			ImGui::SameLine();
			ImGui::PushID("##xyz_en");
			const char *xyz_en_label[2] = { "OFF", "ON" };
			if (ImGui::Button(xyz_en_label[view->xyz_en], ImVec2(40, 0)))
			{ view->xyz_en = !view->xyz_en; }
			ImGui::PopID();
		}
		
		{
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(5, 0));
			ImGui::SameLine();
			ImGui::Text("GRID");

//			ImGui::SameLine();
//			ImGui::SetNextItemWidth(80);
//			ImGui::DragScalar("##grid_pt_size", ImGuiDataType_Double,
//							  &view->grid_pt_size, 0.001, NULL, NULL, "%.3f");
			
			const vlf_t grid_pt_disp_min = 0.1;
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80);
			ImGui::DragScalar("##grid_pt_disp", ImGuiDataType_Double,
							  &view->grid_pt_disp, 0.01, &grid_pt_disp_min, NULL, "%.2f");
			
			ImGui::SameLine(0.0, 0.0);
			ImGui::PushID("##grid_en");
			const char *grid_mode_label[3] = { "OFF", "DOT", "LINE" };
			if (ImGui::Button(grid_mode_label[view->grid_mode], ImVec2(40, 0)))
			{ ++view->grid_mode; view->grid_mode %= 3; }
			ImGui::PopID();
		}
		
		{
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(5, 0));
			ImGui::SameLine();
			const vlf_t scale_min = 1E-9;
			ImGui::Text("SCALE");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80);
			ImGui::DragScalar("##scale", ImGuiDataType_Double,
							  &view->scale, 0.001, &scale_min, NULL, "%.3f");
			ImGui::SameLine(0.0, 0.0);
			
			if (ImGui::Button("AUTO"))
			{ vl3d_view_reset(self, view); }
		}
		
		ImGui::EndGroup();
		
//		ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), col_text_u32);
	}
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	bool view_hovered = ImGui::IsWindowHovered();
	
	vlf_t e0[3] = { -view->rot[0][2], view->rot[1][2], view->rot[2][2] };
	vlf_t e1[3] = { -view->rot[0][1], view->rot[1][1], view->rot[2][1] };
	vlf_t e2[3] = { -view->rot[0][0], view->rot[1][0], view->rot[2][0] };

//	vl_mmul_v(view_pos, &view->rot[0][0], view->pos);
	
	view->rect = window->InnerRect;
	
	// Handle user interaction before rendering to reduce lag
	
	if (io.MouseWheel != 0x00 && view_hovered
		&& !ImGui::IsMouseDragging(ImGuiMouseButton_Left)
		&& !ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
//		ImVec2 scale_point = inv_transform(&view, ImGui::GetMousePos());
//		float32_t scroll_delta = io.MouseWheel * 0.1f;
//
//		if (scroll_delta > +0.5) scroll_delta = +0.5;
//		if (scroll_delta < -0.5) scroll_delta = -0.5;
//
//		view.p0 = scale_point + (view.p0 - scale_point) * (1.0f - scroll_delta);
//		view.p1 = scale_point + (view.p1 - scale_point) * (1.0f - scroll_delta);
		
		view->scale *= 1.0 + 0.01 * io.MouseWheel;
	}
	
	if (view_hovered
		&& ImGui::IsMouseDragging(ImGuiMouseButton_Left) && !io.KeyCtrl)
	{
		vlf_t mouse_drag_delta[3] = {
				0.0,
				+0.01 * io.MouseDelta.x,
				-0.01 * io.MouseDelta.y,
		};
		
		mouse_drag_delta[2] = -mouse_drag_delta[2];
		
		// convert to skew sim matrix for rotation
		vlf_t d_rot_t[3][3]; // tied
		vlf_t d_rot[3][3];
		
		vl_skew(&d_rot_t[0][0], mouse_drag_delta);
		
		// Rotate original matrix C = C + C*d_rot;
		vl_mmul_m(&d_rot[0][0], &view->rot[0][0], &d_rot_t[0][0]);
		vl_msum(&view->rot[0][0], &view->rot[0][0], &d_rot[0][0]);
		
		// Normalize matrix
		vl_rnorm(&view->rot[0][0]);
	}

//	if (view_hovered
//	&& ImGui::IsMouseDragging(ImGuiMouseButton_Left) && io.KeyCtrl)
//	{
//		vlf_t mouse_drag_delta[3] = {
//				0.0,
//				-io.MouseDelta.y / (view->scale * view->rect.GetHeight() * 0.5),
//				+io.MouseDelta.x / (view->scale * view->rect.GetWidth() * 0.5),
//		};
//
//		vlf_t d_pos[3];
//
//		vl_mmul_v(d_pos, &view->rot[0][0], mouse_drag_delta);
//		vl_vsub(view->pos, view->pos, d_pos);
//	}
	
	for (int i = 0; i < self->obj_offset; ++i)
	{
		s_vl3d_obj *obj = &self->obj_list[i];
		
		switch (obj->type)
		{
			case vl3d_obj_type_line:
			{
				vlf_t l_p0[3] = {
						vl_vdot(obj->line.p0, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->line.p0, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->line.p0, e2) - vl_vdot(view->pos, e2),
				};
				
				vlf_t l_p1[3] = {
						vl_vdot(obj->line.p1, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->line.p1, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->line.p1, e2) - vl_vdot(view->pos, e2),
				};
				
				if (l_p0[2] > 0 || l_p1[2] > 0 || 0x01)
				{
					window->DrawList->AddLine(
							__vl3d_view_tf__(view, ImVec2(l_p0[0], l_p0[1])),
							__vl3d_view_tf__(view, ImVec2(l_p1[0], l_p1[1])),
							obj->line.color);
				}
				
				break;
			}
			
			case vl3d_obj_type_point:
			{
				vlf_t l_p0[3] = {
						vl_vdot(obj->point.p0, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->point.p0, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->point.p0, e2) - vl_vdot(view->pos, e2),
				};
				
				if (l_p0[2] > 0 || 0x01)
				{
					window->DrawList->AddCircleFilled(
							__vl3d_view_tf__(view, ImVec2(l_p0[0], l_p0[1])),
							obj->point.size,
							obj->point.color);
				}
				
				break;
			}
			
			case vl3d_obj_type_trngl:
			{
				vlf_t l_p0[3] = {
						vl_vdot(obj->trngl.p0, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->trngl.p0, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->trngl.p0, e2) - vl_vdot(view->pos, e2),
				};
				
				vlf_t l_p1[3] = {
						vl_vdot(obj->trngl.p1, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->trngl.p1, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->trngl.p1, e2) - vl_vdot(view->pos, e2),
				};
				
				vlf_t l_p2[3] = {
						vl_vdot(obj->trngl.p2, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->trngl.p2, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->trngl.p2, e2) - vl_vdot(view->pos, e2),
				};
				
				if (l_p0[2] > 0 || l_p1[2] > 0 || 0x01)
				{
					window->DrawList->AddTriangleFilled(
							__vl3d_view_tf__(view, ImVec2(l_p0[0], l_p0[1])),
							__vl3d_view_tf__(view, ImVec2(l_p1[0], l_p1[1])),
							__vl3d_view_tf__(view, ImVec2(l_p2[0], l_p2[1])),
							obj->trngl.color);
				}
				
				break;
			}
			
			case vl3d_obj_type_text:
			{
				vlf_t l_p0[3] = {
						vl_vdot(obj->text.p0, e0) - vl_vdot(view->pos, e0),
						vl_vdot(obj->text.p0, e1) - vl_vdot(view->pos, e1),
						vl_vdot(obj->text.p0, e2) - vl_vdot(view->pos, e2),
				};
				
				if (l_p0[2] > 0 || 0x01)
				{
					window->DrawList->AddText(
							__vl3d_view_tf__(view, ImVec2(l_p0[0], l_p0[1])),
							obj->text.color, obj->text.data);
				}
				
				break;
			}
		}
	}
	
	ImGui::EndChildFrame();
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_view_grid(s_vl3d_view *self, s_vl3d_eng *eng)
{
	if (self->grid_mode == 0x00) { return 0x00; }
	
	uint32_t spec_start = eng->obj_offset;
	
	int cnt = 5;
	vlf_t d = 1.0 / self->scale / 4.0;
	vlf_t pt_norm = vl_gauss1(0.0, 0.0, self->grid_pt_disp);
	vlf_t gain;
	ImVec4 color_v4;
	
	s_vl3d_point point;
	s_vl3d_line  line;
	
	switch (self->grid_mode)
	{
		case 0x01:
		{
			for (int x = 0; x < cnt; ++x)
			{
				for (int y = 0; y < cnt; ++y)
				{
					for (int z = 0; z < cnt; ++z)
					{
						gain = vl_sqrt(x * x + y * y + z * z);
						gain = 0.4 * vl_gauss1(gain, 0.0, self->grid_pt_disp) / pt_norm;
						
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						point.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						point.size = self->grid_pt_size;
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, y * d, z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, y * d, -z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, -y * d, z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, -y * d, -z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, y * d, z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, y * d, -z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, -y * d, z * d});
						vl3d_eng_add_point(eng, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, -y * d, -z * d});
						vl3d_eng_add_point(eng, point);
					}
				}
			}
			
			break;
		}
		
		case 0x02:
		{
			for (int x = 0; x < cnt; ++x)
			{
				for (int y = 0; y < cnt; ++y)
				{
					for (int z = 0; z < cnt; ++z)
					{
						gain = vl_sqrt(x * x + y * y + z * z);
						// 0.2 coz its too bright in (0,0)
						gain = 0.2 * vl_gauss1(gain, 0.0, self->grid_pt_disp) / pt_norm;
						
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						line.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						
						// 1
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, d*y, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y + d, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y, d*z + d });
						vl3d_eng_add_line(eng, line);
						
						// 2
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, d*y, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y + d, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y, -d*z - d });
						vl3d_eng_add_line(eng, line);
						
						// 3
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, -d*y, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y - d, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y, d*z + d });
						vl3d_eng_add_line(eng, line);
						
						// 4
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, -d*y, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y - d, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y, -d*z - d });
						vl3d_eng_add_line(eng, line);
						
						// 5
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, d*y, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y + d, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y, d*z + d });
						vl3d_eng_add_line(eng, line);
						
						// 6
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, d*y, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y + d, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y, -d*z - d });
						vl3d_eng_add_line(eng, line);
						
						// 7
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, -d*y, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y - d, d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y, d*z + d });
						vl3d_eng_add_line(eng, line);
						
						// 8
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, -d*y, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y - d, -d*z });
						vl3d_eng_add_line(eng, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y, -d*z - d });
						vl3d_eng_add_line(eng, line);
					}
				}
			}
			
			break;
		}
	}
	
	for (int i = spec_start; i < eng->obj_offset; ++i)
	{
		eng->obj_list[i].spec = 0x01;
	}
	
	return 0x00;
}

inline uint8_t vl3d_view_xyz(s_vl3d_view *self, s_vl3d_eng *eng)
{
 	if (self->xyz_en == 0x00) { return 0x00; }
	
	vlf_t ratio = self->xyz_scale / self->scale;
	
 	uint32_t spec_start = eng->obj_offset;
 
	vl3d_eng_draw_arrow(eng, vl3d_col_l, (float64_t[]) { -ratio, +0.0, +0.0 }, (float64_t[]) { +ratio, +0.0, +0.0 } );
	vl3d_eng_draw_arrow(eng, vl3d_col_l, (float64_t[]) { +0.0, -ratio, +0.0 }, (float64_t[]) { +0.0, +ratio, +0.0 } );
	vl3d_eng_draw_arrow(eng, vl3d_col_l, (float64_t[]) { +0.0, +0.0, -ratio }, (float64_t[]) { +0.0, +0.0, +ratio } );
	
	s_vl3d_text x_label = { .color = vl3d_col_l, .p0 = { ratio, 0.0, 0.0 } };
	s_vl3d_text y_label = { .color = vl3d_col_l, .p0 = { 0.0, ratio, 0.0 } };
	s_vl3d_text z_label = { .color = vl3d_col_l, .p0 = { 0.0, 0.0, ratio } };
	
	sprintf(x_label.data, "X [%.3Lf]", ratio);
	sprintf(y_label.data, "Y [%.3Lf]", ratio);
	sprintf(z_label.data, "Z [%.3Lf]", ratio);
	
	vl3d_eng_add_text(eng, x_label);
	vl3d_eng_add_text(eng, y_label);
	vl3d_eng_add_text(eng, z_label);
	
	for (int i = spec_start; i < eng->obj_offset; ++i)
	{
		eng->obj_list[i].spec = 0x01;
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __VL__ */




























