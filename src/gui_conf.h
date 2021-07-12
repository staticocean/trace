
#ifndef __GUI_CONF__
#define __GUI_CONF__

//----------------------------------------------------------------

#include <lib_internal/vl.h>
#include <lib_internal/vl3d.h>

#include <lib/imgui/imgui.h>

//----------------------------------------------------------------

typedef struct gui_conf
{
    bool isopen;

    s_trj_eng  *eng;

} 	s_gui_conf;

typedef struct gui_conf_attr
{
    s_trj_eng  *eng;

} 	s_gui_conf_attr;

//----------------------------------------------------------------

inline void gui_conf_init(s_gui_conf *conf, s_gui_conf_attr attr)
{
    conf->isopen = false;

    conf->eng = attr.eng;

    return;
}

inline void gui_conf_view(s_gui_conf *conf)
{
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Always);

    if (conf->isopen)
    {
        ImGui::Begin("SETTINGS", &conf->isopen,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        if (ImGui::BeginChild("##scroll_view"));
        {
            static uint32_t default_proc_euler = vl_crc32("default_proc_euler");

            if (conf->eng->proc.hash == default_proc_euler)
            {
                s_trj_proc_euler *proc = (s_trj_proc_euler*) conf->eng->proc.data;

                ImGui::Text("proc_rotation_tolerance");
                ImGui::SameLine();
                ImGui::DragScalar("##proc_rotation_tolerance", ImGuiDataType_Double, &proc->rot_tol, 1E-6);

                ImGui::Text("proc_rotation_variation");
                ImGui::SameLine();
                ImGui::DragScalar("##proc_rotation_variation", ImGuiDataType_Double, &proc->rot_var, 1E-6);

                ImGui::Text("proc_rotation_step     ");
                ImGui::SameLine();
                ImGui::DragScalar("##proc_rotation_step", ImGuiDataType_Double, &proc->rot_step, 1E-6);

                ImGui::Text("proc_position_tolerance");
                ImGui::SameLine();
                ImGui::DragScalar("##proc_position_tolerance", ImGuiDataType_Double, &proc->pos_tol, 1E-6);

                ImGui::Text("proc_position_variation");
                ImGui::SameLine();
                ImGui::DragScalar("##proc_position_variation", ImGuiDataType_Double, &proc->pos_var, 1E-6);

                ImGui::Text("proc_position_step     ");
                ImGui::SameLine();
                ImGui::DragScalar("##proc_position_step", ImGuiDataType_Double, &proc->pos_step, 1E-6);
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    return;
}

inline void gui_conf_show(s_gui_conf *conf)
{
    conf->isopen = true;

    return;
}

inline void gui_conf_hide(s_gui_conf *conf)
{
    conf->isopen = false;

    return;
}

//----------------------------------------------------------------

#endif /* __GUI_CONF__ */