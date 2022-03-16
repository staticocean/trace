
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI__
#define __GUI__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <imgui/imgui.h>
#include <nfd.h>

#include <sftlstd/vld3.h>
#include <sftlstd/log.h>
#include <sftlgui/imgui_theme.h>
#include <sftlgui/imgui_custom.h>
#include <sftltrc/sftltrc.h>

#include "guitraj.h"
#include "impl/guitraj_static.h"
#include "impl/guitraj_orb.h"
#include "impl/guitraj_bz.h"
#include "impl/guitraj_bz2.h"

#include "guictrl.h"

#include "guidata.h"

#include "gui_eng.h"
#include "gui_obj.h"
#include "gui_tbar.h"
#include "gui_clip.h"

//------------------------------------------------------------------------------

typedef struct gui
{
	t_f32 			w_height;
	t_f32 			w_width;

    t_f32           gscale;
	
	s_trceng 		eng;
	
	s_trcproc 		*proc_ls		[256];
	s_trcrefs 		*refs_ls		[256];
	s_trcobj  		*obj_ls 		[256];
	s_trctraj 		*traj_ls		[256];
	s_trcctrl 		*ctrl_ls		[256];
	s_trcdata 		*data_ls		[256];
	
	s_trcproc_intf 	*proc_intf_ls	[256];
	s_trcrefs_intf 	*refs_intf_ls	[256];
	s_trcobj_intf  	*obj_intf_ls 	[256];
	s_trctraj_intf 	*traj_intf_ls	[256];
	s_trcctrl_intf 	*ctrl_intf_ls	[256];
	s_trcdata_intf 	*data_intf_ls	[256];
	
	s_gui_obj  		gui_obj_ls 		[256];
	
	s_gui_tbar 		gui_tbar;
	s_gui_eng  		gui_eng;
	s_gui_clip 		gui_clip;
	s_gui_conf 		gui_conf;
	
} 	s_gui;

typedef struct gui_attr
{
    t_f32           gscale;

}	s_gui_attr;

//------------------------------------------------------------------------------

t_u8 gui_init (s_gui *self, s_gui_attr *attr)
{
    gui_clip_init(&self->gui_clip, (s_gui_clip_attr) {});

	l_init();
    l_test();

    // Init Native File Dialog Extended library
    NFD_Init();

    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig font_config;
    font_config.OversampleH = 4;
    font_config.OversampleV = 4;

    io.FontDefault = io.Fonts->AddFontFromMemoryCompressedBase85TTF
            (default_font_compressed_data_base85,
             18, &font_config, io.Fonts->GetGlyphRangesCyrillic());

    imgui_theme_set();

    ImGui::StyleColorsLight();

    ImGuiStyle& style_ref = ImGui::GetStyle();

    style_ref.ScrollbarRounding = 0.0;
    style_ref.ChildRounding     = 0.0;
    style_ref.WindowRounding    = 0.0;
    style_ref.FrameRounding     = 0.0;
    style_ref.GrabRounding      = 0.0;
    style_ref.PopupRounding     = 0.0;
    style_ref.TabRounding       = 0.0;

    style_ref.ChildBorderSize   = 0.0;
    style_ref.FrameBorderSize   = 0.0;
    style_ref.PopupBorderSize   = 0.0;
    style_ref.TabBorderSize     = 0.0;
    style_ref.WindowBorderSize  = 0.0;

    self->gscale = attr->gscale;
    io.FontGlobalScale = self->gscale;

//    pem_init(&pem, (s_pem_attr) { .t_sz = PEM_T_SZ });

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
    self->gui_tbar.conf = &self->gui_conf;
	self->gui_tbar.height = 40*self->gscale;

    sprintf(self->gui_tbar.file_path, "res/saves/default.trj");
	
	gui_eng_init(&self->gui_eng, (s_gui_eng_init) {
	    .obj_ls = self->gui_obj_ls,
	    .gui_clip = &self->gui_clip,
    });
	
	s_trceng_attr trceng_attr = {
		
		.proc     		= self->proc_ls[0],

		.proc_ls  		= self->proc_ls,
		.refs_ls  		= self->refs_ls,
		.obj_ls   		= self->obj_ls,
		.traj_ls  		= self->traj_ls,
		.ctrl_ls  		= self->ctrl_ls,
		.data_ls  		= self->data_ls,
		
		.proc_intf_ls 	= self->proc_intf_ls,
		.refs_intf_ls 	= self->refs_intf_ls,
		.obj_intf_ls  	= self->obj_intf_ls,
		.traj_intf_ls	= self->traj_intf_ls,
		.ctrl_intf_ls 	= self->ctrl_intf_ls,
		.data_intf_ls 	= self->data_intf_ls,
		
		.time_limit 	= 3600.0,
		.time_step 		= 0.01,
	};
	
	trceng_init(&self->eng, &trceng_attr);

    trceng_add_proc_intf(&self->eng, &__trcproc_euler__);
    trceng_add_proc_intf(&self->eng, &__trcproc_fps__);

    trceng_add_refs_intf(&self->eng, &__trcrefs_epz9002__);
    trceng_add_refs_intf(&self->eng, &__trcrefs_epz9011__);
    trceng_add_refs_intf(&self->eng, &__trcrefs_ewgs84__);

    trceng_add_traj_intf(&self->eng, &__guitraj_static__);
    trceng_add_traj_intf(&self->eng, &__guitraj_orb__);
//    trceng_add_traj_intf(&self->eng, &__guitraj_bz__);

//	static s_trctraj_static_init trctraj_static_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//
//			.ellp_en = 0x00,
//			.ellp = NULL,
//
//			.pos = { 0, 0, 0 },
//			.rot = { 1, 0, 0, 0, 1, 0, 0, 0, 1 },
//	};
//
//	trceng_add_trajapi(&self->eng, (s_trctraj) {
//
//			.desc = "default_traj_static",
//			.name = "default_traj_static",
//
//			.config_size = sizeof(s_trctraj_static_init),
//			.config = &trctraj_static_config_,
//
//			.data_size = sizeof(s_trctraj_static),
//			.data = NULL,
//
//			.init = trctraj_static_init_,
//			.free = trctraj_static_free_,
//			.save = trctraj_static_save_,
//			.load = trctraj_static_load_,
//			.compile = trctraj_static_compile_,
//			.info = trctraj_static_info_,
//			.pos  = trctraj_static_pos_,
//			.rot  = trctraj_static_rot_,
//	});
//
//	static s_trctraj_orb_init trctraj_orb_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//
//			.sync_en = 0x00,
//
//			.radius = 0.0,
//			.rate = 0.0,
//
//			.s_rate = 0.0,
//	};
//
//	vl3_mid(trctraj_orb_config_.tilt);
//	vl3_mid(trctraj_orb_config_.s_tilt);
//
//	trceng_add_trajapi(&self->eng, (s_trctraj) {
//
//			.desc = "default_traj_orb",
//			.name = "default_traj_orb",
//
//			.config_size = sizeof(s_trctraj_orb_init),
//			.config = &trctraj_orb_config_,
//
//			.data_size = sizeof(s_trctraj_orb),
//			.data = NULL,
//
//			.init = trctraj_orb_init_,
//			.free = trctraj_orb_free_,
//			.save = trctraj_orb_save_,
//			.load = trctraj_orb_load_,
//			.compile = trctraj_orb_compile_,
//			.info = trctraj_orb_info_,
//			.pos = trctraj_orb_pos_,
//			.rot = trctraj_orb_rot_,
//	});
//
//	static s_trctraj_bz_init trctraj_bz_config_ = {
//
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//
//			.ellp_en = 0x00,
//			.ellp = NULL,
//
//			.pts = NULL,
//	};
//
//	trceng_add_trajapi(&self->eng, (s_trctraj) {
//
//			.desc = "default_traj_bz",
//			.name = "default_traj_bz",
//
//			.config_size = sizeof(s_trctraj_bz_init),
//			.config = &trctraj_bz_config_,
//
//			.data_size = sizeof(s_trctraj_bz),
//			.data = NULL,
//
//			.init = trctraj_bz_init_,
//			.free = trctraj_bz_free_,
//			.save = trctraj_bz_save_,
//			.load = trctraj_bz_load_,
//			.compile = trctraj_bz_compile_,
//			.info = trctraj_bz_info_,
//			.pos = trctraj_bz_pos_,
//			.rot = trctraj_bz_rot_,
//	});
//
//	static s_trctraj_bz_init trctraj_bz2_config_ = {
//
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//
//			.ellp_en = 0x00,
//			.ellp = NULL,
//
//			.pts = NULL,
//	};
//
//	trceng_add_trajapi(&self->eng, (s_trctraj) {
//
//			.desc = "default_traj_bz2",
//			.name = "default_traj_bz2",
//
//			.config_size = sizeof(s_trctraj_bz_init),
//			.config = &trctraj_bz_config_,
//
//			.data_size = sizeof(s_trctraj_bz),
//			.data = NULL,
//
//			.init = trctraj_bz_init_,
//			.free = trctraj_bz_free_,
//			.save = trctraj_bz_save_,
//			.load = trctraj_bz_load_,
//			.compile = trctraj_bz_compile_,
//			.info = trctraj_bz_info_,
//			.pos = trctraj_bz_pos_,
//			.rot = trctraj_bz_rot_,
//	});
//
//	static s_trctraj_navsat_init trctraj_navsat_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//	};
//
//	trceng_add_trajapi(&self->eng, (s_trctraj) {
//
//			.desc = "default_traj_navsat",
//			.name = "default_traj_navsat",
//
//			.config_size = sizeof(s_trctraj_navsat_init),
//			.config = &trctraj_navsat_config_,
//
//			.data_size = sizeof(s_trctraj_navsat),
//			.data = NULL,
//
//			.init = trctraj_navsat_init_,
//			.free = trctraj_navsat_free_,
//			.save = trctraj_navsat_save_,
//			.load = trctraj_navsat_load_,
//			.compile = trctraj_navsat_compile_,
//			.info = trctraj_navsat_info_,
//			.pos = trctraj_navsat_pos_,
//			.rot = trctraj_navsat_rot_,
//	});
//
//	static s_trcctrl_upos_init trcctrl_upos_config_ = {
//
//	};
//
//	static s_trcctrl_cpos_init trcctrl_cpos_config_ = {
//
//	};
//
//	static s_trcctrl_urot_init trcctrl_urot_config_ = {
//
//	};
//
//	static s_trcctrl_crot_init trcctrl_crot_config_ = {
//
//	};
//
////	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
////
////			.desc   = "default_ctrl_upos",
////			.name   = "default_ctrl_upos",
////
////			.config_size = sizeof(s_trcctrl_upos_init),
////			.config = &trcctrl_upos_config_,
////
////			.data_size = sizeof(s_trcctrl_upos),
////			.data   = NULL,
////
////			.init   = trcctrl_upos_init_,
////			.free   = trcctrl_upos_free_,
////			.save   = trcctrl_upos_save_,
////			.load   = trcctrl_upos_load_,
////			.reset  = trcctrl_upos_reset_,
////			.update = trcctrl_upos_update_,
////	});
//
//	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
//
//			.desc   = "default_ctrl_cpos",
//			.name   = "default_ctrl_cpos",
//
//			.config_size = sizeof(s_trcctrl_cpos_init),
//			.config = &trcctrl_cpos_config_,
//
//			.data_size = sizeof(s_trcctrl_cpos),
//			.data   = NULL,
//
//			.init   = trcctrl_cpos_init_,
//			.free   = trcctrl_cpos_free_,
//			.save   = trcctrl_cpos_save_,
//			.load   = trcctrl_cpos_load_,
//			.reset  = trcctrl_cpos_reset_,
//			.update = trcctrl_cpos_update_,
//	});
//
////	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
////
////			.desc   = "default_ctrl_urot",
////			.name   = "default_ctrl_urot",
////
////			.config_size = sizeof(s_trcctrl_urot_init),
////			.config = &trcctrl_urot_config_,
////
////			.data_size = sizeof(s_trcctrl_urot),
////			.data   = NULL,
////
////			.init   = trcctrl_urot_init_,
////			.free   = trcctrl_urot_free_,
////			.save   = trcctrl_urot_save_,
////			.load   = trcctrl_urot_load_,
////			.reset  = trcctrl_urot_reset_,
////			.update = trcctrl_urot_update_,
////	});
//
//	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
//
//			.desc   = "default_ctrl_crot",
//			.name   = "default_ctrl_crot",
//
//			.config_size = sizeof(s_trcctrl_crot_init),
//			.config = &trcctrl_crot_config_,
//
//			.data_size = sizeof(s_trcctrl_crot),
//			.data   = NULL,
//
//			.init   = trcctrl_crot_init_,
//			.free   = trcctrl_crot_free_,
//			.save   = trcctrl_crot_save_,
//			.load   = trcctrl_crot_load_,
//			.reset  = trcctrl_crot_reset_,
//			.update = trcctrl_crot_update_,
//	});
//
//	static s_trcctrl_egms_init trcctrl_egms_config_ = {
//			.eng = &self->eng,
//			.ref = self->eng.obj_ls,
//	};
//
//	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
//
//			.desc   = "default_ctrl_egms",
//			.name   = "default_ctrl_egms",
//
//			.config_size = sizeof(s_trcctrl_egms_init),
//			.config = &trcctrl_egms_config_,
//
//			.data_size = sizeof(s_trcctrl_egms),
//			.data   = NULL,
//
//			.init   = trcctrl_egms_init_,
//			.free   = trcctrl_egms_free_,
//			.save   = trcctrl_egms_save_,
//			.load   = trcctrl_egms_load_,
//			.reset  = trcctrl_egms_reset_,
//			.update = trcctrl_egms_update_,
//	});
//
//	static s_trcctrl_egmsnpo_init trcctrl_egmsnpo_config_ = {
//			.eng = &self->eng,
//			.ref = self->eng.obj_ls,
//			};
//
//	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
//
//		.desc   = "default_ctrl_egmsnpo",
//		.name   = "default_ctrl_egmsnpo",
//
//		.config_size = sizeof(s_trcctrl_egmsnpo_init),
//		.config = &trcctrl_egmsnpo_config_,
//
//		.data_size = sizeof(s_trcctrl_egmsnpo),
//		.data   = NULL,
//
//		.init   = trcctrl_egmsnpo_init_,
//		.free   = trcctrl_egmsnpo_free_,
//		.save   = trcctrl_egmsnpo_save_,
//		.load   = trcctrl_egmsnpo_load_,
//		.reset  = trcctrl_egmsnpo_reset_,
//		.update = trcctrl_egmsnpo_update_,
//		});
//
//
//	static s_trcctrl_gms_init trcctrl_gms_config_ = {
//			.eng = &self->eng,
//			.ref = self->eng.obj_ls,
//	};
//
//	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
//
//			.desc   = "default_ctrl_gms",
//			.name   = "default_ctrl_gms",
//
//			.config_size = sizeof(s_trcctrl_gms_init),
//			.config = &trcctrl_gms_config_,
//
//			.data_size = sizeof(s_trcctrl_gms),
//			.data   = NULL,
//
//			.init   = trcctrl_gms_init_,
//			.free   = trcctrl_gms_free_,
//			.save   = trcctrl_gms_save_,
//			.load   = trcctrl_gms_load_,
//			.reset  = trcctrl_gms_reset_,
//			.update = trcctrl_gms_update_,
//	});
//
//	static s_trcctrl_varot_attr trcctrl_varot_config_ = {
//	        .eng = &self->eng,
//	        .ref = self->eng.obj_ls,
//	        };
//
//	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
//
//	    .desc   = "default_ctrl_varot",
//	    .name   = "default_ctrl_varot",
//
//	    .config_size = sizeof(s_trcctrl_varot_attr),
//	    .config = &trcctrl_varot_config_,
//
//	    .data_size = sizeof(s_trcctrl_varot),
//	    .data   = NULL,
//
//	    .init   = trcctrl_varot_init_,
//	    .free   = trcctrl_varot_free_,
//	    .save   = trcctrl_varot_save_,
//	    .load   = trcctrl_varot_load_,
//	    .reset  = trcctrl_varot_reset_,
//	    .update = trcctrl_varot_update_,
//	    });
//
////	static s_trcctrl_gm_init trcctrl_gm_config_ = {
////			.order = 12,
////			.file_name = "res/ctrl/gm/egm2008.txt",
////	};
////
////	trceng_add_ctrlapi(&self->eng, (s_trcctrl) {
////
////			.desc   = "default_ctrl_gm",
////			.name   = "default_ctrl_gm",
////
////			.config_size = sizeof(s_trcctrl_gm_init),
////			.config = &trcctrl_gm_config_,
////
////			.data_size = sizeof(s_trcctrl_gm),
////			.data   = NULL,
////
////			.init   = trcctrl_gm_init_,
////			.free   = trcctrl_gm_free_,
////			.save   = trcctrl_gm_save_,
////			.load   = trcctrl_gm_load_,
////			.reset  = trcctrl_gm_reset_,
////			.update = trcctrl_gm_update_,
////	});
//
//	static s_trcdata_text_init trcdata_text_config_ = {
//
//	};
//
//	trceng_add_dataapi(&self->eng, (s_trcdata) {
//
//			.desc   = "default_data_text",
//			.name   = "default_data_text",
//
//			.config_size = sizeof(s_trcdata_text_init),
//			.config = &trcdata_text_config_,
//
//			.data_size = sizeof(s_trcdata_text),
//			.data   = NULL,
//
//			.init   = trcdata_text_init_,
//			.free   = trcdata_text_free_,
//			.save   = trcdata_text_save_,
//			.load   = trcdata_text_load_,
//			.reset  = trcdata_text_reset_,
//			.render = trcdata_text_render_,
//	});
//
//	static s_trcdata_ram_init trcdata_ram_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//	};
//
//	trceng_add_dataapi(&self->eng, (s_trcdata) {
//
//			.desc   = "default_data_ram",
//			.name   = "default_data_ram",
//
//			.config_size = sizeof(s_trcdata_ram_init),
//			.config = &trcdata_ram_config_,
//
//			.data_size = sizeof(s_trcdata_ram),
//			.data   = NULL,
//
//			.init   = trcdata_ram_init_,
//			.free   = trcdata_ram_free_,
//			.save   = trcdata_ram_save_,
//			.load   = trcdata_ram_load_,
//			.reset  = trcdata_ram_reset_,
//			.render = trcdata_ram_render_,
//	});
//
//	static s_trcdata_ramld_init trcdata_ramld_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//	};
//
//	trceng_add_dataapi(&self->eng, (s_trcdata) {
//
//			.desc   = "default_data_ramld",
//			.name   = "default_data_ramld",
//
//			.config_size = sizeof(s_trcdata_ramld_init),
//			.config = &trcdata_ramld_config_,
//
//			.data_size = sizeof(s_trcdata_ramld),
//			.data   = NULL,
//
//			.init   = trcdata_ramld_init_,
//			.free   = trcdata_ramld_free_,
//			.save   = trcdata_ramld_save_,
//			.load   = trcdata_ramld_load_,
//			.reset  = trcdata_ramld_reset_,
//			.render = trcdata_ramld_render_,
//	});
//
//	static s_trcdata_mat_init trcdata_mat_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//	};
//
//	trceng_add_dataapi(&self->eng, (s_trcdata) {
//
//			.desc   = "default_data_mat",
//			.name   = "default_data_mat",
//
//			.config_size = sizeof(s_trcdata_mat_init),
//			.config = &trcdata_mat_config_,
//
//			.data_size = sizeof(s_trcdata_mat),
//			.data   = NULL,
//
//			.init   = trcdata_mat_init_,
//			.free   = trcdata_mat_free_,
//			.save   = trcdata_mat_save_,
//			.load   = trcdata_mat_load_,
//			.reset  = trcdata_mat_reset_,
//			.render = trcdata_mat_render_,
//	});
//
//	static s_trcdata_bin_init trcdata_bin_config_ = {
//			.eng = &self->eng,
//			.ref = &self->eng.obj_ls[0],
//	};
//
//	trceng_add_dataapi(&self->eng, (s_trcdata) {
//
//			.desc   = "default_data_bin",
//			.name   = "default_data_bin",
//
//			.config_size = sizeof(s_trcdata_bin_init),
//			.config = &trcdata_bin_config_,
//
//			.data_size = sizeof(s_trcdata_bin),
//			.data   = NULL,
//
//			.init   = trcdata_bin_init_,
//			.free   = trcdata_bin_free_,
//			.save   = trcdata_bin_save_,
//			.load   = trcdata_bin_load_,
//			.reset  = trcdata_bin_reset_,
//			.render = trcdata_bin_render_,
//	});
//
//	static s_trcproc_euler_attr trcproc_euler_config_ = {
//            .rot_tol  = 1E-6,
//            .rot_var  = 1E-6,
//            .rot_step = 1E-6,
//
//            .pos_tol  = 1E-3,
//            .pos_var  = 1E-3,
//            .pos_step = 1E-3,
//	};
//
//	trceng_add_procapi(&self->eng, (s_trcproc) {
//
//            .desc   = "default_proc_euler",
////            .name   = "default_proc_euler",
//
//			.config_size = sizeof(s_trcproc_euler_attr),
//			.config = &trcproc_euler_config_,
//
//			.data_size = sizeof(s_trcproc_euler),
//			.data   = NULL,
//
//			.init   = trcproc_euler_init_,
//			.free   = trcproc_euler_free_,
//			.save   = trcproc_euler_save_,
//			.load   = trcproc_euler_load_,
//			.update = trcproc_euler_update_,
//	});
//
//	static s_trcproc_fps_init trcproc_fps_config_ = {
//			.temp = 0x00,
//	};
//
//	trceng_add_procapi(&self->eng, (s_trcproc) {
//
//			.desc   = "default_proc_fps",
//
//			.config_size = sizeof(s_trcproc_fps_init),
//			.config = &trcproc_fps_config_,
//
//			.data_size = sizeof(s_trcproc_fps),
//			.data   = NULL,
//
//			.init   = trcproc_fps_init_,
//			.free   = trcproc_fps_free_,
//			.save   = trcproc_fps_save_,
//			.load   = trcproc_fps_load_,
//			.update = trcproc_fps_update_,
//	});
	
//	for (int i = 0; i < sizeof(self->st_gui_eng_obj) / sizeof(s_gui_obj); ++i)
//	{
//		gui_obj_init(&self->st_gui_eng_obj[i],
//					 (s_gui_obj_init) {.ref = &self->eng.obj_ls[i]}
//		);
//	}

	// !!! INIT DEFAULT PROC before loading
	// because load will try to free it
	self->eng.proc = self->eng.proc_ls[0];
//    self->eng.proc.init(&self->eng.proc.data, self->eng.proc.config);

	trceng_load(&self->eng, self->gui_tbar.file_path);

//	gui_eng_sel_data(&self->gui_eng, &self->eng.obj_ls[0].data_ls[0]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline t_u8 gui_main(s_gui *self)
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
    
	const t_u32 traj_hash_static = crc32_iso_str("default_traj_static");
	const t_u32 traj_hash_orb    = crc32_iso_str("default_traj_orb");
	const t_u32 traj_hash_bz     = crc32_iso_str("default_traj_bz");
	const t_u32 traj_hash_bz2    = crc32_iso_str("default_traj_bz2");
	const t_u32 traj_hash_navsat = crc32_iso_str("default_traj_navsat");
	
	const t_u32 data_hash_text 	= crc32_iso_str("default_data_text");
	const t_u32 data_hash_ram  	= crc32_iso_str("default_data_ram");
	const t_u32 data_hash_ramld	= crc32_iso_str("default_data_ramld");
	const t_u32 data_hash_mat  	= crc32_iso_str("default_data_mat");
	const t_u32 data_hash_bin  	= crc32_iso_str("default_data_bin");
	
	const t_u32 ctrl_hash_gm   	= crc32_iso_str("default_ctrl_gm");
	const t_u32 ctrl_hash_egms 	= crc32_iso_str("default_ctrl_egms");
	const t_u32 ctrl_hash_egmsnpo= crc32_iso_str("default_ctrl_egmsnpo");
	const t_u32 ctrl_hash_gms  	= crc32_iso_str("default_ctrl_gms");
	const t_u32 ctrl_hash_varot	= crc32_iso_str("default_ctrl_varot");

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
		ImGui::SetNextWindowSize(ImVec2(240*self->gscale, self->w_height - self->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("obj_ls", NULL, static_flags);
		gui_eng_objlist(&self->gui_eng, &self->eng);
		ImGui::End();
	}
	
	{
		// Object edit
		ImGui::SetNextWindowPos(ImVec2(240*self->gscale, (float) self->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(240*self->gscale, self->w_height - self->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("item_edit", NULL, static_flags);
		
		if (self->gui_eng.sel_item != NULL)
		{
			switch (self->gui_eng.sel_type)
			{
				case gui_eng_type_obj:
				{
//					s_gui_obj *obj_gui = (s_gui_obj*) self->gui_eng.sel_item;
//					s_trcobj *obj = obj_gui->ref;
//
//					gui_obj_edit(obj_gui, obj);
					
					break;
				}
				
				case gui_eng_type_traj:
				{
					s_trctraj *traj = (s_trctraj*) self->gui_eng.sel_item;
					guitraj_edit(traj);

					break;
				}
				
				case gui_eng_type_ctrl:
				{
//					s_trcctrl *ctrl = (s_trcctrl*) self->gui_eng.sel_item;
//					gui_ctrl_edit(ctrl);

					break;
				}
				
				case gui_eng_type_data:
				{
//					s_trcdata *data = (s_trcdata*) self->gui_eng.sel_item;
//					gui_data_edit(data);

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
		ImGui::SetNextWindowPos (ImVec2(240*2*self->gscale, (float) self->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(self->w_width - 240*2*self->gscale, self->w_height - self->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("main_view", NULL, static_flags);
		
		if (self->gui_eng.sel_item != NULL)
		{
			switch (self->gui_eng.sel_type)
			{
				case gui_eng_type_obj:
				{
					s_gui_obj *obj_gui = (s_gui_obj*) self->gui_eng.sel_item;
					s_trcobj *obj = obj_gui->ref;

//					gui_obj_view(obj_gui, obj);
					
					break;
				}
				
				case gui_eng_type_traj:
				{
//					s_trctraj *traj = (s_trctraj*) self->gui_eng.sel_item;
//
//					if      (traj->hash == traj_hash_static) { gui_traj_view_static (traj); }
//					else if (traj->hash == traj_hash_orb   ) { gui_traj_view_orb    (traj); }
//					else if (traj->hash == traj_hash_bz    ) { gui_traj_view_bz((s_trctraj_bz *) traj->data, "##test", ImVec2(-1, -1), 0x00); }
//					else if (traj->hash == traj_hash_bz2   ) { gui_traj_view_bz2    (traj); }
//					else if (traj->hash == traj_hash_navsat) { gui_traj_view_navsat (traj); }
					
					break;
				}
				
				case gui_eng_type_ctrl:
				{
//					s_trcctrl *ctrl = (s_trcctrl*) self->gui_eng.sel_item;
//
//					if      (ctrl->hash == ctrl_hash_gm  ) { gui_ctrl_view_gm  (ctrl); }
//					else if (ctrl->hash == ctrl_hash_egms) { gui_ctrl_view_egms(ctrl); }
//					else if (ctrl->hash == ctrl_hash_gms ) { gui_ctrl_view_gms (ctrl); }
					
					break;
				}
				
				case gui_eng_type_data:
				{
//					s_trcdata *data = (s_trcdata*) self->gui_eng.sel_item;
//
//					if      (data->hash == data_hash_text ) { gui_data_view_text  (data); }
//					else if (data->hash == data_hash_ram  ) { gui_data_view_ram   (data); }
//					else if (data->hash == data_hash_ramld) { gui_data_view_ramld (data); }
//					else if (data->hash == data_hash_mat  ) { gui_data_view_mat   (data); }
//					else if (data->hash == data_hash_bin  ) { gui_data_view_bin   (data); }
					
					break;
				}
				
				default: break;
			}
		}
		
		else
		{
			// default view (can be used for debug)
//			gui_map_view(&map, "map", ImVec2(-1, -1));
			
			static s_vl3d_obj  	obj_ls[20000];
			static s_vl3d  		vl3d;
			
			s_vl3d_view view = vl3d_view();
			
			view.scale = 1.0;
			
			vl3d_view_load(self, &view, view);
			
			vl3d_init(&vl3d, (s_vl3d_attr) {
				.obj_sz = sizeof(obj_ls) / sizeof(s_vl3d_obj),
				.obj_ls = obj_ls,
			});
			
			t_f64 pt_disp = 2.0;
			t_f64 pt_size = 2.0;

			ImGuiIO io = ImGui::GetIO();
			
			int cnt = 6;
			static t_f64 a_time = 0.0;
			a_time += 0.5 * io.DeltaTime;
			t_f64 a_state = 0.5 * (1.0 + vld_sin(a_time));
			
			static t_f64 d_ = 1.5 * vld_pi;
			d_ += io.DeltaTime;
			if (d_ > 2.5*vld_pi) { d_ = 2.5*vld_pi; }
			
			t_f64 d = 0.5 * (1.0 + vld_sin(d_)) * (1.0 / view.scale / 4.0);
			
			t_f64 pt_norm = rand_gauss1(0.0, 0.0, pt_disp);
			t_f64 gain;
			ImVec4 color_v4;
			
			s_vl3d_point point;
			s_vl3d_line  line;
			
			s_vl3hpr view_hpr = vl3hpr_mat(&view.rot[0][0]);
			view_hpr.heading += io.DeltaTime * 0.01;
			view_hpr.pitch   += io.DeltaTime * 0.02;
			view_hpr.roll    += io.DeltaTime * 0.03;
			vld3m_rot(&view.rot[0][0], view_hpr);
			
			for (int x = 0; x < cnt; ++x)
			{
				for (int y = 0; y < cnt; ++y)
				{
					for (int z = 0; z < cnt; ++z)
					{
						gain = vld_sqrt(x * x + y * y + z * z);
						gain = 0.4 * rand_gauss1(gain * a_state, 0.0, pt_disp) / pt_norm;
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						point.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						point.size = pt_size;
						
						gain = vld_sqrt(x * x + y * y + z * z);
						gain = 0.2 * rand_gauss1(gain * a_state, 0.0, pt_disp) / pt_norm;
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						line.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						
						vld3v_copy(point.p0, vld3v(x * d, y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(x * d, y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(x * d, -y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(x * d, -y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(-x * d, y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(-x * d, y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(-x * d, -y * d, z * d));
						vl3d_add_point(&vl3d, point);
						
						vld3v_copy(point.p0, vld3v(-x * d, -y * d, -z * d));
						vl3d_add_point(&vl3d, point);
						
						// 1
						vld3v_copy(line.p0, vld3v(d*x, d*y, d*z));
						vld3v_copy(line.p1, vld3v(d*x + d, d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, d*y, d*z));
						vld3v_copy(line.p1, vld3v(d*x, d*y + d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, d*y, d*z));
						vld3v_copy(line.p1, vld3v(d*x, d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 2
						vld3v_copy(line.p0, vld3v(d*x, d*y, -d*z));
						vld3v_copy(line.p1, vld3v(d*x + d, d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, d*y, -d*z));
						vld3v_copy(line.p1, vld3v(d*x, d*y + d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, d*y, -d*z));
						vld3v_copy(line.p1, vld3v(d*x, d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
						
						// 3
						vld3v_copy(line.p0, vld3v(d*x, -d*y, d*z));
						vld3v_copy(line.p1, vld3v(d*x + d, -d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, -d*y, d*z));
						vld3v_copy(line.p1, vld3v(d*x, -d*y - d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, -d*y, d*z));
						vld3v_copy(line.p1, vld3v(d*x, -d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 4
						vld3v_copy(line.p0, vld3v(d*x, -d*y, -d*z));
						vld3v_copy(line.p1, vld3v(d*x + d, -d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, -d*y, -d*z));
						vld3v_copy(line.p1, vld3v(d*x, -d*y - d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(d*x, -d*y, -d*z));
						vld3v_copy(line.p1, vld3v(d*x, -d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
						
						// 5
						vld3v_copy(line.p0, vld3v(-d*x, d*y, d*z));
						vld3v_copy(line.p1, vld3v(-d*x - d, d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, d*y, d*z));
						vld3v_copy(line.p1, vld3v(-d*x, d*y + d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, d*y, d*z));
						vld3v_copy(line.p1, vld3v(-d*x, d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 6
						vld3v_copy(line.p0, vld3v(-d*x, d*y, -d*z));
						vld3v_copy(line.p1, vld3v(-d*x - d, d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, d*y, -d*z));
						vld3v_copy(line.p1, vld3v(-d*x, d*y + d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, d*y, -d*z));
						vld3v_copy(line.p1, vld3v(-d*x, d*y, -d*z - d));
						vl3d_add_line(&vl3d, line);
						
						// 7
						vld3v_copy(line.p0, vld3v(-d*x, -d*y, d*z));
						vld3v_copy(line.p1, vld3v(-d*x - d, -d*y, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, -d*y, d*z));
						vld3v_copy(line.p1, vld3v(-d*x, -d*y - d, d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, -d*y, d*z));
						vld3v_copy(line.p1, vld3v(-d*x, -d*y, d*z + d));
						vl3d_add_line(&vl3d, line);
						
						// 8
						vld3v_copy(line.p0, vld3v(-d*x, -d*y, -d*z));
						vld3v_copy(line.p1, vld3v(-d*x - d, -d*y, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, -d*y, -d*z));
						vld3v_copy(line.p1, vld3v(-d*x, -d*y - d, -d*z));
						vl3d_add_line(&vl3d, line);
						
						vld3v_copy(line.p0, vld3v(-d*x, -d*y, -d*z));
						vld3v_copy(line.p1, vld3v(-d*x, -d*y, -d*z - d));
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
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif
