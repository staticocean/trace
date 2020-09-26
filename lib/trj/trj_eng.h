
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_ENG__
#define __TRJ_ENG__

//------------------------------------------------------------------------------

#include <string.h>

#include "trj_types.h"
#include "vl.h"

#include "trj_obj.h"
#include "trj_utils.h"

//------------------------------------------------------------------------------

typedef struct trj_eng
{
	uint32_t obj_count;
	s_trj_obj *obj_list;
	
}	s_trj_eng;

typedef struct trj_eng_init_attr
{
	s_trj_obj *st_objects;
	
}	s_trj_eng_init_attr;

//------------------------------------------------------------------------------

uint8_t trj_eng_init(s_trj_eng *self, s_trj_eng_init_attr attr);
uint8_t trj_eng_add(s_trj_eng *self, s_trj_obj obj);
uint8_t trj_eng_update(s_trj_eng *self, vlf_t d_time);
uint8_t trj_eng_objnames(s_trj_eng *self, char **objnames);

//------------------------------------------------------------------------------


#endif /* __TRJ_ENG__ */
