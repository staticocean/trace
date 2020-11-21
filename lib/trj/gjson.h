
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __GJSON__
#define __GJSON__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>

#include "vl.h"

//------------------------------------------------------------------------------

typedef enum gjson_geom_type_t
{
	gjson_geom_type_poly = 0x00,
};

typedef struct gjson_pt
{
	vlf_t vec[3];
	
}	s_gjson_pt;

typedef struct gjson_poly
{
	std::vector<s_gjson_pt> pt_list;
	
} s_gjson_poly;

typedef struct gjson_obj
{
	char name[256];
	
	vlf_t p0[3];
	vlf_t p1[3];
	vlf_t mid[3];
	
	gjson_geom_type_t type;
	
	s_gjson_poly poly;
	
}	s_gjson_obj;

//------------------------------------------------------------------------------

#endif /* __GJSON__ */




























