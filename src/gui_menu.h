
#ifndef __GUI_MENU__
#define __GUI_MENU__

//----------------------------------------------------------------

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <softael_lib/vl.h>

#include <lib/imgui/imgui.h>
#include <lib/trj/trj_eng.h>

#include "gui_env.h"
#include "gui_cmd.h"

//----------------------------------------------------------------

typedef struct gui_menu
{
	s_gui_env *env;
	s_gui_cmd *cmd;

	s_trj_eng *eng;
	
	int height;
	
} 	s_gui_menu;

//----------------------------------------------------------------

inline uint8_t gui_menu_file(s_gui_menu *self)
{
//	ImGui::MenuItem("(demo menu)", NULL, false, false);
	
//	if (ImGui::MenuItem("New")) {}
	{
		if (ImGui::MenuItem("Open", "Ctrl+O"))
		{
//			__file_browser_open__.Open();
		}
		
//		if(__file_browser_open__.HasSelected())
//		{
//			trj_eng_load(self->eng, (char*) __file_browser_open__.GetSelected().string().c_str());
//			__file_browser_open__.ClearSelected();
//		}
	}
//	if (ImGui::BeginMenu("Open Recent"))
//	{
//		ImGui::MenuItem("fish_hat.c");
//		ImGui::MenuItem("fish_hat.inl");
//		ImGui::MenuItem("fish_hat.h");
//
//		if (ImGui::BeginMenu("More.."))
//		{
//			ImGui::MenuItem("Hello");
//			ImGui::MenuItem("Sailor");
//
//			if (ImGui::BeginMenu("Recurse.."))
//			{
//				gui_menu_file(self);
//				ImGui::EndMenu();
//			}
//			ImGui::EndMenu();
//		}
//		ImGui::EndMenu();
//	}
//
	if (ImGui::MenuItem("Save", "Ctrl+S")) { trj_eng_save(self->eng, "res/saves/test.trj"); }
//	if (ImGui::MenuItem("Save As..")) {}
	
	ImGui::Separator();
	
//	if (ImGui::BeginMenu("Options"))
//	{
//		static bool enabled = true;
//
//		ImGui::MenuItem("Enabled", "", &enabled);
//		ImGui::BeginChild("child", ImVec2(0, 60), true);
//
//		for (int i = 0; i < 10; i++)
//			ImGui::Text("Scrolling Text %d", i);
//
//		ImGui::EndChild();
//
//		static float f = 0.5f;
//		static int n = 0;
//
//		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
//		ImGui::InputFloat("Input", &f, 0.1f);
//		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
//		ImGui::EndMenu();
//	}
	
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		
		ImGui::EndMenu();
	}
	
	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
//	if (ImGui::BeginMenu("Options")) // <-- Append!
//	{
//		static bool b = true;
//
//		ImGui::Checkbox("SomeOption", &b);
//		ImGui::EndMenu();
//	}
//
//	if (ImGui::BeginMenu("Disabled", false)) // Disabled
//	{
//		IM_ASSERT(0);
//	}
//	if (ImGui::MenuItem("Checked", NULL, true)) {}
//	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t gui_menu_main(s_gui_menu *self)
{
	if (ImGui::BeginMainMenuBar())
	{
		self->height = ImGui::GetWindowHeight();
		
		// Temporary disabled because ImGui filebrowser cant be rendered from menu
		// thats why SAVE/LOAD is placed in toolbar and thats why we dont have a reference here
		// TODO find workaround to render filebrowser from menu context
//		if (ImGui::BeginMenu("File"))
//		{
//			gui_menu_file(self);
//			ImGui::EndMenu();
//		}
		
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("New object", NULL))
			{ trj_eng_add_obj(self->eng, (s_trj_obj_init) { .desc = "object" }); }
			
//			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
//			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
//			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
//			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
//
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Env"))
		{
			if (ImGui::MenuItem(self->cmd->visible ? "Hide" : "Show", "CTRL+P")) { self->cmd->visible = !self->cmd->visible; }
			
			ImGui::Separator();
			
			if (ImGui::MenuItem("Reset", ""))
			{ gui_cmd_clearlog(self->cmd); gui_env_reset(self->env); }

//			ImGui::Separator();
			
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}
	
	return 0x00;
}

//----------------------------------------------------------------

#endif
