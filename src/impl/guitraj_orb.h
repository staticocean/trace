
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUITRAJ_ORB__
#define __GUITRAJ_ORB__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

static
void __guitraj_orb_attr__ (void *__traj__, void *__attr__)
{
    ImGui::Text("[Plugin does not support view]");
}

//------------------------------------------------------------------------------

static
void __guitraj_orb_edit__ (void *__traj__)
{
	s_trctraj_orb *traj = (s_trctraj_orb*) __traj__;

//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(-60);
//	gui_objsel("##ref", traj->eng->obj_sz, traj->eng->obj_ls, &traj->ref);
//	ImGui::SameLine(0.0, 0.0);
//	imgui_switch("##sync_en", (char*[2]) { "SYNC", "FLOAT" }, ImVec2(-1, 0), &traj->sync_en);
//	if (traj->ref != NULL) { traj->ref_hash = traj->ref->hash; }
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));

	ImGui::Text("radius");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[m]"); }
	ImGui::SameLine();
	t_f64 radius_min = 0.0;
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragScalar("##radius", ImGuiDataType_Double, &traj->radius, 1.0, &radius_min, NULL, "%.3f");

	ImGui::Text("rate  ");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[deg/s]"); }
	ImGui::SameLine();
	t_f64 rate_deg = vld_deg(traj->rate);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragScalar("##rate", ImGuiDataType_Double, &rate_deg, 0.1, NULL, NULL, "%.9f");
	traj->rate = vld_rad(rate_deg);

	ImGui::Text("tilt  "); ImGui::SameLine();
	imgui_rot("##tilt", traj->tilt);

	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));

	ImGui::Text("s_rate");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("[deg/s]"); }
	ImGui::SameLine();
	t_f64 s_rate_deg = vld_deg(traj->s_rate);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::DragScalar("##s_rate", ImGuiDataType_Double, &s_rate_deg, 0.1, NULL, NULL, "%.9f");
	traj->s_rate = vld_rad(s_rate_deg);

	ImGui::Text("s_tilt"); ImGui::SameLine();
	imgui_rot("##s_tilt", traj->s_tilt);
}

//------------------------------------------------------------------------------

static
void __guitraj_orb_view__ (void *__traj__)
{
//	s_trctraj_orb *traj = (s_trctraj_orb*) __traj__;
//
//	static s_vl3d 		vl3d;
//	static s_vl3d_obj 	obj_ls[4096*2];
//
//	vl3d_init(&vl3d, (s_vl3d_attr) {
//		.obj_sz = sizeof(obj_ls) / sizeof(s_vl3d_obj),
//		.obj_ls = obj_ls,
//	});
//
//	if (fabs(traj->rate) > 1E-9)
//	{
//		t_f64 time = 0.0;
//		t_f64 time_limit = vld_2pi / fabs(traj->rate);
//		t_f64 time_step = time_limit / 1000;
//		int time_iter = time_limit / time_step;
//
//		t_f64 p0[3];
//		t_f64 p1[3];
//		t_f64 rot[9];
//
//		trctraj_orb_pos(traj, time, p0);
//		trctraj_orb_pos(traj, time, p1);
//
//		for (int i = 0; i < time_iter; ++i)
//		{
//			time += time_step;
//
//			vl3_vcopy(p0, p1);
//			trctraj_orb_pos(traj, time, p1);
//
//			vl3d_add_line(&vl3d, (s_vl3d_line) {
//					.color = vl3d_col_legacy,
//					.p0 = { p0[0], p0[1], p0[2] },
//					.p1 = { p1[0], p1[1], p1[2] },
//			});
//		}
//	}
//
//	static s_vl3d_view view = {
//			.scale = 1.0,
//			.pos = { 0.0, 0.0, 0.0 },
//	};
//
//	gui_vl3d(&vl3d);
}

//------------------------------------------------------------------------------

s_trctraj_intf __guitraj_orb__ = {

        .desc       = "trctraj_orb",

        .data_sz    = sizeof(s_trctraj_orb),
        .attr_sz    = sizeof(s_trctraj_orb_attr),

        .init       = __trctraj_orb_init__,
        .free       = __trctraj_orb_free__,
        .save       = __trctraj_orb_save__,
        .load       = __trctraj_orb_load__,
        .compile    = __trctraj_orb_compile__,
        .pos        = __trctraj_orb_pos__,
        .rot        = __trctraj_orb_rot__,

        .gui_attr   = __guitraj_orb_attr__,
        .gui_edit   = __guitraj_orb_edit__,
        .gui_view   = __guitraj_orb_view__,
};

//------------------------------------------------------------------------------

#endif /* __GUITRAJ_ORB__ */
