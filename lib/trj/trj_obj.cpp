
#include "trj_obj.h"

//------------------------------------------------------------------------------

void trj_obj_update(s_trj_obj *obj, vlf_t d_time)
{
	obj->time[2] = d_time * d_time * 0.5;
	obj->time[1] = d_time;
	obj->time[0] = obj->time[0] + d_time;
   
    if (obj->ctrl_data != NULL && obj->ctrl_update != NULL)
    {
        obj->ctrl_update(obj->ctrl_data);
    }

    return;
}

//------------------------------------------------------------------------------




