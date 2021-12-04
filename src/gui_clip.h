
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#ifndef __GUI_CLIP__
#define __GUI_CLIP__

//------------------------------------------------------------------------------

#include <sftlstd/vl.h>
#include <libcommon/vl3d.h>

#include <libgui/imgui/imgui.h>
#include <sftltrc/trcobj.h>

#include <libgui/clip/clip.h>

//------------------------------------------------------------------------------

typedef struct gui_clip
{
    clip::format    format_traj;

}   s_gui_clip;

typedef struct gui_clip_attr
{


}   s_gui_clip_attr;

void gui_clip_init (s_gui_clip *clip, s_gui_clip_attr attr)
{
    clip->format_traj = clip::register_format("com.trajectory.traj");
    // Do not clear the clipboard TF ?
//    clip->lock.clear();

    return;
}

void gui_clip_set_traj (s_gui_clip *clip, s_trceng *eng, s_trctraj *traj)
{
    u8_t __traj_data__[256 * 1024];
    u8_t *traj_data = __traj_data__;

    trctraj_save(traj, eng, &traj_data);

    u32_t traj_data_size = (u32_t) (traj_data - __traj_data__);

    clip::lock lock;
    lock.set_data(clip->format_traj, (const char*) __traj_data__, traj_data_size);

    return;
}

void gui_clip_get_traj (s_gui_clip *clip, s_trceng *eng, s_trctraj *traj)
{
    u8_t __traj_data__[256 * 1024];
    u8_t *traj_data = __traj_data__;
    clip::lock lock;
    u32_t traj_data_size = lock.get_data_length(clip->format_traj);

    if (traj_data_size > 0x00)
    {
        lock.get_data(clip->format_traj, (char*) __traj_data__, traj_data_size);
        traj->free(&traj->data);
        trctraj_load(traj, eng, &traj_data);
    }

    return;
}

//------------------------------------------------------------------------------

#endif /* __GUI_CLIP__ */
