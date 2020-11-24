
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_DATA___
#define __TRJ_DATA___

//------------------------------------------------------------------------------

#include "vl.h"
#include "trj_api.h"

#include <lib/tinymat/tinymat.h>

//------------------------------------------------------------------------------

typedef struct trj_data_mat
{
	char file_name[256];
	TinyMATWriterFile *file_data;
	
} 	s_trj_data_mat;

typedef struct trj_data_mat_init
{
	char file_name[256];
	
} 	s_trj_data_mat_init;

inline uint8_t trj_data_mat_init(s_trj_data_mat *self, s_trj_data_mat_init attr)
{
	strcpy(self->file_name, attr.file_name);
	self->file_data = NULL;
	
	return 0x00;
}

inline uint8_t trj_data_mat_save(s_trj_data_mat *self, s_trj_data_mat_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_mat_load(s_trj_data_mat *self, s_trj_data_mat_init *attr, uint8_t **v_file)
{
	self->file_data = NULL;
	
	return 0x00;
}

inline uint8_t trj_data_mat_render(s_trj_data_mat *self, s_trj_obj *obj)
{
	uint32_t i;
	
	self->file_data = TinyMATWriter_open(self->file_name);
	
	if (self->file_data)
	{
		vlf_t *vec_list = (vlf_t *) malloc(sizeof(vlf_t) * obj->log_offset * 3);
		
		for (i = 0; i < obj->log_offset; ++i)
		{ vl_vcopy(&vec_list[i * 3], &obj->log_list[i].pos[0][0]); }
		
		TinyMATWriter_writeMatrix2D_rowmajor(
				self->file_data, "pos_ecef", vec_list, 3, obj->log_offset);
		
		for (i = 0; i < obj->log_offset; ++i)
		{ vl_vcopy(&vec_list[i * 3], &obj->log_list[i].pos[1][0]); }
		
		TinyMATWriter_writeMatrix2D_rowmajor(
				self->file_data, "vec_ecef", vec_list, 3, obj->log_offset);
		
		for (i = 0; i < obj->log_offset; ++i)
		{ vl_vcopy(&vec_list[i * 3], &obj->log_list[i].pos[2][0]); }
		
		TinyMATWriter_writeMatrix2D_rowmajor(
				self->file_data, "acc_ecef", vec_list, 3, obj->log_offset);
		
		vlf_t ctn_t[9];
		
		for (i = 0; i < obj->log_offset; ++i)
		{
			vl_tnp(ctn_t, &obj->log_list[i].rot[0][0]);
			vl_mmul_v(&vec_list[i * 3], ctn_t, &obj->log_list[i].pos[2][0]);
		}
		
		TinyMATWriter_writeMatrix2D_rowmajor(
				self->file_data, "acc_tied", vec_list, 3, obj->log_offset);
		
		free(vec_list);
		
		TinyMATWriter_close(self->file_data);
	}
	
	return 0x00;
}

inline uint8_t trj_data_mat_reset(s_trj_data_mat *self, s_trj_obj *obj)
{
	if (self->file_data != NULL)
	{
		self->file_data = NULL;
	}
	
	return 0x00;
}

inline uint8_t trj_data_mat_init_ (void **data, void *config)
{
	*data = (s_trj_data_mat*) malloc(sizeof(s_trj_data_mat));
	
	s_trj_data_text *data_ = (s_trj_data_text*) *data;
	s_trj_data_text_init *data_init = (s_trj_data_text_init*) config;
	
	return trj_data_text_init(data_, *data_init);
}

inline uint8_t trj_data_mat_free_ (void **data)
{
	s_trj_data_mat *data_ = (s_trj_data_mat*) *data;
	
	if (data_->file_data != NULL)
	{
		data_->file_data = NULL;
	}
	
	return 0x00;
}

inline uint8_t trj_data_mat_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_mat_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_data_mat *data_ = (s_trj_data_mat*) data;
	s_trj_data_mat_init *attr = (s_trj_data_mat_init*) config;
	
	return trj_data_mat_load(data_, attr, v_file);
}

inline uint8_t trj_data_mat_render_(void *data, void *obj)
{
	s_trj_data_mat *data_ = (s_trj_data_mat*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_mat_render(data_, obj_);
}

inline uint8_t trj_data_mat_reset_(void *data, void *obj)
{
	s_trj_data_mat *data_ = (s_trj_data_mat*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_mat_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRJ_DATA___ */
