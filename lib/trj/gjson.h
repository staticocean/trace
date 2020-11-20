
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
#include <array>

#include "vl.h"

//------------------------------------------------------------------------------

typedef enum gjson_geom_type_t
{
	gjson_geom_type_poly = 0x00,
};

typedef struct gjson_poly
{
	gjson_geom_type_t type;
	std::vector<std::array<vlf_t, 3>> pt_list;
	
} s_gjson_poly;

typedef union gjson_obj
{
	gjson_geom_type_t type;
	
	s_gjson_poly poly;
	
} 	s_gjson_obj;

typedef struct gjson_feature
{
	char name[256];
	
	std::vector<s_gjson_obj> obj_list;
	
}	s_gjson_feature;

//------------------------------------------------------------------------------

inline uint8_t gjson_load(char *filename)
{
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __GJSON__ */




























