
#include "trj_gui.h"

//------------------------------------------------------------------------------

static s_trj_traj_bz_point pts[2048];
static s_trj_traj_bz traj_bz = {.pts = pts, .pts_offset = 0 };

static  s_vl3d_obj vl3d_obj_list[255];
static  s_vl3d_eng vl3d_eng;
static  s_vl3d_view vl3d_view;

static 	s_trj_gui_map map;

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init attr)
{
	s_trj_gui_map *temp_map = new s_trj_gui_map();
	map = *temp_map;
	free(temp_map);
	
	trj_gui_map_load(&map, "res/maps/earth/countries.geojson");
//	trj_gui_map_load(&map, "res/maps/earth/cities.geojson");

//	printf("s_trj_obj %d \r\n", sizeof(s_trj_obj));
//	vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) { .obj_list = vl3d_obj_list });
//
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 1.0, 0.0, 0.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 1.0 }, .p1 = { 1.0, 0.0, 1.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 1.0, 0.0 }, .p1 = { 1.0, 1.0, 0.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 1.0, 1.0 }, .p1 = { 1.0, 1.0, 1.0 }, });
//
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 0.0, 0.0, 1.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 1.0, 0.0 }, .p1 = { 0.0, 1.0, 1.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.0 }, .p1 = { 1.0, 0.0, 1.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 1.0, 0.0 }, .p1 = { 1.0, 1.0, 1.0 }, });
//
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 0.0, 1.0, 0.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 1.0 }, .p1 = { 0.0, 1.0, 1.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.0 }, .p1 = { 1.0, 1.0, 0.0 }, });
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 1.0 }, .p1 = { 1.0, 1.0, 1.0 }, });
//
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 0.5, 1.0, 0.0 }, }); // /
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.5, 1.0, 0.0 }, .p1 = { 1.0, 0.0, 0.0 }, }); //
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.0 }, .p1 = { 0.0, 0.5, 0.0 }, }); //
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.5, 0.0 }, .p1 = { 1.0, 0.5, 0.0 }, }); //
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.5, 0.0 }, .p1 = { 0.0, 0.0, 0.0 }, }); //
////
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.1 }, .p1 = { 0.5, 1.0, 0.1 }, }); // /
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.5, 1.0, 0.1 }, .p1 = { 1.0, 0.0, 0.1 }, }); //
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.1 }, .p1 = { 0.0, 0.5, 0.1 }, }); //
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.5, 0.1 }, .p1 = { 1.0, 0.5, 0.1 }, }); //
////	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.5, 0.1 }, .p1 = { 0.0, 0.0, 0.1 }, }); //
//
//	vl3d_view.scale = 1.0;
//
//	vl_vzero(vl3d_view.pos);
//	vl_mid(&vl3d_view.rot[0][0]);
	
	self->w_height = 720;
	self->w_width  = 1024;
	
	self->gui_tbar.eng = &self->eng;
	self->gui_tbar.eng_gui = &self->gui_eng;
	self->gui_tbar.height = 32;
	
	trj_gui_eng_init(&self->gui_eng, (s_trj_gui_eng_init) { .obj_list = self->st_gui_eng_obj });
	
	trj_eng_init(&self->eng, (s_trj_eng_init) {
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
			.id = trj_traj_orb_id,
			.desc = "default_traj_orb",
			.init = trj_traj_orb_init_,
			.free = trj_traj_orb_free_,
			.data = NULL,
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
			.id = trj_traj_bz_id,
			.desc = "default_traj_bz",
			.init = trj_traj_bz_init_,
			.free = trj_traj_bz_free_,
			.data = NULL,
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
			.id     = 0x00,
			.desc   = "default_ctrl_upos",
			.init   = trj_ctrl_upos_init_,
			.free   = trj_ctrl_upos_free_,
			.data   = NULL,
			.config = &trj_ctrl_upos_config_,
			.reset  = trj_ctrl_upos_reset_,
			.update = trj_ctrl_upos_update_,
	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.id     = 0x00,
			.desc   = "default_ctrl_cpos",
			.init   = trj_ctrl_cpos_init_,
			.free   = trj_ctrl_cpos_free_,
			.data   = NULL,
			.config = &trj_ctrl_cpos_config_,
			.reset  = trj_ctrl_cpos_reset_,
			.update = trj_ctrl_cpos_update_,
	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.id     = 0x00,
			.desc   = "default_ctrl_urot",
			.init   = trj_ctrl_urot_init_,
			.free   = trj_ctrl_urot_free_,
			.data   = NULL,
			.config = &trj_ctrl_urot_config_,
			.reset  = trj_ctrl_urot_reset_,
			.update = trj_ctrl_urot_update_,
	});
	
	trj_eng_add_ctrlapi(&self->eng, (s_trj_ctrl) {
			.id     = 0x00,
			.desc   = "default_ctrl_crot",
			.init   = trj_ctrl_crot_init_,
			.free   = trj_ctrl_crot_free_,
			.data   = NULL,
			.config = &trj_ctrl_crot_config_,
			.reset  = trj_ctrl_crot_reset_,
			.update = trj_ctrl_crot_update_,
	});
	
	static s_trj_data_ram_init trj_data_ram_config_ = {
			.temp = 0x00,
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			.id     = trj_data_ram_id,
			.desc   = "default_data_ram",
			.init   = trj_data_ram_init_,
			.free   = trj_data_ram_free_,
			.data   = NULL,
			.config = &trj_data_ram_config_,
			.render = trj_data_ram_render_,
			.reset  = trj_data_ram_reset_,
	});
	
	static s_trj_data_text_init trj_data_text_config_ = {
			.temp = 0x00,
	};
	
	trj_eng_add_dataapi(&self->eng, (s_trj_data) {
			.id     = trj_data_text_id,
			.desc   = "default_data_text",
			.init   = trj_data_text_init_,
			.free   = trj_data_text_free_,
			.data   = NULL,
			.config = &trj_data_text_config_,
			.render = trj_data_text_render_,
			.reset  = trj_data_text_reset_,
	});
	
	static s_trj_proc_euler_init trj_proc_euler_config_ = {
			.temp = 0x00,
	};
	
	trj_eng_add_procapi(&self->eng, (s_trj_proc) {
			.id     = trj_proc_euler_id,
			.desc   = "default_proc_euler",
			.init   = trj_proc_euler_init_,
			.free   = trj_proc_euler_free_,
			.update = trj_proc_euler_update_,
			.data   = NULL,
			.config = &trj_proc_euler_config_,
	});
	
	trj_eng_add(&self->eng, (s_trj_obj_init) { .desc = "ref"   , .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .desc = "sun"   , .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .desc = "earth" , .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .desc = "moon"  , .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .desc = "object", .ref = &self->eng.obj_list[0] });
	
	for (int i = 0; i < sizeof(self->st_gui_eng_obj) / sizeof(s_trj_gui_obj); ++i)
	{
		trj_gui_obj_init(&self->st_gui_eng_obj[0],
				(s_trj_gui_obj_init) {.ref = &self->eng.obj_list[0]}
				);
	}
	
	trj_obj_add_traj(&self->eng.obj_list[0], self->eng.traj_list[1]);
	trj_obj_add_traj(&self->eng.obj_list[1], self->eng.traj_list[1]);
	trj_obj_add_traj(&self->eng.obj_list[2], self->eng.traj_list[1]);
	trj_obj_add_traj(&self->eng.obj_list[3], self->eng.traj_list[1]);
	trj_obj_add_traj(&self->eng.obj_list[4], self->eng.traj_list[1]);
	
	trj_obj_add_traj(&self->eng.obj_list[0], self->eng.traj_list[0]);
	trj_obj_add_traj(&self->eng.obj_list[1], self->eng.traj_list[0]);
	trj_obj_add_traj(&self->eng.obj_list[2], self->eng.traj_list[0]);
	trj_obj_add_traj(&self->eng.obj_list[3], self->eng.traj_list[0]);
	trj_obj_add_traj(&self->eng.obj_list[4], self->eng.traj_list[0]);
	
	trj_obj_add_ctrl(&self->eng.obj_list[0], self->eng.ctrl_list[1]);
	trj_obj_add_ctrl(&self->eng.obj_list[0], self->eng.ctrl_list[3]);
	trj_obj_add_ctrl(&self->eng.obj_list[1], self->eng.ctrl_list[1]);
	trj_obj_add_ctrl(&self->eng.obj_list[1], self->eng.ctrl_list[3]);
	trj_obj_add_ctrl(&self->eng.obj_list[2], self->eng.ctrl_list[1]);
	trj_obj_add_ctrl(&self->eng.obj_list[2], self->eng.ctrl_list[3]);
	trj_obj_add_ctrl(&self->eng.obj_list[3], self->eng.ctrl_list[1]);
	trj_obj_add_ctrl(&self->eng.obj_list[3], self->eng.ctrl_list[3]);
	trj_obj_add_ctrl(&self->eng.obj_list[4], self->eng.ctrl_list[1]);
	trj_obj_add_ctrl(&self->eng.obj_list[4], self->eng.ctrl_list[3]);
	
	trj_obj_add_data(&self->eng.obj_list[0], self->eng.data_list[0]);
	trj_obj_add_data(&self->eng.obj_list[1], self->eng.data_list[0]);
	trj_obj_add_data(&self->eng.obj_list[2], self->eng.data_list[0]);
	trj_obj_add_data(&self->eng.obj_list[3], self->eng.data_list[0]);
	trj_obj_add_data(&self->eng.obj_list[4], self->eng.data_list[0]);
	
	trj_gui_eng_sel_data(&self->gui_eng, &self->eng.obj_list[0].data_list[0]);
	
	for (uint32_t i = 0; i < self->eng.obj_count; ++i)
	{
		vl_mid(&self->eng.obj_list[i].rot[0][0]);
	}
	
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
	style_ref.PopupBorderSize = 0.0;
	style_ref.TabBorderSize = 0.0;
	style_ref.WindowBorderSize = 1.0;
	
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
	
	self->gui_eng.time_limit = 3600.0;
	self->gui_eng.time_step = 0.01;
	self->gui_eng.time_iter = self->gui_eng.time_limit
						    / self->gui_eng.time_step;
	
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
					s_trj_obj *obj = (s_trj_obj*) self->gui_eng.sel_item;
					trj_gui_obj_edit(&self->gui_eng.obj_list[obj->id], obj);
					
					break;
				}
				
				case trj_gui_eng_type_traj:
				{
					s_trj_traj *traj = (s_trj_traj*) self->gui_eng.sel_item;
					trj_gui_traj_edit(traj);
					
					switch (traj->id)
					{
						case trj_traj_orb_id : { trj_gui_traj_orb_edit(traj); break; }
						case trj_traj_bz_id  : { trj_gui_traj_bz_edit  (traj); break; }
						
						default: break;
					}
					
					break;
				}
				
				case trj_gui_eng_type_ctrl:
				{
					s_trj_ctrl *ctrl = (s_trj_ctrl*) self->gui_eng.sel_item;
					trj_gui_ctrl_edit(ctrl);
					
					break;
				}
				
				case trj_gui_eng_type_data:
				{
					s_trj_data *data = (s_trj_data*) self->gui_eng.sel_item;
					trj_gui_data_edit(data);
					
					switch (data->id)
					{
						case trj_data_text_id : { trj_gui_data_text_edit(data); break; }
						case trj_data_ram_id  : { trj_gui_data_ram_edit (data); break; }
						
						default: break;
					}
					
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
					s_trj_obj *obj = (s_trj_obj*) self->gui_eng.sel_item;
					s_trj_gui_obj *obj_gui = &self->st_gui_eng_obj[obj->id];
					
					trj_gui_obj_view(obj_gui, obj);
					
					break;
				}
				
				case trj_gui_eng_type_traj:
				{
					s_trj_traj *traj = (s_trj_traj*) self->gui_eng.sel_item;
					
					switch (traj->id)
					{
						case trj_traj_orb_id: { trj_gui_traj_orb_view(traj); break; }
						case trj_traj_bz_id : { trj_gui_traj_bz_view((s_trj_traj_bz *) traj->data, "##test", ImVec2(-1, -1), 0x00); break; }
						
						default: break;
					}
					
					break;
				}
				
				case trj_gui_eng_type_ctrl:
				{
					break;
				}
				
				case trj_gui_eng_type_data:
				{
					s_trj_data *data = (s_trj_data*) self->gui_eng.sel_item;
					
					switch (data->id)
					{
						case trj_data_text_id : { trj_gui_data_text_view(data); break; }
						case trj_data_ram_id  : { trj_gui_data_ram_view (data); break; }
						
						default: break;
					}
					
					break;
				}
				
				default:
					break;
			}
		}
		
		else
		{
			// default view (can be used for debug)
			trj_gui_map_view(&map, "map", ImVec2(-1, -1));
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
