
#ifndef __IMGUI_CUSTOM__
#define __IMGUI_CUSTOM__

//----------------------------------------------------------------

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

//----------------------------------------------------------------

namespace ImGui
{
	bool BufferingBar(const char *label, float value, const ImVec2 &size_arg, const ImU32 &bg_col, const ImU32 &fg_col);
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color);
}

//----------------------------------------------------------------

#endif /* __IMGUI_CUSTOM__ */
