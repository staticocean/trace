
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_CONF__
#define __GUI_CONF__

//------------------------------------------------------------------------------

#include <sftlstd/vld3.h>
#include <sftlstd/vl3d.h>
#include <sftlstd/vl3d_imgui.h>

#include <imgui/imgui.h>

//------------------------------------------------------------------------------

typedef struct gui_conf
{
    bool isopen;

    s_trceng  *eng;

} 	s_gui_conf;

typedef struct gui_conf_attr
{
    s_trceng  *eng;

} 	s_gui_conf_attr;

//------------------------------------------------------------------------------

inline void gui_conf_init(s_gui_conf *conf, s_gui_conf_attr attr)
{
    conf->isopen = false;

    conf->eng = attr.eng;

    return;
}

//------------------------------------------------------------------------------

void gui_conf_view (s_gui_conf *conf)
{
//    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
//    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
//    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);
//
//    if (conf->isopen)
//    {
//        ImGui::Begin("SETTINGS", &conf->isopen,
//                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
//
//        if (ImGui::BeginChild("##scroll_view"))
//        {
//            static t_u32 default_proc_euler = crc32_iso_str("default_proc_euler");
//
//            if (conf->eng->proc.hash == default_proc_euler)
//            {
//                s_trcproc_euler *proc = (s_trcproc_euler*) conf->eng->proc.data;
//
//                ImGui::Text("proc_rotation_tolerance");
//                ImGui::SameLine();
//                ImGui::DragScalar("##proc_rotation_tolerance", ImGuiDataType_Double, &proc->rot_tol, 1E-6, NULL, NULL, "%.9lf");
//
//                ImGui::Text("proc_rotation_variation");
//                ImGui::SameLine();
//                ImGui::DragScalar("##proc_rotation_variation", ImGuiDataType_Double, &proc->rot_var, 1E-6, NULL, NULL, "%.9lf");
//
//                ImGui::Text("proc_rotation_step     ");
//                ImGui::SameLine();
//                ImGui::DragScalar("##proc_rotation_step", ImGuiDataType_Double, &proc->rot_step, 1E-6, NULL, NULL, "%.9lf");
//
//                ImGui::Text("proc_position_tolerance");
//                ImGui::SameLine();
//                ImGui::DragScalar("##proc_position_tolerance", ImGuiDataType_Double, &proc->pos_tol, 1E-6, NULL, NULL, "%.9lf");
//
//                ImGui::Text("proc_position_variation");
//                ImGui::SameLine();
//                ImGui::DragScalar("##proc_position_variation", ImGuiDataType_Double, &proc->pos_var, 1E-6, NULL, NULL, "%.9lf");
//
//                ImGui::Text("proc_position_step     ");
//                ImGui::SameLine();
//                ImGui::DragScalar("##proc_position_step", ImGuiDataType_Double, &proc->pos_step, 1E-6, NULL, NULL, "%.9lf");
//            }
//
//            ImGui::EndChild();
//        }
//
//        ImGui::End();
//    }
}

//------------------------------------------------------------------------------

void gui_conf_show (s_gui_conf *conf)
{
    conf->isopen = true;
}

//------------------------------------------------------------------------------

void gui_conf_hide (s_gui_conf *conf)
{
    conf->isopen = false;
}

//------------------------------------------------------------------------------

#endif /* __GUI_CONF__ */
