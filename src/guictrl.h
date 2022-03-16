
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUICTRL__
#define __GUICTRL__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

extern
void guictrl_edit (s_trcctrl *ctrl)
{
	ImGui::PushID(ctrl);
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputText("##name", ctrl->name, sizeof(ctrl->name));

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

    if (ctrl->intf->gui_edit != NULL)
    {
        ctrl->intf->gui_edit(ctrl);
    }

    else
    {
        ImGui::Text("No edit available.");
    }

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
}

extern
void guictrl_view (s_trcctrl *ctrl)
{
    ImGui::PushID(ctrl);

    if (ctrl->intf->gui_view != NULL)
    {
        ctrl->intf->gui_view(ctrl);
    }

    else
    {
        ImGui::Text("No view port available.");
    }

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

    ImGui::PopID();
}

//------------------------------------------------------------------------------

#endif /* __GUICTRL__ */
