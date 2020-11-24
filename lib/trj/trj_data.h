
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

typedef struct trj_data_text
{
	char file_name[256];
	char *file_data;
	uint32_t file_size;
	uint32_t file_lcnt;
	
} 	s_trj_data_text;

typedef struct trj_data_text_init
{
	char file_name[256];
	
} 	s_trj_data_text_init;

inline uint8_t trj_data_text_init(s_trj_data_text *self, s_trj_data_text_init attr)
{
	strcpy(self->file_name, attr.file_name);
	self->file_data = NULL;
	self->file_size = 0x00;
	self->file_lcnt = 0x00;
	
	return 0x00;
}

inline uint8_t trj_data_text_save(s_trj_data_text *self, s_trj_data_text_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_text_load(s_trj_data_text *self, s_trj_data_text_init *attr, uint8_t **v_file)
{
	self->file_data = NULL;
	self->file_size = 0x00;
	self->file_lcnt = 0x00;
	
	return 0x00;
}

inline uint8_t trj_data_text_render(s_trj_data_text *self, s_trj_obj *obj)
{
	uint32_t i;
	
	self->file_data = (char*) malloc(obj->log_offset * 256);
	char *file_ptr = self->file_data;
	self->file_size = 0x00;
	self->file_lcnt = 0x00;
	
	FILE *file_handle = fopen(self->file_name, "w+");
	
	for (i = 0; i < obj->log_offset; ++i)
	{
		self->file_size += sprintf(file_ptr, "%Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf",
			obj->log_list[i].time[0],
			
			obj->log_list[i].pos[0][0],
			obj->log_list[i].pos[0][1],
			obj->log_list[i].pos[0][2],
			
			obj->log_list[i].pos[1][0],
			obj->log_list[i].pos[1][1],
			obj->log_list[i].pos[1][2],
			
			obj->log_list[i].pos[2][0],
			obj->log_list[i].pos[2][1],
			obj->log_list[i].pos[2][2]
	   	);

		fprintf(file_handle, file_ptr);
		
		file_ptr += 256;
		
		++self->file_lcnt;
	}
	
	fclose(file_handle);
	
	return 0x00;
}

inline uint8_t trj_data_text_reset(s_trj_data_text *self, s_trj_obj *obj)
{
	if (self->file_data != NULL)
	{
		free(self->file_data);
		self->file_data = NULL;
	}
	
	self->file_size = 0x00;
	self->file_lcnt = 0x00;
	
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
	
	if (data_->file_data != NULL)
	{
		free(data_->file_data);
		
		data_->file_data = NULL;
		data_->file_size = 0x00;
		data_->file_lcnt = 0x00;
	}
	
	free(data_);
	
	return 0x00;
}

inline uint8_t trj_data_text_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_text_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_data_text *data_ = (s_trj_data_text*) data;
	s_trj_data_text_init *attr = (s_trj_data_text_init*) config;
	
	return trj_data_text_load(data_, attr, v_file);
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

inline uint8_t trj_data_ram_save(s_trj_data_ram *self, s_trj_data_ram_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_ram_load(s_trj_data_ram *self, s_trj_data_ram_init *attr, uint8_t **v_file)
{
	self->data_list = NULL;
	self->data_offset = NULL;
	
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

inline uint8_t trj_data_ram_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_ram_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_data_ram_init *attr = (s_trj_data_ram_init*) config;
	s_trj_data_ram *data_ = (s_trj_data_ram*) data;
	
	return trj_data_ram_load(data_, attr, v_file);
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
