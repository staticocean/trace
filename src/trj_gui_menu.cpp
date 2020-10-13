
#include "trj_gui_menu.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_menu_main(s_trj_gui_menu *self)
{
	if (ImGui::BeginMainMenuBar())
	{
		self->height = ImGui::GetWindowHeight();
		
		if (ImGui::BeginMenu("File"))
		{
			trj_gui_menu_file(self);
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			
			ImGui::Separator();
			
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Env"))
		{
			if (ImGui::MenuItem("Reset", "")) { trj_gui_env_reset(self->env); }
			
//			ImGui::Separator();
			
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

uint8_t trj_gui_menu_file(s_trj_gui_menu *self)
{
	ImGui::MenuItem("(demo menu)", NULL, false, false);
	
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
	
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
	
			if (ImGui::BeginMenu("Recurse.."))
			{
				trj_gui_menu_file(self);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}
	
	ImGui::Separator();
	
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
	
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
	
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
	
		ImGui::EndChild();
	
		static float f = 0.5f;
		static int n = 0;
	
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}
	
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
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		static bool b = true;
	
		ImGui::Checkbox("SomeOption", &b);
		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
	
	return 0x00;
}

//------------------------------------------------------------------------------
