
#include "trj_gui.h"

//------------------------------------------------------------------------------

static 	s_trj_gui_map map;

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init attr)
{
	s_trj_gui_map *temp_map = new s_trj_gui_map();
	map = *temp_map;
	free(temp_map);
	
	trj_gui_map_load(&map, "res/maps/earth/countries.geojson");
//	trj_gui_map_load(&map, "res/maps/earth/cities.geojson");
	
	self->w_height = 720;
	self->w_width  = 1024;
	
	self->gui_tbar.eng = &self->eng;
	self->gui_tbar.eng_gui = &self->gui_eng;
	self->gui_tbar.height = 38;
	
	trj_gui_eng_init(&self->gui_eng, (s_trj_gui_eng_init) { .obj_list = self->st_gui_eng_obj });
	
	trj_eng_init(&self->eng, (s_trj_eng_init) {
			.time_limit = 3600.0,
			.time_step = 0.01,
			.proc      = self->st_eng_proc_list,
			
			.obj_list  = self->st_eng_obj_list,
			
			.ellp_list = self->st_eng_ellp_list,
			.traj_list = self->st_eng_traj_list,
			.ctrl_list = self->st_eng_ctrl_list,
			.data_list = self->st_eng_data_list,
			.proc_list = self->st_eng_proc_list,
	});
	
	trj_ellp_init(&trj_ellp_wgs84);
	
	trj_eng_add_ellpapi(&self->eng, trj_ellp_wgs84);
	
	static s_trj_traj_static_init trj_traj_static_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.ellp_en = 0x00,
			.ellp = NULL,
			
			.pos = { 0, 0, 0 },
			.rot = { 1, 0, 0, 0, 1, 0, 0, 0, 1 },
	};
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			.name = "default_traj_static",
			.desc = "default_traj_static",
			.init = trj_traj_static_init_,
			.free = trj_traj_static_free_,
			.save = trj_traj_static_save_,
			.load = trj_traj_static_load_,
			.data_size = sizeof(s_trj_traj_static),
			.data = NULL,
			.config_size = sizeof(s_trj_traj_static_init),
			.config = &trj_traj_static_config_,
			.compile = trj_traj_static_compile_,
			.rot  = trj_traj_static_rot_,
			.pos  = trj_traj_static_pos_,
			.info = trj_traj_static_info_,
	});
	
	static s_trj_traj_orb_init trj_traj_orb_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.sync_en = 0x00,
			
			.radius = 0.0,
			.rate = 0.0,
			
			.s_rate = 0.0,
	};
	
	vl_mid(trj_traj_orb_config_.tilt);
	vl_mid(trj_traj_orb_config_.s_tilt);
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			.name = "default_traj_orb",
			.desc = "default_traj_orb",
			.init = trj_traj_orb_init_,
			.free = trj_traj_orb_free_,
			.save = trj_traj_orb_save_,
			.load = trj_traj_orb_load_,
			.data_size = sizeof(s_trj_traj_orb),
			.data = NULL,
			.config_size = sizeof(s_trj_traj_orb_init),
			.config = &trj_traj_orb_config_,
			.compile = trj_traj_orb_compile_,
			.rot = trj_traj_orb_rot_,
			.pos = trj_traj_orb_pos_,
			.info = trj_traj_orb_info_,
	});
	
	static s_trj_traj_bz_init trj_traj_bz_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.ellp_en = 0x00,
			.ellp = NULL,
			
			.pts = NULL,
	};
	
	trj_eng_add_trajapi(&self->eng, (s_trj_traj) {
			.name = "default_traj_bz",
			.desc = "default_traj_bz",
			.init = trj_traj_bz_init_,
			.free = trj_traj_bz_free_,
			.save = trj_traj_bz_save_,
			.load = trj_traj_bz_load_,
			.data_size = sizeof(s_trj_traj_bz),
			.data = NULL,
			.config_size = sizeof(s_trj_traj_bz_init),
			.config = &trj_traj_bz_config_,
			.compile = trj_traj_bz_compile_,
			.rot = trj_traj_bz_rot_,
			.pos = trj_traj_bz_pos_,
			.info = trj_traj_bz_info_,
	});
	
	static s_trj_ctrl_upos_init trj_ctrl_upos_config_ = {
	
	};
	
	static s_trj_ctrl_cpos_init trj_ctrl_cpos_config_ = {
	
	};
	
	static s_trj_ctrl_urot_init trj_ctrl_urot_config_ = {
	
	};
	
	static s_trj_ctrl_crot_init trj_ctrl_crot_config_ = {
	
	};
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.name   = "default_ctrl_upos",
			.desc   = "default_ctrl_upos",
			.init   = trj_ctrl_upos_init_,
			.free   = trj_ctrl_upos_free_,
			.save   = trj_ctrl_upos_save_,
			.load   = trj_ctrl_upos_load_,
			.data_size = sizeof(s_trj_ctrl_upos),
			.data   = NULL,
			.config_size = sizeof(s_trj_ctrl_upos_init),
			.config = &trj_ctrl_upos_config_,
			.reset  = trj_ctrl_upos_reset_,
			.update = trj_ctrl_upos_update_,
	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.name   = "default_ctrl_cpos",
			.desc   = "default_ctrl_cpos",
			.init   = trj_ctrl_cpos_init_,
			.free   = trj_ctrl_cpos_free_,
			.save   = trj_ctrl_cpos_save_,
			.load   = trj_ctrl_cpos_load_,
			.data_size = sizeof(s_trj_ctrl_cpos),
			.data   = NULL,
			.config_size = sizeof(s_trj_ctrl_cpos_init),
			.config = &trj_ctrl_cpos_config_,
			.reset  = trj_ctrl_cpos_reset_,
			.update = trj_ctrl_cpos_update_,
	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.name   = "default_ctrl_urot",
			.desc   = "default_ctrl_urot",
			.init   = trj_ctrl_urot_init_,
			.free   = trj_ctrl_urot_free_,
			.save   = trj_ctrl_urot_save_,
			.load   = trj_ctrl_urot_load_,
			.data_size = sizeof(s_trj_ctrl_urot),
			.data   = NULL,
			.config_size = sizeof(s_trj_ctrl_urot_init),
			.config = &trj_ctrl_urot_config_,
			.reset  = trj_ctrl_urot_reset_,
			.update = trj_ctrl_urot_update_,
	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.name   = "default_ctrl_crot",
			.desc   = "default_ctrl_crot",
			.init   = trj_ctrl_crot_init_,
			.free   = trj_ctrl_crot_free_,
			.save   = trj_ctrl_crot_save_,
			.load   = trj_ctrl_crot_load_,
			.data_size = sizeof(s_trj_ctrl_crot),
			.data   = NULL,
			.config_size = sizeof(s_trj_ctrl_crot_init),
			.config = &trj_ctrl_crot_config_,
			.reset  = trj_ctrl_crot_reset_,
			.update = trj_ctrl_crot_update_,
	});

	static s_trj_ctrl_gm_init trj_ctrl_gm_config_ = {
            .file_name = "res/ctrl/gm/egm2008.txt",
            .order = 12,
	};

	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
            .name   = "default_ctrl_gm",
            .desc   = "default_ctrl_gm",
            .init   = trj_ctrl_gm_init_,
            .free   = trj_ctrl_gm_free_,
            .save   = trj_ctrl_gm_save_,
            .load   = trj_ctrl_gm_load_,
            .data_size = sizeof(s_trj_ctrl_gm),
            .data   = NULL,
            .config_size = sizeof(s_trj_ctrl_gm_init),
            .config = &trj_ctrl_gm_config_,
            .reset  = trj_ctrl_gm_reset_,
            .update = trj_ctrl_gm_update_,
	});

	static s_trj_data_text_init trj_data_text_config_ = {
			.file_name = "default_data_text.txt",
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			.name   = "default_data_text",
			.desc   = "default_data_text",
			.init   = trj_data_text_init_,
			.free   = trj_data_text_free_,
			.save   = trj_data_text_save_,
			.load   = trj_data_text_load_,
			.data_size = sizeof(s_trj_data_text),
			.data   = NULL,
			.config_size = sizeof(s_trj_data_text_init),
			.config = &trj_data_text_config_,
			.render = trj_data_text_render_,
			.reset  = trj_data_text_reset_,
	});
	
	static s_trj_data_ram_init trj_data_ram_config_ = {
			.eng = &self->eng,
			.ref = &self->eng.obj_list[0],
			
			.ellp_en = 0x00,
			.ellp = NULL,
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			.name   = "default_data_ram",
			.desc   = "default_data_ram",
			.init   = trj_data_ram_init_,
			.free   = trj_data_ram_free_,
			.save   = trj_data_ram_save_,
			.load   = trj_data_ram_load_,
			.data_size = sizeof(s_trj_data_ram),
			.data   = NULL,
			.config_size = sizeof(s_trj_data_ram_init),
			.config = &trj_data_ram_config_,
			.render = trj_data_ram_render_,
			.reset  = trj_data_ram_reset_,
	});
	
	static s_trj_data_mat_init trj_data_mat_config_ = {
			.file_name = "default_data_mat.mat",
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			.name   = "default_data_mat",
			.desc   = "default_data_mat",
			.init   = trj_data_mat_init_,
			.free   = trj_data_mat_free_,
			.save   = trj_data_mat_save_,
			.load   = trj_data_mat_load_,
			.data_size = sizeof(s_trj_data_mat),
			.data   = NULL,
			.config_size = sizeof(s_trj_data_mat_init),
			.config = &trj_data_mat_config_,
			.render = trj_data_mat_render_,
			.reset  = trj_data_mat_reset_,
	});
	
	static s_trj_proc_euler_init trj_proc_euler_config_ = {
			.temp = 0x00,
	};
	
	trj_eng_add_procapi(&self->eng, (s_trj_proc) {
			.desc   = "default_proc_euler",
			.init   = trj_proc_euler_init_,
			.free   = trj_proc_euler_free_,
			.save   = trj_proc_euler_save_,
			.load   = trj_proc_euler_load_,
			.update = trj_proc_euler_update_,
			.data_size = sizeof(s_trj_proc_euler),
			.data   = NULL,
			.config_size = sizeof(s_trj_proc_euler_init),
			.config = &trj_proc_euler_config_,
	});
	
	static s_trj_proc_fps_init trj_proc_fps_config_ = {
			.temp = 0x00,
	};
	
	trj_eng_add_procapi(&self->eng, (s_trj_proc) {
			.desc   = "default_proc_fps",
			.init   = trj_proc_fps_init_,
			.free   = trj_proc_fps_free_,
			.save   = trj_proc_fps_save_,
			.load   = trj_proc_fps_load_,
			.update = trj_proc_fps_update_,
			.data_size = sizeof(s_trj_proc_fps),
			.data   = NULL,
			.config_size = sizeof(s_trj_proc_fps_init),
			.config = &trj_proc_fps_config_,
	});

    for (int i = 0; i < sizeof(self->st_gui_eng_obj) / sizeof(s_trj_gui_obj); ++i)
    {
        trj_gui_obj_init(&self->st_gui_eng_obj[i],
                         (s_trj_gui_obj_init) {.ref = &self->eng.obj_list[i]}
        );
    }

    //-----------------------------------------------------------------------------

//	trj_eng_add_obj(&self->eng, (s_trj_obj_init) {.desc = "ref"});
//	trj_eng_add_obj(&self->eng, (s_trj_obj_init) {.desc = "sun"});
//	trj_eng_add_obj(&self->eng, (s_trj_obj_init) {.desc = "earth"});
//	trj_eng_add_obj(&self->eng, (s_trj_obj_init) {.desc = "moon"});
//	trj_eng_add_obj(&self->eng, (s_trj_obj_init) {.desc = "object"});
//
//	trj_obj_add_traj(&self->eng.obj_list[0], self->eng.traj_list[1]);
//	trj_obj_add_traj(&self->eng.obj_list[1], self->eng.traj_list[1]);
//	trj_obj_add_traj(&self->eng.obj_list[2], self->eng.traj_list[1]);
//	trj_obj_add_traj(&self->eng.obj_list[3], self->eng.traj_list[1]);
//	trj_obj_add_traj(&self->eng.obj_list[4], self->eng.traj_list[1]);
//
//	trj_obj_add_traj(&self->eng.obj_list[0], self->eng.traj_list[0]);
//	trj_obj_add_traj(&self->eng.obj_list[1], self->eng.traj_list[0]);
//	trj_obj_add_traj(&self->eng.obj_list[2], self->eng.traj_list[0]);
//	trj_obj_add_traj(&self->eng.obj_list[3], self->eng.traj_list[0]);
//	trj_obj_add_traj(&self->eng.obj_list[4], self->eng.traj_list[0]);
//
//	trj_obj_add_ctrl(&self->eng.obj_list[0], self->eng.ctrl_list[1]);
//	trj_obj_add_ctrl(&self->eng.obj_list[0], self->eng.ctrl_list[3]);
//	trj_obj_add_ctrl(&self->eng.obj_list[1], self->eng.ctrl_list[1]);
//	trj_obj_add_ctrl(&self->eng.obj_list[1], self->eng.ctrl_list[3]);
//	trj_obj_add_ctrl(&self->eng.obj_list[2], self->eng.ctrl_list[1]);
//	trj_obj_add_ctrl(&self->eng.obj_list[2], self->eng.ctrl_list[3]);
//	trj_obj_add_ctrl(&self->eng.obj_list[3], self->eng.ctrl_list[1]);
//	trj_obj_add_ctrl(&self->eng.obj_list[3], self->eng.ctrl_list[3]);
//	trj_obj_add_ctrl(&self->eng.obj_list[4], self->eng.ctrl_list[1]);
//	trj_obj_add_ctrl(&self->eng.obj_list[4], self->eng.ctrl_list[3]);
//
//	trj_obj_add_data(&self->eng.obj_list[0], self->eng.data_list[0]);
//	trj_obj_add_data(&self->eng.obj_list[1], self->eng.data_list[0]);
//	trj_obj_add_data(&self->eng.obj_list[2], self->eng.data_list[0]);
//	trj_obj_add_data(&self->eng.obj_list[3], self->eng.data_list[0]);
//	trj_obj_add_data(&self->eng.obj_list[4], self->eng.data_list[0]);
//
//	trj_obj_add_data(&self->eng.obj_list[0], self->eng.data_list[1]);
//	trj_obj_add_data(&self->eng.obj_list[1], self->eng.data_list[1]);
//	trj_obj_add_data(&self->eng.obj_list[2], self->eng.data_list[1]);
//	trj_obj_add_data(&self->eng.obj_list[3], self->eng.data_list[1]);
//	trj_obj_add_data(&self->eng.obj_list[4], self->eng.data_list[1]);
//
//	trj_obj_add_data(&self->eng.obj_list[0], self->eng.data_list[2]);
//	trj_obj_add_data(&self->eng.obj_list[1], self->eng.data_list[2]);
//	trj_obj_add_data(&self->eng.obj_list[2], self->eng.data_list[2]);
//	trj_obj_add_data(&self->eng.obj_list[3], self->eng.data_list[2]);
//	trj_obj_add_data(&self->eng.obj_list[4], self->eng.data_list[2]);
//
//	for (uint32_t i = 0; i < self->eng.obj_count; ++i)
//	{
//		vl_mid(&self->eng.obj_list[i].rot[0][0]);
//	}
//
	ImGui::StyleColorsDark();
//	ImGui::StyleColorsLight();
	
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
	style_ref.PopupBorderSize = 1.0;
	style_ref.TabBorderSize = 0.0;
	style_ref.WindowBorderSize = 0.0;
	
	trj_gui_env_init(&self->gui_env, (s_trj_gui_env_init) {
		.eng = &self->eng,
		.traj_offset = &self->eng.traj_offset,
		.ctrl_offset = &self->eng.ctrl_offset,
		.data_offset = &self->eng.data_offset,
		
		.traj_list = self->eng.traj_list,
		.ctrl_list = self->eng.ctrl_list,
		.data_list = self->eng.data_list,
	});
	
	trj_gui_cmd_init(&self->gui_cmd, (s_trj_gui_cmd_init)
	{
		.env = &self->gui_env,
		.title = "env",
		.visible = false
	});
	
	self->gui_menu.env = &self->gui_env;
	self->gui_menu.cmd = &self->gui_cmd;
	self->gui_menu.eng = &self->eng;
	
	trj_eng_load(&self->eng, "files/test.trj");
	
//	trj_gui_eng_sel_data(&self->gui_eng, &self->eng.obj_list[0].data_list[0]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_gui_main(s_trj_gui *self)
{
	int static_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus
					   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	
	bool show_demo_window = true;
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	
	const uint32_t traj_hash_static = vl_crc32("default_traj_static");
	const uint32_t traj_hash_orb    = vl_crc32("default_traj_orb");
	const uint32_t traj_hash_bz     = vl_crc32("default_traj_bz");
	
	const uint32_t data_hash_text = vl_crc32("default_data_text");
	const uint32_t data_hash_ram  = vl_crc32("default_data_ram");

	const uint32_t ctrl_hash_gm   = vl_crc32("default_ctrl_gm");

	trj_gui_menu_main(&self->gui_menu);
	
	{
		// Toolbar
		ImGui::SetNextWindowPos((ImVec2) { 0, (float) self->gui_menu.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) { self->w_width, (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("toolbar", NULL, static_flags | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
		trj_gui_tbar_main(&self->gui_tbar);
		ImGui::End();
	}
	
	{
		// progress popup
		// must be after toolbar
		trj_gui_eng_updategui(&self->gui_eng, &self->eng);
	}
	
	{
		// Object list
		ImGui::SetNextWindowPos((ImVec2) {0, (float) self->gui_menu.height + (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) {240, self->w_height - self->gui_menu.height - self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("obj_list", NULL, static_flags);
		trj_gui_eng_objlist(&self->gui_eng, &self->eng);
		ImGui::End();
	}
	
	{
		// Object edit
		ImGui::SetNextWindowPos((ImVec2) {240, (float) self->gui_menu.height + (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) {240, self->w_height - self->gui_menu.height - self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("item_edit", NULL, static_flags);
		
		if (self->gui_eng.sel_item != NULL)
		{
			switch (self->gui_eng.sel_type)
			{
				case trj_gui_eng_type_obj:
				{
					s_trj_gui_obj *obj_gui = (s_trj_gui_obj*) self->gui_eng.sel_item;
					s_trj_obj *obj = obj_gui->ref;
					
					trj_gui_obj_edit(obj_gui, obj);
					
					break;
				}
				
				case trj_gui_eng_type_traj:
				{
					s_trj_traj *traj = (s_trj_traj*) self->gui_eng.sel_item;
					trj_gui_traj_edit(traj);
					
					if      (traj->hash == traj_hash_static) { trj_gui_traj_edit_static (traj); }
					else if (traj->hash == traj_hash_orb   ) { trj_gui_traj_edit_orb    (traj); }
					else if (traj->hash == traj_hash_bz    ) { trj_gui_traj_edit_bz     (traj); }
					
					break;
				}
				
				case trj_gui_eng_type_ctrl:
				{
					s_trj_ctrl *ctrl = (s_trj_ctrl*) self->gui_eng.sel_item;
					trj_gui_ctrl_edit(ctrl);

                    if      (ctrl->hash == ctrl_hash_gm) { trj_gui_ctrl_edit_gm(ctrl); }
//                    else if (traj->hash == traj_hash_orb   ) { trj_gui_traj_edit_orb    (traj); }
//                    else if (traj->hash == traj_hash_bz    ) { trj_gui_traj_edit_bz     (traj); }

                    break;
				}
				
				case trj_gui_eng_type_data:
				{
					s_trj_data *data = (s_trj_data*) self->gui_eng.sel_item;
					trj_gui_data_edit(data);
					
					if      (data->hash == data_hash_text) { trj_gui_data_edit_text(data); }
					else if (data->hash == data_hash_ram ) { trj_gui_data_edit_ram(data); }
					
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
		ImGui::SetNextWindowPos ((ImVec2) {240*2, (float) self->gui_menu.height + (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) {self->w_width - 240*2, self->w_height - self->gui_menu.height - self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("main_view", NULL, static_flags);
		
		if (self->gui_eng.sel_item != NULL)
		{
			switch (self->gui_eng.sel_type)
			{
				case trj_gui_eng_type_obj:
				{
					s_trj_gui_obj *obj_gui = (s_trj_gui_obj*) self->gui_eng.sel_item;
					s_trj_obj *obj = obj_gui->ref;
					
//					trj_gui_obj_view(obj_gui, obj);
					
					break;
				}
				
				case trj_gui_eng_type_traj:
				{
					s_trj_traj *traj = (s_trj_traj*) self->gui_eng.sel_item;
					
					if      (traj->hash == traj_hash_static) { trj_gui_traj_view_static (traj); }
					else if (traj->hash == traj_hash_orb   ) { trj_gui_traj_view_orb    (traj); }
					else if (traj->hash == traj_hash_bz    ) { trj_gui_traj_view_bz((s_trj_traj_bz *) traj->data, "##test", ImVec2(-1, -1), 0x00); }
					
					break;
				}
				
				case trj_gui_eng_type_ctrl:
				{
                    s_trj_ctrl *ctrl = (s_trj_ctrl*) self->gui_eng.sel_item;

                    if      (ctrl->hash == ctrl_hash_gm) { trj_gui_ctrl_view_gm(ctrl); }
//                    else if (traj->hash == traj_hash_orb   ) { trj_gui_traj_view_orb    (traj); }

					break;
				}
				
				case trj_gui_eng_type_data:
				{
					s_trj_data *data = (s_trj_data*) self->gui_eng.sel_item;
					
					if      (data->hash == data_hash_text) { trj_gui_data_view_text(data); }
					else if (data->hash == data_hash_ram ) { trj_gui_data_view_ram(data); }
					
					break;
				}
				
				default: break;
			}
		}
		
		else
		{
			// default view (can be used for debug)
//			trj_gui_map_view(&map, "map", ImVec2(-1, -1));
			
			static s_vl3d_obj  obj_list[20000];
			static s_vl3d_eng  vl3d;
			
			s_vl3d_view view = {
					.scale = 1.0,
					.pos = { 0.0, 0.0, 0.0 },
					
					.tbar_en = 0x01,
					
					.grid_mode = 0x01,
					.grid_pt_size = 2.0,
					.grid_pt_disp = 2.0,
					
					.xyz_en = 0x01,
					.xyz_scale = 0.25
			};
			
			vl3d_view_load(self, &view, view);
			
			view.scale = 1.0;
			view.grid_pt_disp = 1.0;
			view.tbar_en = 0x01;
			view.grid_mode = 0x02;
			view.xyz_en = 0x01;
			
			vl3d_eng_init(&vl3d, (s_vl3d_eng_init) { .obj_list = obj_list, });
			
			ImGuiIO io = ImGui::GetIO();
			
			int cnt = 6;
			static vlf_t a_time = 0.0;
			a_time += 0.5 * io.DeltaTime;
			vlf_t a_state = 0.5 * (1.0 + vl_sin(a_time));
			
			static vlf_t d_ = 1.5 * vl_pi;
			d_ += io.DeltaTime;
			if (d_ > 2.5*vl_pi) { d_ = 2.5*vl_pi; }
			
			vlf_t d = 0.5 * (1.0 + vl_sin(d_)) * (1.0 / view.scale / 4.0);
			
			vlf_t pt_norm = vl_gauss1(0.0, 0.0, view.grid_pt_disp);
			vlf_t gain;
			ImVec4 color_v4;
			
			s_vl3d_point point;
			s_vl3d_line  line;
			
			s_trj_rot_hpr view_hpr;
			trj_ctn_to_hpr(&view_hpr, &view.rot[0][0]);
			view_hpr.heading += io.DeltaTime * 0.01;
			view_hpr.pitch   += io.DeltaTime * 0.02;
			view_hpr.roll    += io.DeltaTime * 0.03;
			trj_hpr_to_ctn(&view.rot[0][0], view_hpr);
			
			for (int x = 0; x < cnt; ++x)
			{
				for (int y = 0; y < cnt; ++y)
				{
					for (int z = 0; z < cnt; ++z)
					{
						gain = vl_sqrt(x * x + y * y + z * z);
						gain = 0.4 * vl_gauss1(gain * a_state, 0.0, view.grid_pt_disp) / pt_norm;
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						point.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						point.size = view.grid_pt_size;
						
						gain = vl_sqrt(x * x + y * y + z * z);
						gain = 0.2 * vl_gauss1(gain * a_state, 0.0, view.grid_pt_disp) / pt_norm;
						color_v4 = ImGui::ColorConvertU32ToFloat4(vl3d_col_d);
						color_v4.w *= gain;
						
						line.color = ImGui::ColorConvertFloat4ToU32(color_v4);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, y * d, z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, y * d, -z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, -y * d, z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {x * d, -y * d, -z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, y * d, z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, y * d, -z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, -y * d, z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						vl_vcopy(point.p0, (vlf_t[3]) {-x * d, -y * d, -z * d});
						vl3d_eng_add_point(&vl3d, point);
						
						// 1
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, d*y, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y + d, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y, d*z + d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 2
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, d*y, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y + d, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, d*y, -d*z - d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 3
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, -d*y, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y - d, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y, d*z + d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 4
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x + d, -d*y, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y - d, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { d*x, -d*y, -d*z - d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 5
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, d*y, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y + d, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y, d*z + d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 6
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, d*y, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y + d, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, d*y, -d*z - d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 7
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, -d*y, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y - d, d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y, d*z + d });
						vl3d_eng_add_line(&vl3d, line);
						
						// 8
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x - d, -d*y, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y - d, -d*z });
						vl3d_eng_add_line(&vl3d, line);
						
						vl_vcopy(line.p0, (vlf_t[3]) { -d*x, -d*y, -d*z });
						vl_vcopy(line.p1, (vlf_t[3]) { -d*x, -d*y, -d*z - d });
						vl3d_eng_add_line(&vl3d, line);
					}
				}
			}
			
//			static vl3d_text text = (s_vl3d_text) { .p0 = { 0, 0, 0 }, .data = "hello", .color = vl3d_col_l };
//			vl3d_eng_add_text(&vl3d, text);

//			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(0x00));

			vl3d_eng_render(&vl3d, &view, "temp", ImVec2(-1, -1));
			vl3d_view_save(self, &view);
			
//			ImGui::PopStyleColor();
		}
		
		ImGui::End();
	}
	
	{
		// Scripting view
		trj_gui_cmd_render(&self->gui_cmd);
	}
	
	bool mw = true;
	ImGui::ShowMetricsWindow(&mw);
	
	return 0x00;
}

//------------------------------------------------------------------------------
