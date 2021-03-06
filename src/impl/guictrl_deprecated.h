
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUICTRL__
#define __GUICTRL__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

inline void gui_ctrl_edit_egmsnpo(s_trcctrl *self)
{
//	ImGui::PushID(self);
//
//	s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) self->data;
//
//	// !!! UPDATE HASHES !!!
//	// if ref name was changed we must recalc hash
//	// to retain save/load and gui objsel functionality
//	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
//
//	ImGui::Text("desc  ");
//	ImGui::SameLine();
//	ImGui::Text(self->desc);
//
//	//	ImGui::Text("hash  ");
//	//	ImGui::SameLine();
//	//	imgui_hash("##hash", self->hash);
//	//
//	//	ImGui::Text("eng   ");
//	//	ImGui::SameLine();
//	//	ImGui::Text("%08X", (t_u32) ctrl->eng);
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	gui_objsel("##ref", ctrl->eng->obj_sz, ctrl->eng->obj_ls, &ctrl->ref);
//	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
//
//	ImGui::Dummy(ImVec2(0, 5));
//	ImGui::Separator();
//	ImGui::Dummy(ImVec2(0, 5));
//
//	ImGui::PopID();
}

//------------------------------------------------------------------------------

inline void gui_ctrl_view_egmsnpo(s_trcctrl *self)
{
//	ImGui::PushID(self);
//
//
//	ImGui::PopID();
}

//------------------------------------------------------------------------------

inline void gui_ctrl_edit_gm(s_trcctrl *self)
{
//    s_trcctrl_gm *ctrl = (s_trcctrl_gm*) self->data;
//
//    ImGui::Text("file  ");
//    ImGui::SameLine();
//    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//    ImGui::InputText("##file_name", ctrl->file_name, 256);
//
//    ImGui::Text("order ");
//    ImGui::SameLine();
//    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//    ImGui::InputScalar("##order", ImGuiDataType_U32, &ctrl->order,
//            NULL, NULL, "%u", ImGuiInputTextFlags_None);
}

//------------------------------------------------------------------------------

inline void gui_ctrl_view_gm(s_trcctrl *self)
{
//    s_trcctrl_gm *ctrl = (s_trcctrl_gm*) self->data;
//
//    static s_vl3d vl3d;
//    static s_vl3d_obj obj_ls[2*4096];
//
//    vl3d_init(&vl3d, (s_vl3d_attr) {
//			.obj_sz = sizeof(obj_ls) / sizeof(s_vl3d_obj),
//			.obj_ls = obj_ls
//	});
//
//    s_vl3d_line  line  = {.color = vl3d_col_legacy};
//    s_vl3d_point point = {
//            .color = vl3d_col_legacy,
//            .size = 2.0,
//    };
//
//    int res = 25;
//
//    t_f64 ecef[3];
//    t_f64 lla[3] = { 0.0, 0.0, 0.0 };
//    trcellp_ecef(&trcellp_wgs84, ecef, lla);
//
//    t_f64 g[3];
//    trcctrl_gm_calc(ctrl, g, ecef);
//    t_f64 gm = vld3v_norm(g);
//
//    printf("\ng: ");
//    vl3_vprint(g);
//    fflush(stdout);
//
//    for (int i = 1; i < res; ++i)
//    {
//        for (int j = 0; j < 2*res; ++j)
//        {
//            t_f64 ecef[3];
//
//            t_f64 lla[3] = {
//                    vld_pi * (t_f64) i / res - 0.5*vld_pi,
//                    vld_2pi * (t_f64) j / (2*res) - vld_pi,
//                    0.0
//            };
//
//            trcellp_ecef(&trcellp_wgs84, ecef, lla);
//
//            t_f64 g[3];
//            trcctrl_gm_calc(ctrl, g, ecef);
//            t_f64 gm = vld3v_norm(g);
//
//            t_u8 c = (gm / 10.0) * 255;
//            point.color = IM_COL32(c, c, c, 255);
//            vl3_vcopy(point.p0, ecef);
//
//            vl3d_add_point(&vl3d, point);
//        }
//    }

//    // pos
//    if (*data->data_sz > 10000)
//    {
//        for (int di = *data->data_sz / 10000, i = 0;
//             i < 10000 - 1; ++i)
//        {
//            vl_vcopy(line.p0, &data->data_ls[i * di].pos[0][0]);
//            vl_vcopy(line.p1, &data->data_ls[(i + 1) * di].pos[0][0]);
//
//            vl3d_add_line(&vl3d_eng, line);
//        }
//    } else
//    {
//        for (int i = 0; i < *data->data_sz - 1; ++i)
//        {
//            vl_vcopy(line.p0, &data->data_ls[i].pos[0][0]);
//            vl_vcopy(line.p1, &data->data_ls[i + 1].pos[0][0]);
//
//            vl3d_add_line(&vl3d_eng, line);
//        }
//    }
//
//    s_vl3d_trngl trngl = {.color = vl3d_col_l, .spec = 0x01 };
//
////		vl_vcopy(trngl.p0, &data->data_ls[0].pos[0][0]);
////		vl_vcopy(trngl.p1, &data->data_ls[*data->data_sz / 2].pos[0][0]);
////		vl_vcopy(trngl.p2, &data->data_ls[*data->data_sz - 1].pos[0][0]);
////		vl3d_add_trngl(&vl3d_eng, trngl);
//
//    // hpr
//    if (*data->data_sz > 20)
//    {
//        for (int di = *data->data_sz / 20, i = 0;
//             i < 20 - 1; ++i)
//        {
//            vl_vcopy(trngl.p0, &data->data_ls[i * di].pos[0][0]);
//            vl_vcopy(trngl.p1, &data->data_ls[i * di].pos[0][0]);
//            vl_vcopy(trngl.p2, &data->data_ls[i * di].pos[0][0]);
//
//            t_f64 rot[9];
//            vl_tnp(rot, &data->data_ls[i * di].rot[0][0]);
//            vl_mmul_s(rot, rot, 0.025 / view.scale);
//
//            // top middle
//
//            vl_vmul_s(&rot[0], &rot[0], 2.0);
//            vl_vsum(trngl.p0, trngl.p0, &rot[0]);
//
//            // left
//            vl_vsub(trngl.p1, trngl.p1, &rot[6]);
//
//            // right
//            vl_vsum(trngl.p2, trngl.p2, &rot[6]);
//
//            vl3d_add_trngl(&vl3d_eng, trngl);
//        }
//    } else
//    {
//        for (int i = 0; i < *data->data_sz - 1; ++i)
//        {
//            vl_vcopy(trngl.p0, &data->data_ls[i].pos[0][0]);
//            vl_vcopy(trngl.p1, &data->data_ls[i].pos[0][0]);
//            vl_vcopy(trngl.p2, &data->data_ls[i].pos[0][0]);
//
//            t_f64 rot[9];
//            vl_tnp(rot, &data->data_ls[i].rot[0][0]);
//            vl_mmul_s(rot, rot, 10.0 / view.scale);
//
//            // top middle
//            vl_vsum(trngl.p0, trngl.p0, &rot[0]);
//
//            // left
//            vl_vsub(trngl.p1, trngl.p1, &rot[6]);
//
//            // right
//            vl_vsum(trngl.p2, trngl.p2, &rot[6]);
//
//            vl3d_add_trngl(&vl3d_eng, trngl);
//        }
//    }

//	gui_vl3d(&vl3d);
//
//    free(obj_ls);
}

//------------------------------------------------------------------------------

#endif /* __GUICTRL__ */
