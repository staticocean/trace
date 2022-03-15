
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUITRAJ_STATIC__
#define __GUITRAJ_STATIC__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

static
void __guitraj_static_attr__ (void *__traj__, void *__attr__)
{
    ImGui::Text("[Plugin does not support view]");
}

static
void __guitraj_static_edit__ (void *__traj__)
{
    s_trctraj_static *traj = (s_trctraj_static*) __traj__;

//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	gui_objsel("##ref", traj->eng->obj_sz, traj->eng->obj_ls, &traj->ref);
//	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ellp  ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(-40);
//	gui_ellpsel("##ellp", traj->eng->ellp_offset, traj->eng->ellp_ls, &traj->ellp);
//	ImGui::SameLine(0.0, 0.0);
//	imgui_bool("##ellp_en", ImVec2(-1, 0), &traj->ellp_en);
//	if (traj->ellp == NULL) { traj->ellp_en = 0x00; }
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));

	if (traj->ellp_en == 0x00)
	{
		ImGui::Text("pos   "); ImGui::SameLine();
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
		imgui_vec("##pos", traj->pos, 1.0, NULL, NULL, "%.3f");
	}

	else
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("lat   "); ImGui::SameLine();
		ImGui::SameLine();
		imgui_lat("##lat", &traj->pos[0], ImGui::GetContentRegionAvail().x);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("lon   "); ImGui::SameLine();
		ImGui::SameLine();
		imgui_lon("##lon", &traj->pos[1], ImGui::GetContentRegionAvail().x);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("alt   "); ImGui::SameLine();
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragScalar("##alt", ImGuiDataType_Double, &traj->pos[2], 1.0, NULL, NULL, "%.3f");
	}

	ImGui::Text("rot   "); ImGui::SameLine();
	imgui_rot("##rot", traj->rot);
}

//------------------------------------------------------------------------------

static
void __guitraj_static_view__ (void *__traj__)
{
    ImGui::Text("[Plugin does not support view]");
}

//------------------------------------------------------------------------------

s_trctraj_intf __guitraj_static__ = {

        .desc       = "trctraj_static",

        .data_sz    = sizeof(s_trctraj_static),
        .attr_sz    = sizeof(s_trctraj_static_attr),

        .init       = __trctraj_static_init__,
        .free       = __trctraj_static_free__,
        .save       = __trctraj_static_save__,
        .load       = __trctraj_static_load__,
        .compile    = __trctraj_static_compile__,
        .pos        = __trctraj_static_pos__,
        .rot        = __trctraj_static_rot__,

        .gui_attr   = __guitraj_static_attr__,
        .gui_edit   = __guitraj_static_edit__,
        .gui_view   = __guitraj_static_view__,
};

//------------------------------------------------------------------------------

#endif /* __GUITRAJ_STATIC__ */
