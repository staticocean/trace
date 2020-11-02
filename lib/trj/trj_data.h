
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_DATA__
#define __TRJ_DATA__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

#include "trj_api.h"

//------------------------------------------------------------------------------

typedef struct trj_data_text
{
	uint32_t temp;

} 	s_trj_data_text;

typedef struct trj_data_text_init
{
	uint32_t temp;

} 	s_trj_data_text_init;

inline uint8_t trj_data_text_init(s_trj_data_text *data, s_trj_data_text_init attr)
{
	return 0x00;
}

inline uint8_t trj_data_text_render(s_trj_data_text *data, s_trj_obj *obj)
{
	return 0x00;
}

inline uint8_t trj_data_text_init_ (void **data, void *config)
{
	*data = (s_trj_data_text*) malloc(sizeof(s_trj_data_text));
	
	s_trj_data_text *data_ = (s_trj_data_text*) *data;
	s_trj_data_text_init *data_init = (s_trj_data_text_init*) config;
	
	return trj_data_text_init(data_, *data_init);
}

inline uint8_t trj_data_text_free_ (void **data)
{
	s_trj_data_text *data_ = (s_trj_data_text*) *data;
	
	free(data_);
	
	return 0x00;
}

inline uint8_t trj_data_text_render_(void *data, void *obj)
{
	s_trj_data_text *data_ = (s_trj_data_text*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_text_render(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRJ_UTILS__ */
