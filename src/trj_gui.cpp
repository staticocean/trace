
#include "trj_gui.h"

//------------------------------------------------------------------------------

static s_trj_traj_bz_point pts[2048];
static s_trj_traj_bz traj_bz = {.pts = pts, .pts_offset = 0 };

static  s_vl3d_obj vl3d_obj_list[255];
static  s_vl3d_eng vl3d_eng;
static  s_vl3d_view vl3d_view;

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init attr)
{
	trj_gui_eng_add_trajapi(&self->gui_eng, (s_trj_traj_api) {
			.rot = NULL,
			.pos = trj_traj_bz_pos_api,
			.compile = trj_traj_bz_compile_api,
			.desc = "traj_bz",
	});
	
	trj_gui_eng_add_ctrlapi(&self->gui_eng, (s_trj_ctrl_api) {
			.reset = NULL,
			.update = NULL,
			.desc = "ctrl_test",
	});
	
	traj_bz.pts_offset = 64;
	
	for (int i = 0; i < traj_bz.pts_offset; ++i)
	{
		traj_bz.pts[i].time = i;
		
		traj_bz.pts[i].pos_p[0] = rand();
		traj_bz.pts[i].pos_p[1] = rand();
		traj_bz.pts[i].pos_p[2] = rand();
		
		traj_bz.pts[i].rot_p[0] = rand();
		traj_bz.pts[i].rot_p[1] = rand();
		traj_bz.pts[i].rot_p[2] = rand();
	}
	
	vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init_attr) { .obj_list = vl3d_obj_list });
	

	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 1.0, 0.0, 0.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 1.0 }, .p1 = { 1.0, 0.0, 1.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 1.0, 0.0 }, .p1 = { 1.0, 1.0, 0.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 1.0, 1.0 }, .p1 = { 1.0, 1.0, 1.0 }, });

	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 0.0, 0.0, 1.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 1.0, 0.0 }, .p1 = { 0.0, 1.0, 1.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.0 }, .p1 = { 1.0, 0.0, 1.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 1.0, 0.0 }, .p1 = { 1.0, 1.0, 1.0 }, });

	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 0.0, 1.0, 0.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 1.0 }, .p1 = { 0.0, 1.0, 1.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.0 }, .p1 = { 1.0, 1.0, 0.0 }, });
	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 1.0 }, .p1 = { 1.0, 1.0, 1.0 }, });
	
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.0 }, .p1 = { 0.5, 1.0, 0.0 }, }); // /
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.5, 1.0, 0.0 }, .p1 = { 1.0, 0.0, 0.0 }, }); //
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.0 }, .p1 = { 0.0, 0.5, 0.0 }, }); //
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.5, 0.0 }, .p1 = { 1.0, 0.5, 0.0 }, }); //
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.5, 0.0 }, .p1 = { 0.0, 0.0, 0.0 }, }); //
//
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.0, 0.1 }, .p1 = { 0.5, 1.0, 0.1 }, }); // /
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.5, 1.0, 0.1 }, .p1 = { 1.0, 0.0, 0.1 }, }); //
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.0, 0.1 }, .p1 = { 0.0, 0.5, 0.1 }, }); //
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 0.0, 0.5, 0.1 }, .p1 = { 1.0, 0.5, 0.1 }, }); //
//	vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .p0 = { 1.0, 0.5, 0.1 }, .p1 = { 0.0, 0.0, 0.1 }, }); //
	
	vl3d_view.scale = 1.0;
	
	vl_vzero(vl3d_view.pos);
	vl_mid(&vl3d_view.rot[0][0]);
	
	trj_traj_bz_compile(&traj_bz);
	
	self->w_height = 800;
	self->w_width  = 800;
	
	trj_gui_eng_init(&self->gui_eng, (s_trj_gui_eng_init) { .obj_list = self->st_gui_eng_obj });
	trj_eng_init(&self->eng, (s_trj_eng_init) { .st_objects = self->st_eng_obj });
	
	trj_eng_add(&self->eng, (s_trj_obj_init) { .name = "test object 00", .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .name = "test object 01", .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .name = "test object 02", .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj_init) { .name = "test object 03", .ref = &self->eng.obj_list[0] });
	
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
	style_ref.WindowBorderSize = 0.0;
	
	trj_gui_env_init(&self->gui_env, (s_trj_gui_env_init) { .eng = &self->eng });
	
	trj_gui_cmd_init(&self->gui_cmd, (s_trj_gui_cmd_init)
	{
		.env = &self->gui_env,
		.title = "env",
		.visible = true
	});
	
	self->gui_menu.env = &self->gui_env;
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_gui_main(s_trj_gui *self)
{
	int static_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus
					   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	
	bool show_demo_window = true;
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	
	trj_gui_menu_main(&self->gui_menu);
	
	{
		// Object list
		ImGui::SetNextWindowPos((ImVec2) {0, (float) self->gui_menu.height});
		ImGui::SetNextWindowSize((ImVec2) {200, self->w_height - self->gui_menu.height});
		ImGui::Begin("obj_list", NULL, static_flags);
		trj_gui_eng_objlist(&self->gui_eng, &self->eng);
		ImGui::End();
	}
	
	{
		// Object edit
		ImGui::SetNextWindowPos((ImVec2) {200, (float) self->gui_menu.height});
		ImGui::SetNextWindowSize((ImVec2) {200, self->w_height - self->gui_menu.height});
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
					s_trj_traj_api *api = (s_trj_traj_api*) self->gui_eng.sel_item;
					trj_gui_traj_bz((s_trj_traj_bz*) api, "##test", ImVec2(-1, 400), 0x00);
					
					break;
				}
				
				default:
					break;
			}
		}
		
		trj_gui_eng_addbox(&self->gui_eng, &self->eng);
		
		ImGui::End();
	}
	
	{
//		// Main view
//		ImGui::SetNextWindowPos ((ImVec2) {400, (float) self->gui_menu.height});
//		ImGui::SetNextWindowSize((ImVec2) {self->w_width - 400, self->w_height - self->gui_menu.height});
//		ImGui::Begin("main_view", NULL, static_flags);
//
//		bool view_res = ImGui::Button("view_res");
//
//		trj_gui_traj_bz(&traj_bz, "##temp", ImVec2(-1, 600), view_res);
//
//		ImGui::End();
		
		
		// Main view
		ImGui::SetNextWindowPos ((ImVec2) {400, (float) self->gui_menu.height});
		ImGui::SetNextWindowSize((ImVec2) {self->w_width - 400, self->w_height - self->gui_menu.height});
		ImGui::Begin("main_view", NULL, static_flags);
		
		vl3d_eng_render(&vl3d_eng, &vl3d_view, "##temp", ImVec2(-1, 600));
		
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
