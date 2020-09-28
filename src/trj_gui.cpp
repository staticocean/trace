
#include "trj_gui.h"

//------------------------------------------------------------------------------

static s_trj_traj_bz_point pts[2048];
static s_trj_traj_bz traj_bz = {.pts = pts, .pts_offset = 0 };

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init_attr attr)
{
	traj_bz.pts_offset = 64;
	
	for (int i = 0; i < traj_bz.pts_offset; ++i)
	{
		traj_bz.pts[i].p[0] = i;
		traj_bz.pts[i].p[1] = rand();
	}
	
	self->w_height = 800;
	self->w_width  = 800;
	
	self->gui_eng.sel_item = NULL;
	self->gui_eng.sel_type = 0x00;
	self->gui_eng.obj_hide = self->st_gui_eng_objhide;
	
	for (int i = 0; i < 255; ++i)
	{
		self->gui_eng.obj_hide[i] = 0x00;
	}
	
	trj_eng_init(&self->eng, (s_trj_eng_init_attr) { .st_objects = self->st_eng_obj });
	
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 00", .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 01", .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 02", .ref = &self->eng.obj_list[0] });
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 03", .ref = &self->eng.obj_list[0] });
	
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
				case 0x00:
				{
					trj_gui_obj_edit((s_trj_obj *) self->gui_eng.sel_item);
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
		
		//	ImGui::SetNextWindowPos((ImVec2) { 400, (float) self->gui_menu.height });
		//	ImGui::SetNextWindowSize((ImVec2) { 0, 0 });
		ImGui::SetNextWindowPos((ImVec2) {400, (float) self->gui_menu.height});
		ImGui::SetNextWindowSize((ImVec2) {self->w_width - 400, self->w_height - self->gui_menu.height});
		ImGui::Begin("main_view", NULL, static_flags);
		
		bool view_res = ImGui::Button("view_res");
		
		trj_gui_traj_bz(&traj_bz, "##temp", ImVec2(600, 600), view_res);
		
		ImGui::End();
	}
	
	{
		// Scripting view
		static char src_data[1024];
		
		ImGui::Begin("script_edit", NULL, ImGuiWindowFlags_NoCollapse);
		
		ImGui::InputTextMultiline("", src_data, 1024);
		
		if (ImGui::Button("compile"))
		{
		
		}
		
		ImGui::End();
	}
	
	bool mw = true;
	ImGui::ShowMetricsWindow(&mw);
	
	return 0x00;
}

//------------------------------------------------------------------------------
