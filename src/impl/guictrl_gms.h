
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __GUICTRL_GMS__
#define __GUICTRL_GMS__

//------------------------------------------------------------------------------

#include <sftlgui/sftlgui.h>
#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

//static
//void __guictrl_gms_attr__ (void *__ctrl__, void *__attr__)
//{
//    return;
//}

static
void __guictrl_gms_edit__ (void *__ctrl__)
{
	s_trcctrl_gms *ctrl = (s_trcctrl_gms*) __ctrl__;
//
//	ImGui::AlignTextToFramePadding();
//	ImGui::Text("ref   ");
//	ImGui::SameLine();
//	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
//	gui_objsel("##ref", ctrl->eng->obj_sz, ctrl->eng->obj_ls, &ctrl->ref);
//	if (ctrl->ref != NULL) { ctrl->ref_hash = ctrl->ref->hash; }
}

//static
//void __guictrl_gms_view__ (void *__ctrl__)
//{
//    s_trcctrl_gms *ctrl = (s_trcctrl_gms*) __ctrl__;
//}

//------------------------------------------------------------------------------

s_trcctrl_intf __guictrl_gms__ = {

        .desc       = "trcctrl_gms",

        .data_sz    = sizeof(s_trcctrl_gms),
        .attr_sz    = sizeof(s_trcctrl_gms_attr),

        .init       = __trcctrl_gms_init__,
        .free       = __trcctrl_gms_free__,
        .save       = __trcctrl_gms_save__,
        .load       = __trcctrl_gms_load__,
        .reset      = __trcctrl_gms_reset__,
        .update     = __trcctrl_gms_update__,

//        .gui_attr   = __guictrl_gms_attr__,
        .gui_edit   = __guictrl_gms_edit__,
//        .gui_view   = __guictrl_gms_view__,
};

//------------------------------------------------------------------------------

#endif /* __GUICTRL_GMS__ */
