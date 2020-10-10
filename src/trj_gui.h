
#ifndef __TRJ_GUI__
#define __TRJ_GUI__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/imgui/imgui.h"

//#include "lib/tcc/libtcc.h"
#include "lib/picoc/picoc.h"

#include "lib/trj/trj_eng.h"
#include "lib/trj/trj_obj.h"
#include "lib/trj/trj_bz.h"
#include "lib/trj/vl3d.h"

#include "trj_gui_eng.h"
#include "trj_gui_obj.h"
#include "trj_gui_menu.h"
#include "trj_gui_traj.h"

//------------------------------------------------------------------------------

typedef struct trj_gui
{
	float32_t w_height;
	float32_t w_width;
	
	s_trj_eng eng;
	s_trj_obj st_eng_obj[255];
	s_trj_gui_obj st_gui_eng_obj[255];
	
	s_trj_gui_menu gui_menu;
	s_trj_gui_eng  gui_eng;
	
} 	s_trj_gui;

typedef struct trj_gui_init_attr
{

}	s_trj_gui_init_attr;

//------------------------------------------------------------------------------

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init_attr attr);
uint8_t trj_gui_main(s_trj_gui *self);

//------------------------------------------------------------------------------

int tcc_test(void);

/*

 //
 //  main.c
 //  ins
 //
 //  Created by John Brown on 11/9/19.
 //  Copyright � 2019 John Brown. All rights reserved.
 //

 //------------------------------------------------------------------------------

 #include <stdio.h>

 #include "types.h"
 #include "vl.h"

 #include "trj_eng.h"
 #include "ins_obj.h"
 #include "ins_ctrl.h"
 #include "ins_proc.h"

 //------------------------------------------------------------------------------

 int main(int argc, const char * argv[])
 {
 s_trj_rot_hpr earth_hpr =
 {
 .heading = 0.0,
 .pitch = 0.0,
 .roll = vl_rad(23.44),
 };

 s_trj_obj obj_earth =
 {
 .ref = NULL,

 .pos_p =
 {
 { 0, 0, 0 },
 { 0, 0, 0 },
 { 0, 0, 0 },
 },

 .rot =
 {
 {
 1, 0, 0,
 0, 1, 0,
 0, 0, 1,
 },
 {
 0, 0, 0,
 0, 0, 0,
 0, 0, 0
 },
 {
 0, 0, 0,
 0, 0, 0,
 0, 0, 0
 },
 },

 .pos_inert = 5.9722E+24,
 .rot_inert = 1,

 .pos_force = { 0, 0, 0 },
 .rot_force = { 0, 0, 0 },

 .time = { 0, 0, 0 },
 };

 trj_hpr_to_ctn(obj_earth.rot[0], &earth_hpr);
 vlf_t earth_rot_t[3] = { 0.0, vl_rad((15.0) / 3600), 0.0 };
 vl_mmul_m(obj_earth.rot[1], obj_earth.rot[0], earth_rot_t);

 ins_ctrl_upos_t earth_ctrl =
 {
 .parent = &obj_earth,
 };

 obj_earth.ctrl_data = &earth_ctrl;
 obj_earth.ctrl_update = ins_ctrl_upos_update;
 obj_earth.proc_data = NULL;
 obj_earth.proc_update = NULL;

 s_trj_obj obj_system =
 {
 .ref = &obj_earth,

 .pos_p =
 {
 { 0, 0, 0 },
 { 0, 0, 0 },
 { 0, 0, 0 },
 },

 .rot =
 {
 {
 1, 0, 0,
 0, 1, 0,
 0, 0, 1,
 },
 {
 0, 0, 0,
 0, 0, 0,
 0, 0, 0
 },
 {
 0, 0, 0,
 0, 0, 0,
 0, 0, 0
 },
 },

 .pos_inert = 1,
 .rot_inert = 1,

 .pos_force = { 0, 0, 0 },
 .rot_force = { 0, 0, 0 },

 .time = { 0, 0, 0 },
 };

 ins_ctrl_uins_t system_ctrl =
 {
 .parent = &obj_system,
 };

 obj_system.ctrl_data = &system_ctrl;
 obj_system.ctrl_update = ins_ctrl_uins_update;
 obj_system.proc_data = NULL;
 obj_system.proc_update = NULL;

 trj_eng_t engine;
 engine.obj_list[0] = &obj_earth;
 engine.obj_list[1] = &obj_system;
 engine.obj_count = 0x02;

 uint32_t i;

 vlf_t step = 0.1;

 for (i = 0; i < 2; ++i)
 {
 //        printf("earth force \n");
 //        vl_vprint(obj_earth.pos_force);
 //
 //        printf("earth pos_p \n");
 //        vl_vprint(obj_earth.pos_0);
 //        vl_vprint(obj_earth.pos_1);
 //        vl_vprint(obj_earth.pos_2);
 //
 //        printf("earth rot \n");
 //        vl_mprint(obj_earth.rot_0);
 //        vl_vprint(obj_earth.rot_1);
 //        vl_vprint(obj_earth.rot_2);
 //
 //        printf("\n");

 trj_pos_ecef_t pos_ecef;
 trj_pos_ecef(&pos_ecef, &obj_system);
 trj_pos_lla_t pos_lla;
 trj_ecef_to_lla(&pos_lla, &pos_ecef);

 printf("lat %f lon %f \n", vl_deg(pos_lla.lat), vl_deg(pos_lla.lon));

 obj_system.pos_p[2][0] = 0.0;
 obj_system.pos_p[2][1] = 0.0;
 obj_system.pos_p[2][2] = 0.0;

 obj_system.rot[2][0] = 0.0;
 obj_system.rot[2][1] = 0.0;
 obj_system.rot[2][2] = 0.0;

 printf("system force \n");
 vl_vprint(obj_system.pos_force);

 printf("system pos_p \n");
 vl_vprint(obj_system.pos_p[0]);
 vl_vprint(obj_system.pos_p[1]);
 vl_vprint(obj_system.pos_p[2]);

 printf("system rot \n");
 vl_mprint(obj_system.rot[0]);
 vl_vprint(obj_system.rot[1]);
 vl_vprint(obj_system.rot[2]);

 printf("\n");

 trj_engine_update(&engine, step);
 }

 return 0x00;
 }

 //------------------------------------------------------------------------------

*/


#endif
