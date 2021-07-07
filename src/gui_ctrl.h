
#ifndef __GUI_CTRL__
#define __GUI_CTRL__

//----------------------------------------------------------------

#include <lib_internal/vl.h>

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_ctrl.h>
#include <lib/trj/trj_ctrl_.h>

//----------------------------------------------------------------

inline void gui_ctrl_edit(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::InputText("##name", self->name, 255);
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

inline void gui_ctrl_view(s_trj_ctrl *self)
{
	ImGui::Text("No view port available.");
	
	return;
}

//----------------------------------------------------------------

inline void gui_ctrl_edit_egms(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	s_trj_ctrl_egms *ctrl = (s_trj_ctrl_egms*) self->data;
	
	// !!! UPDATE HASHES !!!
	// if ref name was changed we must recalc hash
	// to retain save/load and gui objsel functionality
	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
//
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) ctrl->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", ctrl->eng->obj_count, ctrl->eng->obj_list, &ctrl->ref);
	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_ctrl_view_egms(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_ctrl_edit_gms(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	s_trj_ctrl_gms *ctrl = (s_trj_ctrl_gms*) self->data;
	
	// !!! UPDATE HASHES !!!
	// if ref name was changed we must recalc hash
	// to retain save/load and gui objsel functionality
	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
	
	ImGui::Text("desc  ");
	ImGui::SameLine();
	ImGui::Text(self->desc);
	
//	ImGui::Text("hash  ");
//	ImGui::SameLine();
//	imgui_hash("##hash", self->hash);
//
//	ImGui::Text("eng   ");
//	ImGui::SameLine();
//	ImGui::Text("%08X", (uint32_t) ctrl->eng);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("ref   ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	gui_objsel("##ref", ctrl->eng->obj_count, ctrl->eng->obj_list, &ctrl->ref);
	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_ctrl_view_gms(s_trj_ctrl *self)
{
	ImGui::PushID(self);
	
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_ctrl_edit_gm(s_trj_ctrl *self)
{
    s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) self->data;

    ImGui::Text("file  ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
    ImGui::InputText("##file_name", ctrl->file_name, 256);

    ImGui::Text("order ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
    ImGui::InputScalar("##order", ImGuiDataType_U32, &ctrl->order,
            NULL, NULL, "%u", ImGuiInputTextFlags_None);

    return;
}

//----------------------------------------------------------------

inline void gui_ctrl_view_gm(s_trj_ctrl *self)
{
    s_trj_ctrl_gm *ctrl = (s_trj_ctrl_gm*) self->data;

    static s_vl3d vl3d;
    static s_vl3d_obj obj_list[2*4096];
    
    vl3d_init(&vl3d, (s_vl3d_attr) {
			.obj_sz = sizeof(obj_list) / sizeof(s_vl3d_obj),
			.obj_ls = obj_list
	});

    s_vl3d_line  line  = {.color = vl3d_col_legacy};
    s_vl3d_point point = {
            .color = vl3d_col_legacy,
            .size = 2.0,
    };

    int res = 25;

    vlf_t ecef[3];
    vlf_t lla[3] = { 0.0, 0.0, 0.0 };
    trj_ellp_ecef(&trj_ellp_wgs84, ecef, lla);

    vlf_t g[3];
    trj_ctrl_gm_calc(ctrl, g, ecef);
    vlf_t gm = vl3_vnorm(g);

    printf("\ng: ");
    vl3_vprint(g);
    fflush(stdout);

    for (int i = 1; i < res; ++i)
    {
        for (int j = 0; j < 2*res; ++j)
        {
            vlf_t ecef[3];

            vlf_t lla[3] = {
                    vl_pi * (vlf_t) i / res - 0.5*vl_pi,
                    vl_2pi * (vlf_t) j / (2*res) - vl_pi,
                    0.0
            };

            trj_ellp_ecef(&trj_ellp_wgs84, ecef, lla);

            vlf_t g[3];
            trj_ctrl_gm_calc(ctrl, g, ecef);
            vlf_t gm = vl3_vnorm(g);

            uint8_t c = (gm / 10.0) * 255;
            point.color = IM_COL32(c, c, c, 255);
            vl3_vcopy(point.p0, ecef);

            vl3d_add_point(&vl3d, point);
        }
    }

//    // pos
//    if (*data->data_offset > 10000)
//    {
//        for (int di = *data->data_offset / 10000, i = 0;
//             i < 10000 - 1; ++i)
//        {
//            vl_vcopy(line.p0, &data->data_list[i * di].pos[0][0]);
//            vl_vcopy(line.p1, &data->data_list[(i + 1) * di].pos[0][0]);
//
//            vl3d_add_line(&vl3d_eng, line);
//        }
//    } else
//    {
//        for (int i = 0; i < *data->data_offset - 1; ++i)
//        {
//            vl_vcopy(line.p0, &data->data_list[i].pos[0][0]);
//            vl_vcopy(line.p1, &data->data_list[i + 1].pos[0][0]);
//
//            vl3d_add_line(&vl3d_eng, line);
//        }
//    }
//
//    s_vl3d_trngl trngl = {.color = vl3d_col_l, .spec = 0x01 };
//
////		vl_vcopy(trngl.p0, &data->data_list[0].pos[0][0]);
////		vl_vcopy(trngl.p1, &data->data_list[*data->data_offset / 2].pos[0][0]);
////		vl_vcopy(trngl.p2, &data->data_list[*data->data_offset - 1].pos[0][0]);
////		vl3d_add_trngl(&vl3d_eng, trngl);
//
//    // hpr
//    if (*data->data_offset > 20)
//    {
//        for (int di = *data->data_offset / 20, i = 0;
//             i < 20 - 1; ++i)
//        {
//            vl_vcopy(trngl.p0, &data->data_list[i * di].pos[0][0]);
//            vl_vcopy(trngl.p1, &data->data_list[i * di].pos[0][0]);
//            vl_vcopy(trngl.p2, &data->data_list[i * di].pos[0][0]);
//
//            vlf_t rot[9];
//            vl_tnp(rot, &data->data_list[i * di].rot[0][0]);
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
//        for (int i = 0; i < *data->data_offset - 1; ++i)
//        {
//            vl_vcopy(trngl.p0, &data->data_list[i].pos[0][0]);
//            vl_vcopy(trngl.p1, &data->data_list[i].pos[0][0]);
//            vl_vcopy(trngl.p2, &data->data_list[i].pos[0][0]);
//
//            vlf_t rot[9];
//            vl_tnp(rot, &data->data_list[i].rot[0][0]);
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

	gui_vl3d(&vl3d);

    free(obj_list);

    return;
}

//----------------------------------------------------------------

#endif /* __GUI_CTRL__ */
