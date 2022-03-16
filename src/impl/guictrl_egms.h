
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUICTRL_EGMS__
#define __GUICTRL_EGMS__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

//extern
//void __guictrl_egms_attr__(void *__ctrl__, void *__attr__)
//{
//    return;
//}

extern
void __guictrl_egms_edit__ (void *__ctrl__)
{
	s_trcctrl_egms *ctrl = (s_trcctrl_egms*) __ctrl__;

//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	gui_objsel("##ref", ctrl->eng->obj_sz, ctrl->eng->obj_ls, &ctrl->ref);
//	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
}

//extern
//void __guictrl_egms_view__(void *__ctrl__)
//{
//    return;
//}

//------------------------------------------------------------------------------

s_trcctrl_intf __guictrl_egms__ = {

        .desc       = "trcctrl_egms",

        .data_sz    = sizeof(s_trcctrl_egms),
        .attr_sz    = sizeof(s_trcctrl_egms_attr),

        .init       = __trcctrl_egms_init__,
        .free       = __trcctrl_egms_free__,
        .save       = __trcctrl_egms_save__,
        .load       = __trcctrl_egms_load__,
        .reset      = __trcctrl_egms_reset__,
        .update     = __trcctrl_egms_update__,

//        .gui_attr   = __guictrl_egms_attr__,
        .gui_edit   = __guictrl_egms_edit__,
//        .gui_view   = __guictrl_egms_view__,
};

//------------------------------------------------------------------------------

#endif /* __GUICTRL_EGMS__ */
