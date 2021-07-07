
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
    clip->lock.clear();

    return;
}

void gui_clip_set_traj (s_gui_clip *clip, s_trj_traj *traj)
{
    clip::

    return;
}

//----------------------------------------------------------------

#endif /* __GUI_CLIP__ */
