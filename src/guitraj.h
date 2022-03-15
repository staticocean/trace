
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUITRAJ__
#define __GUITRAJ__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

extern
void guitraj_edit (s_trctraj *traj)
{
	ImGui::PushID(traj);

	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputText("##name", traj->name, 32);

	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));

    ImGui::Text("desc  ");
    ImGui::SameLine();
    ImGui::Text(traj->intf->desc);

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

    if (traj->intf->gui_edit == NULL)
    {
        ImGui::Text("[Plugin edit is not supported]");
    }

    else
    {
        traj->intf->gui_edit(traj);
    }

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

    ImGui::PopID();
}

//------------------------------------------------------------------------------

extern
void guitraj_view (s_trctraj *traj)
{
	ImGui::PushID(traj);

    if (traj->intf->gui_view == NULL)
    {
        ImGui::Text("[Plugin view is not supported]");
    }

    else
    {
        traj->intf->gui_view(traj);
    }

    ImGui::PopID();
}


//------------------------------------------------------------------------------

extern
void guitraj_attr (s_trctraj *traj, s_trctraj_attr *attr)
{
    ImGui::PushID(traj);

    if (traj->intf->gui_attr == NULL)
    {
        ImGui::Text("[Plugin attr is not supported]");
    }

    else
    {
        traj->intf->gui_attr(traj, attr);
    }

    ImGui::PopID();
}

//------------------------------------------------------------------------------

#endif /* __GUITRAJ__ */
