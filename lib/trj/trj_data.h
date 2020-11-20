
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_DATA__
#define __TRJ_DATA__

//------------------------------------------------------------------------------

#include "vl.h"

#include "trj_api.h"

//------------------------------------------------------------------------------

#define trj_data_text_id (0x00000001)

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

inline uint8_t trj_data_text_reset(s_trj_data_text *data, s_trj_obj *obj)
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

inline uint8_t trj_data_text_reset_(void *data, void *obj)
{
	s_trj_data_text *data_ = (s_trj_data_text*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_text_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#define trj_data_ram_id (0x00000002)

typedef struct trj_data_ram
{
	s_trj_obj_data *data_list;
	uint32_t *data_offset;
	
} 	s_trj_data_ram;

typedef struct trj_data_ram_init
{
	uint32_t temp;
	
} 	s_trj_data_ram_init;

inline uint8_t trj_data_ram_init(s_trj_data_ram *data, s_trj_data_ram_init attr)
{
	data->data_list = NULL;
	data->data_offset = 0x00;
	
	return 0x00;
}

inline uint8_t trj_data_ram_render(s_trj_data_ram *data, s_trj_obj *obj)
{
	data->data_list = obj->log_list;
	data->data_offset = &obj->log_offset;
	
	return 0x00;
}

inline uint8_t trj_data_ram_reset(s_trj_data_ram *data, s_trj_obj *obj)
{
	data->data_list = NULL;
	data->data_offset = NULL;
	
	return 0x00;
}

inline uint8_t trj_data_ram_init_ (void **data, void *config)
{
	*data = (s_trj_data_ram*) malloc(sizeof(s_trj_data_ram));
	
	s_trj_data_ram *data_ = (s_trj_data_ram*) *data;
	s_trj_data_ram_init *data_init = (s_trj_data_ram_init*) config;
	
	return trj_data_ram_init(data_, *data_init);
}

inline uint8_t trj_data_ram_free_ (void **data)
{
	s_trj_data_ram *data_ = (s_trj_data_ram*) *data;
	
	free(data_);
	
	return 0x00;
}

inline uint8_t trj_data_ram_render_(void *data, void *obj)
{
	s_trj_data_ram *data_ = (s_trj_data_ram*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_ram_render(data_, obj_);
}

inline uint8_t trj_data_ram_reset_(void *data, void *obj)
{
	s_trj_data_ram *data_ = (s_trj_data_ram*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_ram_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRJ_UTILS__ */
