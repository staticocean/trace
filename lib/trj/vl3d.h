
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

#define vl3d_type_line (0x80000000)

//------------------------------------------------------------------------------

enum vl_obj_type
{
	vl_obj_type_line  = 0x01,
	vl_obj_type_point = 0x02,
	vl_obj_type_text  = 0x03,
};


typedef struct vl3d_line
{
	uint16_t type;
	uint16_t id;
	
	vlf_t p0[3];
	vlf_t p1[3];
	
}	s_vl3d_line;

typedef struct vl3d_point
{
	uint16_t type;
	uint16_t id;
	
	vlf_t p0[3];
	vlf_t size;
	
}	s_vl3d_point;

typedef struct vl3d_text
{
	uint16_t type;
	uint16_t id;
	
	vlf_t p0[3];
	vlf_t size;
	char data[255];
	
}	s_vl3d_text;

typedef union vl3d_obj
{
	struct {
		uint16_t type;
		uint16_t id;
	};
	
	s_vl3d_line  line;
	s_vl3d_point point;
	s_vl3d_text  text;
	
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
	self->obj_list[self->obj_offset].type = vl_obj_type_line;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_add_point(s_vl3d_eng *self, s_vl3d_point obj)
{
	self->obj_list[self->obj_offset].point = obj;
	self->obj_list[self->obj_offset].type = vl_obj_type_point;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_add_text(s_vl3d_eng *self, s_vl3d_text obj)
{
	self->obj_list[self->obj_offset].text = obj;
	self->obj_list[self->obj_offset].type = vl_obj_type_text;
	self->obj_list[self->obj_offset].id = self->obj_offset;
	self->obj_offset++;
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_draw_arrow(s_vl3d_eng *self, float64_t p0[3], float64_t p1[3])
{
	vl3d_eng_add_line(self, (s_vl3d_line) {
			.p0 = { p0[0], p0[1], p0[2] },
			.p1 = { p1[0], p1[1], p1[2] },
	});
	
	vl3d_eng_add_point(self, (s_vl3d_point) {
			.p0 = { p1[0], p1[1], p1[2] },
			.size = 3,
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

inline ImVec2 __vl3d_view_inv__(s_vl3d_view *view, ImVec2 pos)
{
	ImVec2 temp = ImVec2(pos.x - view->rect.Min.x, pos.y - view->rect.Min.y);
	
	float32_t x = temp.x * view->scale * 0.5 * view->rect.GetHeight();
	float32_t y = (- temp.y + view->rect.GetHeight()) * view->scale * 0.5 * view->rect.GetHeight();
	
	return ImVec2(x, y);
}

//------------------------------------------------------------------------------

inline uint8_t vl3d_eng_render(s_vl3d_eng *self, s_vl3d_view *view, char *label, ImVec2 size)
{
	ImGuiContext& context = *ImGui::GetCurrentContext();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	
	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
	
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	ImGuiID id = parent_window->GetID(label);
	
	size.x = size.x < 0 ? ImGui::GetContentRegionAvail().x : size.x;
	size.y = size.y < 0 ? ImGui::GetContentRegionAvail().y : size.y;
	
	ImGui::BeginChildFrame(id, size,
						   ImGuiWindowFlags_NoScrollbar |
						   ImGuiWindowFlags_NoScrollWithMouse);
	
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
			case vl_obj_type_line:
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
							col_text_u32);
				}
				
				break;
			}
			
			case vl_obj_type_point:
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
							col_text_u32);
				}
				
				break;
			}
			
			case vl_obj_type_text:
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
							col_text_u32, obj->text.data);
				}
				
				break;
			}
		}
	}
	
	ImGui::EndChildFrame();
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __VL__ */




























