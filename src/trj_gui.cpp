
#include "trj_gui.h"

//------------------------------------------------------------------------------

uint8_t trj_gui_init(s_trj_gui *self, s_trj_gui_init_attr attr)
{
	self->gui_eng.sel_item = NULL;
	self->gui_eng.sel_type = 0x00;
	self->gui_eng.obj_hide = self->st_gui_eng_objhide;
	
	for (int i = 0; i < 255; ++i)
	{
		self->gui_eng.obj_hide[i] = 0x00;
	}
	
	trj_eng_init(&self->eng, (s_trj_eng_init_attr) { .st_objects = self->st_eng_obj });
	
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 00" });
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 01" });
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 02" });
	trj_eng_add(&self->eng, (s_trj_obj) { .name = "test object 03" });
	
	ImGui::StyleColorsDark();
	
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
					 | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
	
	bool show_demo_window = true;
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	
	trj_gui_menu_main(&self->gui_menu);
	
	// Object list
	ImGui::SetNextWindowPos((ImVec2) { 0, (float) self->gui_menu.height });
	ImGui::SetNextWindowSize((ImVec2) { 200, 0 });
	ImGui::Begin("eng_objlist", NULL, static_flags);
	trj_gui_eng_objlist(&self->gui_eng, &self->eng);
	ImGui::End();
	
	// Object edit
	ImGui::SetNextWindowPos((ImVec2) { 200, (float) self->gui_menu.height });
	ImGui::SetNextWindowSize((ImVec2) { 200, 0 });
	ImGui::Begin("item_edit", NULL, static_flags);
	
	if (self->gui_eng.sel_item != NULL)
	{
		switch (self->gui_eng.sel_type)
		{
			case 0x00:
			{
				trj_gui_obj_edit((s_trj_obj*) self->gui_eng.sel_item);
				break;
			}
			
			default: break;
		}
	}
	
	ImGui::End();
	
	return 0x00;
}

//------------------------------------------------------------------------------
//
///* this function is called by the generated code */
//int fadd(int a, int b)
//{
//	return a + b;
//}
//
//int32_t tcc_test(void)
//{
//	/* this strinc is referenced by the generated code */
//	static const char hello[] = "Hello World!";
//
//	static const char* program = \
//	"#include <tcclib.h>\n" /* include the "Simple libc header for TCC" */
//	"extern int add(int a, int b);\n"
//	"extern int foo(int a);\n"
//	"#ifdef _WIN32\n" /* dynamically linked data needs 'dllimport' */
//	" __attribute__((dllimport))\n"
//	"#endif\n"
//	"extern const char hello[];\n"
//	"int fib(int n)\n"
//	"{\n"
//	"    if (n <= 2)\n"
//	"        return 1;\n"
//	"    else\n"
//	"        return fib(n-1) + fib(n-2);\n"
//	"}\n"
//	"\n"
//	"int foo(int n)\n"
//	"{\n"
//	"    return 32*32;\n"
//	"}\n";
//
//	int i;
//
//	TCCState *s = tcc_new();
//
//	if (!s) {
//		fprintf(stderr, "Could not create tcc state\n");
//		exit(1);
//	}
//
//	/* if tcclib.h and libtcc1.a are not installed, where can we find them */
//
//	#ifdef __EMSCRIPTEN__
//
//	tcc_set_lib_path(s, "/lib");
//	tcc_add_include_path(s, "/lib");
//	tcc_add_include_path(s, "/include");
//	tcc_add_include_path(s, "/include/winapi");
//	tcc_add_include_path(s, "/include/tcc");
//	tcc_add_include_path(s, "/include/sys");
//	tcc_add_include_path(s, "/include/sec_api");
//	tcc_add_sysinclude_path(s, "/lib");
//	tcc_add_sysinclude_path(s, "/include");
//	tcc_add_sysinclude_path(s, "/include/winapi");
//	tcc_add_sysinclude_path(s, "/include/tcc");
//	tcc_add_sysinclude_path(s, "/include/sys");
//	tcc_add_sysinclude_path(s, "/include/sec_api");
//
//	#else
//
//	tcc_set_lib_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\lib");
//	tcc_add_include_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\lib");
//	tcc_add_include_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include");
//	tcc_add_include_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\winapi");
//	tcc_add_include_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\tcc");
//	tcc_add_include_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\sys");
//	tcc_add_include_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\sec_api");
//	tcc_add_sysinclude_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\lib");
//	tcc_add_sysinclude_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include");
//	tcc_add_sysinclude_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\winapi");
//	tcc_add_sysinclude_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\tcc");
//	tcc_add_sysinclude_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\sys");
//	tcc_add_sysinclude_path(s, "C:\\Users\\cel\\dev\\git\\bitbucket\\trajectory\\src\\cmake-build-debug\\include\\sec_api");
//
//	#endif
//
//	/* MUST BE CALLED before any compilation */
//	tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
//
//	tcc_set_options(s, "-nostdlib");
//
//	if (tcc_compile_string(s, program) > 0) {
//		printf("Compilation error !\n");
//		return 2;
//	}
//
//	/* as a test, we add symbols that the compiled program can use.
//	   You may also open a dll with tcc_add_dll() and use symbols from that */
////	tcc_add_symbol(s, "add", (const void *) fadd);
////	tcc_add_symbol(s, "hello", (const void *) hello);
//
//	/* relocate the code */
//	if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) return 1;
//
//	/* get entry symbol */
//
//	int32_t (*func)(int32_t) = (int32_t (*)(int32_t)) tcc_get_symbol(s, "foo");
//	if (tcc_get_symbol(s, "foo") == 0x00) return 2;
//
//	/* run the code */
//	int32_t res = func(32);
//
//	/* delete the state */
//	tcc_delete(s);
//
//	return res;
//}
