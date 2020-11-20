
#ifndef __TRJ_GUI_TRAJ__
#define __TRJ_GUI_TRAJ__

#define IMGUI_DEFINE_MATH_OPERATORS

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>
#include <math.h>

#include <lib/trj/vl.h>
#include <lib/trj/vl3d.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_traj.h>

#include "trj_gui_w.h"

//------------------------------------------------------------------------------

enum class CurveEditorFlags
{
	NO_TANGENTS = 1 << 0,
	SHOW_GRID = 1 << 1
};

//------------------------------------------------------------------------------

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

int CurveEditor(const char* label
		, float* values
		, int points_count
		, ImVec2 size
		, ImU32 flags
		, int* new_count);

//------------------------------------------------------------------------------

void trj_gui_traj_edit(s_trj_traj *self);

void trj_gui_traj_bz_edit(s_trj_traj *self);
void trj_gui_traj_bz_view(s_trj_traj_bz *self, const char* label, ImVec2 size, bool view_res);

void trj_gui_traj_orb_edit(s_trj_traj *self);
void trj_gui_traj_orb_view(s_trj_traj *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_TRAJ__ */
