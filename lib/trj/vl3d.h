
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __VL3D__
#define __VL3D__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>
#include <math.h>

#include "trj_types.h"
#include "vl.h"
#include "vl_svd.h"

//------------------------------------------------------------------------------

#define vl3d_type_line (0x80000000)

//------------------------------------------------------------------------------

enum vl_obj_type
{
	vl_obj_type_line = 0x01,
};


typedef struct vl3d_line
{
	uint16_t type;
	uint16_t id;
	
	vlf_t p0[3];
	vlf_t p1[3];
	
}	s_vl3d_line;

typedef union vl3d_obj
{
	struct {
		uint16_t type;
		uint16_t id;
	};
	
	s_vl3d_line line;

} 	s_vl3d_obj;


typedef struct vl3d_eng
{
	uint32_t obj_offset;
	s_vl3d_obj *obj_list;
	
}	s_vl3d_eng;

typedef struct vl3d_eng_init_attr
{
	s_vl3d_obj *obj_list;
	
}	s_vl3d_eng_init_attr;

typedef struct vl3d_view
{
	vlf_t pos[3];
	vlf_t rot[3][3];
	vlf_t scale;
	
	ImRect rect;
	
} 	s_vl3d_view;

//------------------------------------------------------------------------------

uint8_t vl3d_eng_init(s_vl3d_eng *self, s_vl3d_eng_init_attr attr);
uint8_t vl3d_eng_add_line(s_vl3d_eng *self, s_vl3d_line obj);
uint8_t vl3d_eng_render(s_vl3d_eng *self, s_vl3d_view *view, char *label, ImVec2 size);

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

#endif /* __VL__ */




























