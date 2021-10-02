
#ifndef __GUI__
#define __GUI__

//----------------------------------------------------------------

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <res/fonts/default_font.h>

#include <libgui/imgui/imgui.h>
#include <lib/picoc/picoc.h>
#include <nfd.h>
#include <libcommon/vl.h>
#include <libcommon/imgui_theme.h>
#include <libcommon/imgui_w.h>

#include <lib/trj/trj_eng.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_ellp.h>
#include <lib/trj/trj_traj.h>
#include <lib/trj/trj_traj_.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_proc.h>

#include "gui_eng.h"
#include "gui_obj.h"
#include "gui_map.h"
#include "gui_tbar.h"
#include "gui_traj.h"
#include "gui_ctrl.h"
#include "gui_data.h"
#include "gui_cmd.h"
#include "gui_env.h"
#include "gui_clip.h"

//----------------------------------------------------------------

typedef struct gui
{
	float32_t w_height;
	float32_t w_width;
	
	s_trj_eng eng;
	s_trj_obj  st_eng_obj_list [255];
	s_trj_ellp st_eng_ellp_list[255];
	s_trj_traj st_eng_traj_list[255];
	s_trj_ctrl st_eng_ctrl_list[255];
	s_trj_data st_eng_data_list[255];
	s_trj_proc st_eng_proc_list[255];
	
	s_gui_obj st_gui_eng_obj[255];
	
	s_gui_tbar gui_tbar;
	s_gui_eng  gui_eng;
	s_gui_cmd  gui_cmd;
	s_gui_env  gui_env;
	s_gui_clip gui_clip;
	s_gui_conf gui_conf;
	
} 	s_gui;

typedef struct gui_init_attr
{
	uint32_t temp;
	
}	s_gui_init;

//----------------------------------------------------------------

//static 	s_gui_map map;

inline uint8_t gui_init(s_gui *self, s_gui_init attr)
{
    gui_clip_init(&self->gui_clip, (s_gui_clip_attr) {});

	// Init Native File Dialog Extended library
	NFD_Init();

//	s_gui_map *temp_map = new s_gui_map();
//	map = *temp_map;
//	free(temp_map);

//	gui_map_load(&map, "res/maps/earth/countries.geojson");
//	gui_map_load(&map, "res/maps/earth/cities.geojson");
	
	self->w_height = 720;
	self->w_width  = 1024;

	gui_conf_init(&self->gui_conf, (s_gui_conf_attr) {
	    .eng = &self->eng,
	});

	self->gui_tbar.eng  = &self->eng;
	self->gui_tbar.eng_gui = &self->gui_eng;
	self->gui_tbar.env  = &self->gui_env;
    self->gui_tbar.cmd  = &self->gui_cmd;
    self->gui_tbar.conf = &self->gui_conf;
	self->gui_tbar.height = 40;
	sprintf(self->gui_tbar.file_path, "res/saves/default.trj");
	
	gui_eng_init(&self->gui_eng, (s_gui_eng_init) {
	    .obj_list = self->st_gui_eng_obj,
	    .gui_clip = &self->gui_clip,
    });
	
	trj_eng_init(&self->eng, (s_trj_eng_init) {
			
			.proc      = self->st_eng_proc_list[0],
			
			.obj_list  = self->st_eng_obj_list,
			
			.ellp_list = self->st_eng_ellp_list,
			.traj_list = self->st_eng_traj_list,
			.ctrl_list = self->st_eng_ctrl_list,
			.data_list = self->st_eng_data_list,
			.proc_list = self->st_eng_proc_list,
			
			.time_limit = 3600.0,
			.time_step = 0.01,
	});
	
	trj_ellp_init(&trj_ellp_wgs84);
	trj_ellp_init(&trj_ellp_pz90);
	
	trj_eng_add_ellpapi(&self->eng, trj_ellp_wgs84);
	trj_eng_add_ellpapi(&self->eng, trj_ellp_pz90);
	
	static s_trj_traj_static_init trj_traj_static_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.ellp_en = 0x00,
			.ellp = NULL,
			
			.pos = { 0, 0, 0 },
			.rot = { 1, 0, 0, 0, 1, 0, 0, 0, 1 },
	};
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			
			.desc = "default_traj_static",
			.name = "default_traj_static",
			
			.config_size = sizeof(s_trj_traj_static_init),
			.config = &trj_traj_static_config_,
			
			.data_size = sizeof(s_trj_traj_static),
			.data = NULL,
			
			.init = trj_traj_static_init_,
			.free = trj_traj_static_free_,
			.save = trj_traj_static_save_,
			.load = trj_traj_static_load_,
			.compile = trj_traj_static_compile_,
			.info = trj_traj_static_info_,
			.pos  = trj_traj_static_pos_,
			.rot  = trj_traj_static_rot_,
	});
	
	static s_trj_traj_orb_init trj_traj_orb_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.sync_en = 0x00,
			
			.radius = 0.0,
			.rate = 0.0,
			
			.s_rate = 0.0,
	};
	
	vl3_mid(trj_traj_orb_config_.tilt);
	vl3_mid(trj_traj_orb_config_.s_tilt);
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			
			.desc = "default_traj_orb",
			.name = "default_traj_orb",
			
			.config_size = sizeof(s_trj_traj_orb_init),
			.config = &trj_traj_orb_config_,
			
			.data_size = sizeof(s_trj_traj_orb),
			.data = NULL,
			
			.init = trj_traj_orb_init_,
			.free = trj_traj_orb_free_,
			.save = trj_traj_orb_save_,
			.load = trj_traj_orb_load_,
			.compile = trj_traj_orb_compile_,
			.info = trj_traj_orb_info_,
			.pos = trj_traj_orb_pos_,
			.rot = trj_traj_orb_rot_,
	});
	
	static s_trj_traj_bz_init trj_traj_bz_config_ = {
			
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.ellp_en = 0x00,
			.ellp = NULL,
			
			.pts = NULL,
	};
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			
			.desc = "default_traj_bz",
			.name = "default_traj_bz",
			
			.config_size = sizeof(s_trj_traj_bz_init),
			.config = &trj_traj_bz_config_,
			
			.data_size = sizeof(s_trj_traj_bz),
			.data = NULL,
			
			.init = trj_traj_bz_init_,
			.free = trj_traj_bz_free_,
			.save = trj_traj_bz_save_,
			.load = trj_traj_bz_load_,
			.compile = trj_traj_bz_compile_,
			.info = trj_traj_bz_info_,
			.pos = trj_traj_bz_pos_,
			.rot = trj_traj_bz_rot_,
	});
	
	static s_trj_traj_bz_init trj_traj_bz2_config_ = {
			
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.ellp_en = 0x00,
			.ellp = NULL,
			
			.pts = NULL,
	};
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			
			.desc = "default_traj_bz2",
			.name = "default_traj_bz2",
			
			.config_size = sizeof(s_trj_traj_bz_init),
			.config = &trj_traj_bz_config_,
			
			.data_size = sizeof(s_trj_traj_bz),
			.data = NULL,
			
			.init = trj_traj_bz_init_,
			.free = trj_traj_bz_free_,
			.save = trj_traj_bz_save_,
			.load = trj_traj_bz_load_,
			.compile = trj_traj_bz_compile_,
			.info = trj_traj_bz_info_,
			.pos = trj_traj_bz_pos_,
			.rot = trj_traj_bz_rot_,
	});
	
	static s_trj_traj_navsat_init trj_traj_navsat_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
	};
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			
			.desc = "default_traj_navsat",
			.name = "default_traj_navsat",
			
			.config_size = sizeof(s_trj_traj_navsat_init),
			.config = &trj_traj_navsat_config_,
			
			.data_size = sizeof(s_trj_traj_navsat),
			.data = NULL,
			
			.init = trj_traj_navsat_init_,
			.free = trj_traj_navsat_free_,
			.save = trj_traj_navsat_save_,
			.load = trj_traj_navsat_load_,
			.compile = trj_traj_navsat_compile_,
			.info = trj_traj_navsat_info_,
			.pos = trj_traj_navsat_pos_,
			.rot = trj_traj_navsat_rot_,
	});
	
	static s_trj_ctrl_upos_init trj_ctrl_upos_config_ = {
	
	};
	
	static s_trj_ctrl_cpos_init trj_ctrl_cpos_config_ = {
	
	};
	
	static s_trj_ctrl_urot_init trj_ctrl_urot_config_ = {
	
	};
	
	static s_trj_ctrl_crot_init trj_ctrl_crot_config_ = {
	
	};
	
//	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
//
//			.desc   = "default_ctrl_upos",
//			.name   = "default_ctrl_upos",
//
//			.config_size = sizeof(s_trj_ctrl_upos_init),
//			.config = &trj_ctrl_upos_config_,
//
//			.data_size = sizeof(s_trj_ctrl_upos),
//			.data   = NULL,
//
//			.init   = trj_ctrl_upos_init_,
//			.free   = trj_ctrl_upos_free_,
//			.save   = trj_ctrl_upos_save_,
//			.load   = trj_ctrl_upos_load_,
//			.reset  = trj_ctrl_upos_reset_,
//			.update = trj_ctrl_upos_update_,
//	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			
			.desc   = "default_ctrl_cpos",
			.name   = "default_ctrl_cpos",
			
			.config_size = sizeof(s_trj_ctrl_cpos_init),
			.config = &trj_ctrl_cpos_config_,
			
			.data_size = sizeof(s_trj_ctrl_cpos),
			.data   = NULL,
			
			.init   = trj_ctrl_cpos_init_,
			.free   = trj_ctrl_cpos_free_,
			.save   = trj_ctrl_cpos_save_,
			.load   = trj_ctrl_cpos_load_,
			.reset  = trj_ctrl_cpos_reset_,
			.update = trj_ctrl_cpos_update_,
	});
	
//	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
//
//			.desc   = "default_ctrl_urot",
//			.name   = "default_ctrl_urot",
//
//			.config_size = sizeof(s_trj_ctrl_urot_init),
//			.config = &trj_ctrl_urot_config_,
//
//			.data_size = sizeof(s_trj_ctrl_urot),
//			.data   = NULL,
//
//			.init   = trj_ctrl_urot_init_,
//			.free   = trj_ctrl_urot_free_,
//			.save   = trj_ctrl_urot_save_,
//			.load   = trj_ctrl_urot_load_,
//			.reset  = trj_ctrl_urot_reset_,
//			.update = trj_ctrl_urot_update_,
//	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			
			.desc   = "default_ctrl_crot",
			.name   = "default_ctrl_crot",
			
			.config_size = sizeof(s_trj_ctrl_crot_init),
			.config = &trj_ctrl_crot_config_,
			
			.data_size = sizeof(s_trj_ctrl_crot),
			.data   = NULL,
			
			.init   = trj_ctrl_crot_init_,
			.free   = trj_ctrl_crot_free_,
			.save   = trj_ctrl_crot_save_,
			.load   = trj_ctrl_crot_load_,
			.reset  = trj_ctrl_crot_reset_,
			.update = trj_ctrl_crot_update_,
	});
	
	static s_trj_ctrl_egms_init trj_ctrl_egms_config_ = {
			.eng = &self->eng,
			.ref = self->eng.obj_list,
	};
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			
			.desc   = "default_ctrl_egms",
			.name   = "default_ctrl_egms",
			
			.config_size = sizeof(s_trj_ctrl_egms_init),
			.config = &trj_ctrl_egms_config_,
			
			.data_size = sizeof(s_trj_ctrl_egms),
			.data   = NULL,
			
			.init   = trj_ctrl_egms_init_,
			.free   = trj_ctrl_egms_free_,
			.save   = trj_ctrl_egms_save_,
			.load   = trj_ctrl_egms_load_,
			.reset  = trj_ctrl_egms_reset_,
			.update = trj_ctrl_egms_update_,
	});
	
	static s_trj_ctrl_gms_init trj_ctrl_gms_config_ = {
			.eng = &self->eng,
			.ref = self->eng.obj_list,
	};
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			
			.desc   = "default_ctrl_gms",
			.name   = "default_ctrl_gms",
			
			.config_size = sizeof(s_trj_ctrl_gms_init),
			.config = &trj_ctrl_gms_config_,
			
			.data_size = sizeof(s_trj_ctrl_gms),
			.data   = NULL,
			
			.init   = trj_ctrl_gms_init_,
			.free   = trj_ctrl_gms_free_,
			.save   = trj_ctrl_gms_save_,
			.load   = trj_ctrl_gms_load_,
			.reset  = trj_ctrl_gms_reset_,
			.update = trj_ctrl_gms_update_,
	});

	static s_trj_ctrl_varot_attr trj_ctrl_varot_config_ = {
	        .eng = &self->eng,
	        .ref = self->eng.obj_list,
	        };

	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {

	    .desc   = "default_ctrl_varot",
	    .name   = "default_ctrl_varot",

	    .config_size = sizeof(s_trj_ctrl_varot_attr),
	    .config = &trj_ctrl_varot_config_,

	    .data_size = sizeof(s_trj_ctrl_varot),
	    .data   = NULL,

	    .init   = trj_ctrl_varot_init_,
	    .free   = trj_ctrl_varot_free_,
	    .save   = trj_ctrl_varot_save_,
	    .load   = trj_ctrl_varot_load_,
	    .reset  = trj_ctrl_varot_reset_,
	    .update = trj_ctrl_varot_update_,
	    });
	
//	static s_trj_ctrl_gm_init trj_ctrl_gm_config_ = {
//			.order = 12,
//			.file_name = "res/ctrl/gm/egm2008.txt",
//	};
//
//	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
//
//			.desc   = "default_ctrl_gm",
//			.name   = "default_ctrl_gm",
//
//			.config_size = sizeof(s_trj_ctrl_gm_init),
//			.config = &trj_ctrl_gm_config_,
//
//			.data_size = sizeof(s_trj_ctrl_gm),
//			.data   = NULL,
//
//			.init   = trj_ctrl_gm_init_,
//			.free   = trj_ctrl_gm_free_,
//			.save   = trj_ctrl_gm_save_,
//			.load   = trj_ctrl_gm_load_,
//			.reset  = trj_ctrl_gm_reset_,
//			.update = trj_ctrl_gm_update_,
//	});
	
	static s_trj_data_text_init trj_data_text_config_ = {
	
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			
			.desc   = "default_data_text",
			.name   = "default_data_text",
			
			.config_size = sizeof(s_trj_data_text_init),
			.config = &trj_data_text_config_,
			
			.data_size = sizeof(s_trj_data_text),
			.data   = NULL,
			
			.init   = trj_data_text_init_,
			.free   = trj_data_text_free_,
			.save   = trj_data_text_save_,
			.load   = trj_data_text_load_,
			.reset  = trj_data_text_reset_,
			.render = trj_data_text_render_,
	});
	
	static s_trj_data_ram_init trj_data_ram_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			
			.desc   = "default_data_ram",
			.name   = "default_data_ram",
			
			.config_size = sizeof(s_trj_data_ram_init),
			.config = &trj_data_ram_config_,
			
			.data_size = sizeof(s_trj_data_ram),
			.data   = NULL,
			
			.init   = trj_data_ram_init_,
			.free   = trj_data_ram_free_,
			.save   = trj_data_ram_save_,
			.load   = trj_data_ram_load_,
			.reset  = trj_data_ram_reset_,
			.render = trj_data_ram_render_,
	});
	
	static s_trj_data_ramld_init trj_data_ramld_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			
			.desc   = "default_data_ramld",
			.name   = "default_data_ramld",
			
			.config_size = sizeof(s_trj_data_ramld_init),
			.config = &trj_data_ramld_config_,
			
			.data_size = sizeof(s_trj_data_ramld),
			.data   = NULL,
			
			.init   = trj_data_ramld_init_,
			.free   = trj_data_ramld_free_,
			.save   = trj_data_ramld_save_,
			.load   = trj_data_ramld_load_,
			.reset  = trj_data_ramld_reset_,
			.render = trj_data_ramld_render_,
	});
	
	static s_trj_data_mat_init trj_data_mat_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			
			.desc   = "default_data_mat",
			.name   = "default_data_mat",
			
			.config_size = sizeof(s_trj_data_mat_init),
			.config = &trj_data_mat_config_,
			
			.data_size = sizeof(s_trj_data_mat),
			.data   = NULL,
			
			.init   = trj_data_mat_init_,
			.free   = trj_data_mat_free_,
			.save   = trj_data_mat_save_,
			.load   = trj_data_mat_load_,
			.reset  = trj_data_mat_reset_,
			.render = trj_data_mat_render_,
	});
	
	static s_trj_data_bin_init trj_data_bin_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			
			.desc   = "default_data_bin",
			.name   = "default_data_bin",
			
			.config_size = sizeof(s_trj_data_bin_init),
			.config = &trj_data_bin_config_,
			
			.data_size = sizeof(s_trj_data_bin),
			.data   = NULL,
			
			.init   = trj_data_bin_init_,
			.free   = trj_data_bin_free_,
			.save   = trj_data_bin_save_,
			.load   = trj_data_bin_load_,
			.reset  = trj_data_bin_reset_,
			.render = trj_data_bin_render_,
	});
	
	static s_trj_proc_euler_attr trj_proc_euler_config_ = {
            .rot_tol  = 1E-6,
            .rot_var  = 1E-6,
            .rot_step = 1E-6,

            .pos_tol  = 1E-3,
            .pos_var  = 1E-3,
            .pos_step = 1E-3,
	};
	
	trj_eng_add_procapi(&self->eng, (s_trj_proc) {

            .desc   = "default_proc_euler",
//            .name   = "default_proc_euler",
			
			.config_size = sizeof(s_trj_proc_euler_attr),
			.config = &trj_proc_euler_config_,
			
			.data_size = sizeof(s_trj_proc_euler),
			.data   = NULL,
			
			.init   = trj_proc_euler_init_,
			.free   = trj_proc_euler_free_,
			.save   = trj_proc_euler_save_,
			.load   = trj_proc_euler_load_,
			.update = trj_proc_euler_update_,
	});
	
//	static s_trj_proc_fps_init trj_proc_fps_config_ = {
//			.temp = 0x00,
//	};
//
//	trj_eng_add_procapi(&self->eng, (s_trj_proc) {
//
//			.desc   = "default_proc_fps",
//
//			.config_size = sizeof(s_trj_proc_fps_init),
//			.config = &trj_proc_fps_config_,
//
//			.data_size = sizeof(s_trj_proc_fps),
//			.data   = NULL,
//
//			.init   = trj_proc_fps_init_,
//			.free   = trj_proc_fps_free_,
//			.save   = trj_proc_fps_save_,
//			.load   = trj_proc_fps_load_,
//			.update = trj_proc_fps_update_,
//	});
	
	for (int i = 0; i < sizeof(self->st_gui_eng_obj) / sizeof(s_gui_obj); ++i)
	{
		gui_obj_init(&self->st_gui_eng_obj[i],
					 (s_gui_obj_init) {.ref = &self->eng.obj_list[i]}
		);
	}
	
	//-----------------------------------------------------------------------------
	
	ImGuiIO& io = ImGui::GetIO();
	
	ImFontConfig font_config;
	font_config.OversampleH = 4;
	font_config.OversampleV = 4;
	
	io.FontDefault = io.Fonts->AddFontFromMemoryCompressedBase85TTF
			(default_font_compressed_data_base85,
					18, &font_config, io.Fonts->GetGlyphRangesCyrillic());
	
    imgui_theme_set();
	
	ImGuiStyle& style_ref = ImGui::GetStyle();
	
	style_ref.ScrollbarRounding = 0.0;
	style_ref.ChildRounding     = 0.0;
	style_ref.WindowRounding    = 0.0;
	style_ref.FrameRounding     = 0.0;
	style_ref.GrabRounding      = 0.0;
	style_ref.PopupRounding     = 0.0;
	style_ref.TabRounding       = 0.0;
	
	style_ref.ChildBorderSize = 0.0;
	style_ref.FrameBorderSize = 0.0;
	style_ref.PopupBorderSize = 0.0;
	style_ref.TabBorderSize = 0.0;
	style_ref.WindowBorderSize = 0.0;
	
	gui_env_init(&self->gui_env, (s_gui_env_init) {
			.eng = &self->eng,
			.traj_offset = &self->eng.traj_offset,
			.ctrl_offset = &self->eng.ctrl_offset,
			.data_offset = &self->eng.data_offset,
			
			.traj_list = self->eng.traj_list,
			.ctrl_list = self->eng.ctrl_list,
			.data_list = self->eng.data_list,
	});
	
	gui_cmd_init(&self->gui_cmd, (s_gui_cmd_init)
			{
					.env = &self->gui_env,
					.visible = false,
					.title = "env",
			});

	// !!! INIT DEFAULT PROC before loading
	// because load will try to free it
	self->eng.proc = self->eng.proc_list[0];
    self->eng.proc.init(&self->eng.proc.data, self->eng.proc.config);

	trj_eng_load(&self->eng, self->gui_tbar.file_path);

//	gui_eng_sel_data(&self->gui_eng, &self->eng.obj_list[0].data_list[0]);
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t gui_main(s_gui *self)
{
	int static_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus
					   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
    
    
    #ifdef NDEBUG
	#else
	
	static bool show_demo = true;
	static bool show_metrics = true;
	
	ImGui::ShowMetricsWindow(&show_metrics);
	ImGui::ShowDemoWindow(&show_demo);
	
	#endif
    
	const uint32_t traj_hash_static = crc32_iso_str("default_traj_static");
	const uint32_t traj_hash_orb    = crc32_iso_str("default_traj_orb");
	const uint32_t traj_hash_bz     = crc32_iso_str("default_traj_bz");
	const uint32_t traj_hash_bz2    = crc32_iso_str("default_traj_bz2");
	const uint32_t traj_hash_navsat = crc32_iso_str("default_traj_navsat");
	
	const uint32_t data_hash_text 	= crc32_iso_str("default_data_text");
	const uint32_t data_hash_ram  	= crc32_iso_str("default_data_ram");
	const uint32_t data_hash_ramld	= crc32_iso_str("default_data_ramld");
	const uint32_t data_hash_mat  	= crc32_iso_str("default_data_mat");
	const uint32_t data_hash_bin  	= crc32_iso_str("default_data_bin");
	
	const uint32_t ctrl_hash_gm   	= crc32_iso_str("default_ctrl_gm");
	const uint32_t ctrl_hash_egms 	= crc32_iso_str("default_ctrl_egms");
	const uint32_t ctrl_hash_gms  	= crc32_iso_str("default_ctrl_gms");
	const uint32_t ctrl_hash_varot	= crc32_iso_str("default_ctrl_varot");

    {
        // Toolbar
        ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(self->w_width, (float) self->gui_tbar.height), ImGuiCond_Always);
//		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(255,255,255,255));
        ImGui::Begin("toolbar", NULL, static_flags
                                      | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
        gui_tbar_main(&self->gui_tbar);
        ImGui::End();
//		ImGui::PopStyleColor();
    }

	{
		// progress popup
		// must be after toolbar
		gui_eng_updategui(&self->gui_eng, &self->eng);
	}

    {
        // Settings
        gui_conf_view(&self->gui_conf);
    }

    {
		// Object list
		ImGui::SetNextWindowPos(ImVec2(0, (float) self->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, self->w_height - self->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("obj_list", NULL, static_flags);
		gui_eng_objlist(&self->gui_eng, &self->eng);
		ImGui::End();
	}
	
	{
		// Object edit
		ImGui::SetNextWindowPos(ImVec2(240, (float) self->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240, self->w_height - self->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("item_edit", NULL, static_flags);
		
		if (self->gui_eng.sel_item != NULL)
		{
			switch (self->gui_eng.sel_type)
			{
				case gui_eng_type_obj:
				{
					s_gui_obj *obj_gui = (s_gui_obj*) self->gui_eng.sel_item;
					s_trj_obj *obj = obj_gui->ref;
					
					gui_obj_edit(obj_gui, obj);
					
					break;
				}
				
				case gui_eng_type_traj:
				{
					s_trj_traj *traj = (s_trj_traj*) self->gui_eng.sel_item;
					gui_traj_edit(traj);
					
					if      (traj->hash == traj_hash_static) { gui_traj_edit_static (traj); }
					else if (traj->hash == traj_hash_orb   ) { gui_traj_edit_orb    (traj); }
					else if (traj->hash == traj_hash_bz    ) { gui_traj_edit_bz     (traj); }
					else if (traj->hash == traj_hash_bz2   ) { gui_traj_edit_bz2    (traj); }
					else if (traj->hash == traj_hash_navsat) { gui_traj_edit_navsat (traj); }
					
					break;
				}
				
				case gui_eng_type_ctrl:
				{
					s_trj_ctrl *ctrl = (s_trj_ctrl*) self->gui_eng.sel_item;
					gui_ctrl_edit(ctrl);
					
					if      (ctrl->hash == ctrl_hash_gm   ) { gui_ctrl_edit_gm   (ctrl); }
					else if (ctrl->hash == ctrl_hash_egms ) { gui_ctrl_edit_egms (ctrl); }
					else if (ctrl->hash == ctrl_hash_gms  ) { gui_ctrl_edit_gms  (ctrl); }
					else if (ctrl->hash == ctrl_hash_varot) { gui_ctrl_edit_varot(ctrl); }
//                    else if (traj->hash == traj_hash_bz    ) { gui_traj_edit_bz     (traj); }
					
					break;
				}
				
				case gui_eng_type_data:
				{
					s_trj_data *data = (s_trj_data*) self->gui_eng.sel_item;
					gui_data_edit(data);
					
					if      (data->hash == data_hash_text ) { gui_data_edit_text (data); }
					else if (data->hash == data_hash_ram  ) { gui_data_edit_ram  (data); }
					else if (data->hash == data_hash_ramld) { gui_data_edit_ramld(data); }
					else if (data->hash == data_hash_mat  ) { gui_data_edit_mat  (data); }
					else if (data->hash == data_hash_bin  ) { gui_data_edit_bin  (data); }
					
					break;
				}
				
				default:
					break;
			}
		}
		
		ImGui::End();
	}
	
	{
		// Main view
		ImGui::SetNextWindowPos (ImVec2(240*2, (float) self->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(self->w_width - 240*2, self->w_height - self->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("main_view", NULL, static_flags);
		
		if (self->gui_eng.sel_item != NULL)
		{
			switch (self->gui_eng.sel_type)
			{
				case gui_eng_type_obj:
				{
					s_gui_obj *obj_gui = (s_gui_obj*) self->gui_eng.sel_item;
					s_trj_obj *obj = obj_gui->ref;

//					gui_obj_view(obj_gui, obj);
					
					break;
				}
				
				case gui_eng_type_traj:
				{
					s_trj_traj *traj = (s_trj_traj*) self->gui_eng.sel_item;
					
					if      (traj->hash == traj_hash_static) { gui_traj_view_static (traj); }
					else if (traj->hash == traj_hash_orb   ) { gui_traj_view_orb    (traj); }
					else if (traj->hash == traj_hash_bz    ) { gui_traj_view_bz((s_trj_traj_bz *) traj->data, "##test", ImVec2(-1, -1), 0x00); }
					else if (traj->hash == traj_hash_bz2   ) { gui_traj_view_bz2    (traj); }
					else if (traj->hash == traj_hash_navsat) { gui_traj_view_navsat (traj); }
					
					break;
				}
				
				case gui_eng_type_ctrl:
				{
					s_trj_ctrl *ctrl = (s_trj_ctrl*) self->gui_eng.sel_item;
					
					if      (ctrl->hash == ctrl_hash_gm  ) { gui_ctrl_view_gm  (ctrl); }
					else if (ctrl->hash == ctrl_hash_egms) { gui_ctrl_view_egms(ctrl); }
					else if (ctrl->hash == ctrl_hash_gms ) { gui_ctrl_view_gms (ctrl); }
					
					break;
				}
				
				case gui_eng_type_data:
				{
					s_trj_data *data = (s_trj_data*) self->gui_eng.sel_item;
					
					if      (data->hash == data_hash_text ) { gui_data_view_text  (data); }
					else if (data->hash == data_hash_ram  ) { gui_data_view_ram   (data); }
					else if (data->hash == data_hash_ramld) { gui_data_view_ramld (data); }
					else if (data->hash == data_hash_mat  ) { gui_data_view_mat   (data); }
					else if (data->hash == data_hash_bin  ) { gui_data_view_bin   (data); }
					
					break;
				}
				
				default: break;
			}
		}
		
		else
		{
			// default view (can be used for debug)
//			gui_map_view(&map, "map", ImVec2(-1, -1));
			
			static s_vl3d_obj  	obj_list[20000];
			static s_vl3d  		vl3d;
			
			s_vl3d_view view = vl3d_view();
			
			view.scale = 1.0;
			
			vl3d_view_load(self, &view, view);
			
			vl3d_init(&vl3d, (s_vl3d_attr) {
				.obj_sz = sizeof(obj_list) / sizeof(s_vl3d_obj),
				.obj_ls = obj_list,
			});
			
			float64_t pt_disp = 2.0;
			float64_t pt_size = 2.0;

			ImGuiIO io = ImGui::GetIO();
			
			int cnt = 6;
			static vlf_t a_time = 0.0;
			a_time += 0.5 * io.DeltaTime;
			vlf_t a_state = 0.5 * (1.0 + vl_sin(a_time));
			
			static vlf_t d_ = 1.5 * vl_pi;
			d_ += io.DeltaTime;
			if (d_ > 2.5*vl_pi) { d_ = 2.5*vl_pi; }
			
			vlf_t d = 0.5 * (1.0 + vl_sin(d_)) * (1.0 / view.scale / 4.0);
			
			vlf_t pt_norm = vl_gauss1(0.0, 0.0, pt_disp);
			vlf_t gain;
			ImVec4 color_v4;
			
			s_vl3d_point point;
			s_vl3d_line  line;
			
			s_vl_hpr view_hpr = vl_hpr(&view.rot[0][0]);
			view_hpr.heading += io.DeltaTime * 0.01;
			view_hpr.pitch   += io.DeltaTime * 0.02;
			view_hpr.roll    += io.DeltaTime * 0.03;
			vl3_rot(&view.rot[0][0], view_hpr);
			
			for (int x = 0; x < cnt; ++x)
			{
				for (int y = 0; y < cnt; ++y)
				{
					for (int z = 0; z < cnt; ++z)
					{
						gain = vl_sqrt(x * x + y * y + z * z);
						gain = 0.4 * vl_gauss1(gain * a_state, 0.0, pt_disp) / pt_norm;
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						point.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						point.size = pt_size;
						
						gain = vl_sqrt(x * x + y * y + z * z);
						gain = 0.2 * vl_gauss1(gain * a_state, 0.0, pt_disp) / pt_norm;
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						line.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						
						vl3_vcopy(point.p0, vl_vec(x * d, y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(x * d, y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(x * d, -y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(x * d, -y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(-x * d, y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(-x * d, y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(-x * d, -y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vl3_vcopy(point.p0, vl_vec(-x * d, -y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						// 1
						vl3_vcopy(line.p0, vl_vec(d*x, d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(d*x + d, d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, d*y + d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 2
						vl3_vcopy(line.p0, vl_vec(d*x, d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(d*x + d, d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, d*y + d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
						
						// 3
						vl3_vcopy(line.p0, vl_vec(d*x, -d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(d*x + d, -d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, -d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, -d*y - d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, -d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, -d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 4
						vl3_vcopy(line.p0, vl_vec(d*x, -d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(d*x + d, -d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, -d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, -d*y - d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(d*x, -d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(d*x, -d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
						
						// 5
						vl3_vcopy(line.p0, vl_vec(-d*x, d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x - d, d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, d*y + d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 6
						vl3_vcopy(line.p0, vl_vec(-d*x, d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x - d, d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, d*y + d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
						
						// 7
						vl3_vcopy(line.p0, vl_vec(-d*x, -d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x - d, -d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, -d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, -d*y - d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, -d*y, d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, -d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 8
						vl3_vcopy(line.p0, vl_vec(-d*x, -d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x - d, -d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, -d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, -d*y - d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vl3_vcopy(line.p0, vl_vec(-d*x, -d*y, -d*z));
						vl3_vcopy(line.p1, vl_vec(-d*x, -d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
					}
				}
			}

//			static vl3d_text text = (s_vl3d_text) { .p0 = { 0, 0, 0 }, .data = "hello", .color = vl3d_col_legacy };
//			vl3d_add_text(&vl3d, text);

//			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(0x00));
			
			vl3d_begin(&vl3d);
			vl3d_view_ctrl3d(&vl3d, &view);
			vl3d_view_draw(&vl3d, &view);
			vl3d_end();
			
			vl3d_view_save(self, &view);

//			ImGui::PopStyleColor();
		}
		
		ImGui::End();
	}
	
	{
		// Scripting view
		gui_cmd_render(&self->gui_cmd);
	}
	
	return 0x00;
}

//----------------------------------------------------------------

#endif
