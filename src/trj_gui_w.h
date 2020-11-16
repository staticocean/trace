
#ifndef __TRJ_GUI_W__
#define __TRJ_GUI_W__

#include <lib/imgui/imgui.h>

extern "C"
{
	#include <lib/trj/trj_types.h>
	#include <lib/trj/trj_obj.h>
	#include <lib/trj/trj_utils.h>
	#include <lib/trj/vl.h>
}

#include <lib/imgui/imgui_internal.h>
#include <lib/trj/vl3d.h>

//------------------------------------------------------------------------------

uint8_t vl_gui_vec(char *label, vlf_t *vec, float v_speed, vlf_t *min, vlf_t *max, char *format);
uint8_t vl_gui_hpr(char *label, s_trj_rot_hpr *hpr, float v_speed, char *format);
uint8_t vl_gui_mat(char *label, vlf_t *mat, float v_speed, vlf_t *min, vlf_t *max, char *format);
uint8_t vl_gui_rot(char *label, vlf_t *mat);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_OBJ__ */
