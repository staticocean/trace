
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_CLIP__
#define __GUI_CLIP__

//------------------------------------------------------------------------------

#include <clip/clip.h>
#include <imgui/imgui.h>

#include <sftlstd/vld3.h>
#include <sftlstd/vl3d.h>
#include <sftlstd/vl3d_imgui.h>

#include <sftltrc/sftltrc.h>

//------------------------------------------------------------------------------

typedef struct gui_clip
{
    clip::format    format_traj;

}   s_gui_clip;

typedef struct gui_clip_attr
{


}   s_gui_clip_attr;

//------------------------------------------------------------------------------

void gui_clip_init (s_gui_clip *clip, s_gui_clip_attr attr)
{
    clip->format_traj = clip::register_format("com.trajectory.traj");
    // Do not clear the clipboard TF ?
//    clip->lock.clear();
}

//------------------------------------------------------------------------------

void gui_clip_set_traj (s_gui_clip *clip, s_trceng *eng, s_trctraj *traj)
{
    t_u8 __traj_data__[256 * 1024];
    t_u8 *traj_data = __traj_data__;

    trctraj_save(traj, &eng->intf_spl, &traj_data);

    t_u32 traj_data_size = (t_u32) (traj_data - __traj_data__);

    clip::lock lock;
    lock.set_data(clip->format_traj, (const char*) __traj_data__, traj_data_size);
}

//------------------------------------------------------------------------------

void gui_clip_get_traj (s_gui_clip *clip, s_trceng *eng, s_trctraj *traj)
{
    t_u8 __traj_data__[256 * 1024];
    t_u8 *traj_data = __traj_data__;
    clip::lock lock;
    t_u32 traj_data_size = lock.get_data_length(clip->format_traj);

    if (traj_data_size > 0x00)
    {
        lock.get_data(clip->format_traj, (char*) __traj_data__, traj_data_size);

        trctraj_free(traj);
        trctraj_load(traj, &eng->intf_spl, &traj_data);
    }
}

//------------------------------------------------------------------------------

#endif /* __GUI_CLIP__ */
