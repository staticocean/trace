
#ifndef __TRJ_GUI_DATA__
#define __TRJ_GUI_DATA__

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>
#include <math.h>

#include <lib/trj/trj_types.h>
#include <lib/trj/trj_obj.h>
#include <lib/trj/trj_data.h>
#include <lib/trj/trj_data_.h>
#include <lib/trj/vl.h>
#include <lib/trj/vl3d.h>

//------------------------------------------------------------------------------

void trj_gui_data_edit(s_trj_data *self);

void trj_gui_data_text_edit(s_trj_data *self);
void trj_gui_data_text_view(s_trj_data *self);

void trj_gui_data_ram_edit(s_trj_data *self);
void trj_gui_data_ram_view(s_trj_data *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_DATA__ */
