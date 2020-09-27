
#include "trj_gui.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init_attr attr)
{
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
		
		static ImVec2 values[255];
		int pt_count = 4;
		
		static s_trj_bz4 bz4 = {
			.p0 = { 0.0, 0.0 },
			.p1 = { 1.0, 1.0 },
			.p2 = { 2.0, 1.0 },
			.p3 = { 3.0, 0.0 },
		};
		
		values[0] = ImVec2(bz4.p0[0], bz4.p0[1]);
		values[1] = ImVec2(bz4.p1[0], bz4.p1[1]);
		values[2] = ImVec2(bz4.p2[0], bz4.p2[1]);
		values[3] = ImVec2(bz4.p3[0], bz4.p3[1]);
		
		for (int i = 0; i < 100; ++i)
		{
			values[4+i].x = 3 * (vlf_t) i / 100;
			vlf_t temp;
			trj_bz4_eval(&bz4, values[4+i].x, &temp);

			values[4+i].y = temp;

			pt_count++;
		}

		CurveEditor("##ce", (float*) values, pt_count, ImVec2(400, 400),
			(ImU32) CurveEditorFlags::SHOW_GRID | (ImU32) CurveEditorFlags::NO_TANGENTS, &pt_count);
		
		bz4.p0[0] = values[0].x;
		bz4.p0[1] = values[0].y;
		bz4.p1[0] = values[1].x;
		bz4.p1[1] = values[1].y;
		bz4.p2[0] = values[2].x;
		bz4.p2[1] = values[2].y;
		bz4.p3[0] = values[3].x;
		bz4.p3[1] = values[3].y;
		
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
	
	return 0x00;
}

//------------------------------------------------------------------------------
