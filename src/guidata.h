
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUIDATA__
#define __GUIDATA__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

extern
void guidata_edit (s_trcdata *data)
{
	ImGui::PushID(data);
	
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputText("##name", data->name, sizeof(data->name));
	
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));

    if (data->intf->gui_edit != NULL)
    {
        data->intf->gui_edit(data);
    }

    else
    {
        // Implement not supported message
    }

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));
	
	ImGui::PopID();
}

//------------------------------------------------------------------------------

extern
void guidata_view (s_trcdata *data)
{
    ImGui::PushID(data);

    if (data->intf->gui_view != NULL)
    {
        data->intf->gui_view(data);
    }

    else
    {
        // Implement not supported message
    }

    ImGui::PopID();
}

//------------------------------------------------------------------------------

#endif /* __GUIDATA__ */
