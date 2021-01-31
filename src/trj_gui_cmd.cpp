
#include "trj_gui_cmd.h"

//------------------------------------------------------------------------------

void trj_gui_cmd_init(s_trj_gui_cmd *self, s_trj_gui_cmd_init attr)
{
	self->env = attr.env;
	self->visible = attr.visible;
	for (int i = 0; i < sizeof(self->title); ++i) {self->title[i] = attr.title[i]; }
	
	trj_gui_cmd_clearlog(self);
	memset(self->in_buff, 0, sizeof(self->in_buff));
	self->HistoryPos = -1;
	
	// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
	self->Commands.push_back("HELP");
	self->Commands.push_back("HISTORY");
	self->Commands.push_back("CLEAR");
	self->Commands.push_back("CLASSIFY");
	self->AutoScroll = true;
	self->ScrollToBottom = false;
	
	trj_gui_cmd_addlog(self, "Trajectory interactive c console");
}
	
// Portable helpers
static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char* Strdup(const char* s)                           { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

void trj_gui_cmd_copylog(s_trj_gui_cmd *self)
{
//	for (int i = 0; i < self->Items.Size; i++)
//	{
//		free(self->Items[i]);
//	}
//
//	self->Items.clear();
}

void trj_gui_cmd_clearlog(s_trj_gui_cmd *self)
{
	for (int i = 0; i < self->Items.Size; i++)
	{
		free(self->Items[i]);
	}
	
	self->Items.clear();
}
	
void trj_gui_cmd_addlog(s_trj_gui_cmd *self, const char* fmt, ...)
{
	// FIXME-OPT
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf)-1] = 0;
	va_end(args);
	self->Items.push_back(Strdup(buf));
}

int TextEditCallback(s_trj_gui_cmd *self, ImGuiInputTextCallbackData* data)
{
	//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
	switch (data->EventFlag)
	{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION
			
			// Locate beginning of current word
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}
			
			// Build a list of candidates
			ImVector<const char*> candidates;
			for (int i = 0; i < self->Commands.Size; i++)
				if (Strnicmp(self->Commands[i], word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(self->Commands[i]);
			
			if (candidates.Size == 0)
			{
				// No match
				trj_gui_cmd_addlog(self, "No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can..
				// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
				int match_len = (int)(word_end - word_start);
				for (;;)
				{
					int c = 0;
					bool all_candidates_matches = true;
					for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
						if (i == 0)
							c = toupper(candidates[i][match_len]);
						else if (c == 0 || c != toupper(candidates[i][match_len]))
							all_candidates_matches = false;
					if (!all_candidates_matches)
						break;
					match_len++;
				}
				
				if (match_len > 0)
				{
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}
				
				// List matches
				trj_gui_cmd_addlog(self, "Possible matches:\n");
				for (int i = 0; i < candidates.Size; i++)
					trj_gui_cmd_addlog(self, "- %s\n", candidates[i]);
			}
			
			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int prev_history_pos = self->HistoryPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (self->HistoryPos == -1)
					self->HistoryPos = self->History.Size - 1;
				else if (self->HistoryPos > 0)
					self->HistoryPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (self->HistoryPos != -1)
					if (++self->HistoryPos >= self->History.Size)
						self->HistoryPos = -1;
			}
			
			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != self->HistoryPos)
			{
				const char* history_str = (self->HistoryPos >= 0) ? self->History[self->HistoryPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
	}
	return 0;
}

// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	s_trj_gui_cmd* self = (s_trj_gui_cmd*) data->UserData;
	return TextEditCallback(self, data);
}

void trj_gui_cmd_render(s_trj_gui_cmd *self)
{
	// process env output stream
	{
		uint32_t buff_size = ftell(self->env->out_s);
		
		if (buff_size > 0)
		{
			rewind(self->env->out_s);
			
			fread(self->out_buff, buff_size, 1, self->env->out_s);
			self->out_buff[buff_size] = '\0';
			
			trj_gui_env_sreset(self->env);
			trj_gui_cmd_addlog(self, "# %s", self->out_buff);
		}
	}

//	ImGui::PushID(self);
	
	if (self->visible == false) { return; }
	
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 col_window_bg = style.Colors[ImGuiCol_WindowBg];
	ImVec4 col_window_bg_tp = style.Colors[ImGuiCol_WindowBg];
	col_window_bg_tp.w = 0.7;
	
	ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, col_window_bg_tp);
	
	ImGui::Begin(self->title, &self->visible, ImGuiWindowFlags_None);
	
//	ImGui::TextWrapped(
//			"This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
//			"implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
//	ImGui::TextWrapped("Enter 'HELP' for help.");
//
//	ImGui::SameLine();
//	bool copy_to_clipboard = ImGui::SmallButton("Copy");
//	ImGui::Separator();
	
	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &self->AutoScroll);
		ImGui::EndPopup();
	}
	
	// Options, Filter
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	self->Filter.Draw("", -1);
	ImGui::Separator();
	
	// Reserve enough left-over height for 1 separator + 1 input text
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Copy ")) trj_gui_cmd_copylog(self);
		if (ImGui::Selectable("Clear")) trj_gui_cmd_clearlog(self);
		
		ImGui::EndPopup();
	}
	
	// Display every line as a separate entry so we can change their color or add custom widgets.
	// If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
	// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
	// to only process visible items. The clipper will automatically measure the height of your first item and then
	// "seek" to display only items in the visible area.
	// To use the clipper we can replace your standard loop:
	//      for (int i = 0; i < Items.Size; i++)
	//   With:
	//      ImGuiListClipper clipper;
	//      clipper.Begin(Items.Size);
	//      while (clipper.Step())
	//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
	// - That your items are evenly spaced (same height)
	// - That you have cheap random access to your elements (you can access them given their index,
	//   without processing all the ones before)
	// You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
	// We would need random-access on the post-filtered list.
	// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
	// or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
	// and appending newly elements as they are inserted. This is left as a task to the user until we can manage
	// to improve this example code!
	// If your items are of variable height:
	// - Split them into same height items would be simpler and facilitate random-seeking into your list.
	// - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
//	if (copy_to_clipboard)
//		ImGui::LogToClipboard();
	for (int i = 0; i < self->Items.Size; i++)
	{
		const char* item = self->Items[i];
		if (!self->Filter.PassFilter(item))
			continue;
		
		ImGuiStyle& style = ImGui::GetStyle();
		
		// Normally you would store more information in your item than just a string.
		// (e.g. make Items[] an array of structure, store color/type etc.)
		ImVec4 color;
		bool has_color = false;
		if (strstr(item, "[error]"))          { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
		else if (strncmp(item, "# ", 2) == 0) { color = style.Colors[ImGuiCol_TextDisabled]; has_color = true; }
		if (has_color)
			ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::TextUnformatted(item);
		if (has_color)
			ImGui::PopStyleColor();
	}
//	if (copy_to_clipboard)
//		ImGui::LogFinish();
	
	if (self->ScrollToBottom || (self->AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		ImGui::SetScrollHereY(1.0f);
	self->ScrollToBottom = false;
	
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();
	
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	
	// Command-line
	bool reclaim_focus = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("##input", self->in_buff, IM_ARRAYSIZE(self->in_buff), input_text_flags, &TextEditCallbackStub, (void*) self))
	{
		Strtrim(self->in_buff);
		
		if (self->in_buff[0])
		{
			trj_gui_cmd_exec(self);
			trj_gui_cmd_addlog(self, "> %s", self->in_buff);
		}
		
		strcpy(self->in_buff, "");
		
		reclaim_focus = true;
	}
	
	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	
	ImGui::End();
	
	ImGui::PopStyleColor();
}

//------------------------------------------------------------------------------

void trj_gui_cmd_exec(s_trj_gui_cmd *self)
{
	Picoc *env = &self->env->env;
	
	if (PicocPlatformSetExitPoint(env))
	{
	
	}
	
	else
	{
		PicocParse(env, "env", self->in_buff, strlen(self->in_buff),
				true, false, false, true);
	}
}

//------------------------------------------------------------------------------
