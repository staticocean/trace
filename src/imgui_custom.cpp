
#include "imgui_custom.h"

//------------------------------------------------------------------------------
namespace ImGui {
	
	bool BufferingBar(const char* label, float value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;
		
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;
		
		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;
		
		// Render
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x, bb.Max.y), bg_col);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x*value, bb.Max.y), fg_col);
		
		return true;
	}
	
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;
		
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
		
		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;
		
		// Render
		window->DrawList->PathClear();
		
		int num_segments = 30;
		int start = ImAbs(ImSin(g.Time*1.8f)*(num_segments-5));
		
		const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;
		
		const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);
		
		for (int i = 0; i < num_segments; i++) {
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
												centre.y + ImSin(a+g.Time*8) * radius));
		}
		
		window->DrawList->PathStroke(color, false, thickness);
		
		return true;
	}
	
//	/// Get custom user iu state at given position of id stack. If state does not exist then state object will be created.
///// Using different type at the same id stack position will return new object of that type. Arguments passed to this
///// function will be passed to constructor of type T.
//	template<typename T, typename... Args>
//	T* GetUIState(Args... args)
//	{
//		ImGui::PushID(typeid(T).name());
//		T* state = (T*)GetUIStateP();
//		if (state == nullptr)
//		{
//			state = new T(args...);
//			SetUIStateP(state, [](void* s) { delete (T*)s; });
//		}
//		ImGui::PopID();
//		return state;
//	}
///// Expire custom ui state at given position if id stack, created with GetUIState<T>. It will be freed immediately.
//	template<typename T>
//	void ExpireUIState()
//	{
//		ImGui::PushID(typeid(T).name());
//		ExpireUIStateP();
//		ImGui::PopID();
//	}
//
//	static unordered_map<ImGuiID, UIStateWrapper> uiState_;
//	static int uiStateLastGcFrame_ = 0;
//
//	void SetUIStateP(void* state, void(*deleter)(void*))
//	{
//		auto id = ImGui::GetCurrentWindow()->IDStack.back();
//		uiState_[id].Set(state, deleter);
//	}
//
//	void* GetUIStateP()
//	{
//		void* result = nullptr;
//		auto id = ImGui::GetCurrentWindow()->IDStack.back();
//		auto it = uiState_.find(id);
//		if (it != uiState_.end())
//			result = it->second.Get();
//
//		int currentFrame = ImGui::GetCurrentContext()->FrameCount;
//		if (uiStateLastGcFrame_ != currentFrame)
//		{
//			uiStateLastGcFrame_ = currentFrame;
//			for (auto jt = uiState_.begin(); jt != uiState_.end();)
//			{
//				if (jt->second.IsExpired())
//				{
//					jt->second.Unset();
//					jt = uiState_.erase(jt);
//				}
//				else
//					++jt;
//			}
//		}
//
//		return result;
//	}
//
//	void ExpireUIStateP()
//	{
//		auto it = uiState_.find(ImGui::GetCurrentWindow()->IDStack.back());
//		if (it != uiState_.end())
//		{
//			it->second.Unset();
//			uiState_.erase(it);
//		}
//	}
}

//------------------------------------------------------------------------------
