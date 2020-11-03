
#ifndef __TRJ_GUI_ENG__
#define __TRJ_GUI_ENG__

#include <lib/imgui/imgui.h>

extern "C"
{
	#include <lib/trj/trj_types.h>
	#include <lib/trj/trj_eng.h>
	#include <lib/trj/trj_traj.h>
	#include <lib/trj/trj_ctrl.h>
}

#include "trj_gui_obj.h"
#include "trj_gui_traj.h"

//------------------------------------------------------------------------------

enum trj_gui_eng_type_t
{
	trj_gui_eng_type_obj,
	trj_gui_eng_type_traj,
	trj_gui_eng_type_ctrl,
	trj_gui_eng_type_data,
	trj_gui_eng_type_proc,
};

typedef struct trj_gui_eng
{
	float64_t 			time_limit;
	float64_t 			time_step;
	uint32_t 			time_iter;
	
	s_trj_traj 			traj_list[32];
	uint32_t       		traj_offset;
	
	s_trj_ctrl    		ctrl_list[32];
	uint32_t       		ctrl_offset;
	
	s_trj_data    		data_list[32];
	uint32_t       		data_offset;
	
	void* sel_item;
	trj_gui_eng_type_t sel_type;

	s_trj_gui_obj *obj_list;
	
}	s_trj_gui_eng;

typedef struct trj_gui_eng_init
{
	s_trj_gui_obj *obj_list;

} 	s_trj_gui_eng_init;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

uint8_t trj_gui_eng_init(s_trj_gui_eng *gui, s_trj_gui_eng_init attr);
uint8_t trj_gui_eng_add_trajapi(s_trj_gui_eng *gui, s_trj_traj api);
uint8_t trj_gui_eng_add_ctrlapi(s_trj_gui_eng *gui, s_trj_ctrl api);
uint8_t trj_gui_eng_add_dataapi(s_trj_gui_eng *gui, s_trj_data api);
uint8_t trj_gui_eng_objlist(s_trj_gui_eng *gui, s_trj_eng *self);
uint8_t trj_gui_eng_addbox(s_trj_gui_eng *gui, s_trj_eng *self);

void trj_gui_eng_sel_obj(s_trj_gui_eng *gui, s_trj_obj *obj);
void trj_gui_eng_sel_traj(s_trj_gui_eng *gui, s_trj_traj *traj);
void trj_gui_eng_sel_ctrl(s_trj_gui_eng *gui, s_trj_ctrl *ctrl);
void trj_gui_eng_sel_data(s_trj_gui_eng *gui, s_trj_data *data);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_ENG__ */
