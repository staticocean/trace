
#ifndef __GUI_CLIP__
#define __GUI_CLIP__

//----------------------------------------------------------------

#include <lib_internal/vl.h>
#include <lib_internal/vl3d.h>

#include <lib/imgui/imgui.h>
#include <lib/trj/trj_obj.h>

#include <lib_gui/lib/clip/clip.h>

//----------------------------------------------------------------

typedef struct gui_clip
{
    clip::lock      lock;

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

void gui_clip_set_traj (s_gui_clip *clip, s_trj_eng *eng, s_trj_traj *traj)
{
    uint8_t __traj_data__[256 * 1024];
    uint8_t *traj_data = __traj_data__;

    trj_traj_save(traj, eng, &traj_data);

    uint32_t traj_data_size = (uint32_t) (traj_data - __traj_data__);

    clip->lock.set_data(clip->format_traj, (const char*) __traj_data__, traj_data_size);

    return;
}

void gui_clip_get_traj (s_gui_clip *clip, s_trj_eng *eng, s_trj_traj *traj)
{
    uint8_t __traj_data__[256 * 1024];
    uint8_t *traj_data = __traj_data__;
    uint32_t traj_data_size = clip->lock.get_data_length(clip->format_traj);

    if (traj_data_size > 0x00)
    {
        clip->lock.get_data(clip->format_traj, (char*) __traj_data__, traj_data_size);
        traj->free(&traj->data);
        trj_traj_load(traj, eng, &traj_data);
    }

    return;
}

//----------------------------------------------------------------

#endif /* __GUI_CLIP__ */
