
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_DATA___
#define __TRJ_DATA___

//------------------------------------------------------------------------------

#include <softael_lib/vl.h>

#include <lib/tinymat/tinymat.h>

#include "trj_api.h"
#include "trj_data.h"

//------------------------------------------------------------------------------

typedef struct trj_data_mat
{
	s_trj_data_ram   ram;
	s_trj_data_ramld ramld;
	
	uint8_t 	hpr_en;
	uint8_t 	lla_en;
	uint8_t 	ecef_en;
	uint8_t 	tied_en;
	uint8_t 	ld_en;
	
	char file_name[256];
	TinyMATWriterFile *file_data;
	
} 	s_trj_data_mat;

typedef struct trj_data_mat_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
} 	s_trj_data_mat_init;

inline uint8_t trj_data_mat_init(s_trj_data_mat *self, s_trj_data_mat_init attr)
{
	self->file_data = NULL;
	sprintf(self->file_name, "mat file not selected");
	
	trj_data_ram_init(&self->ram, (s_trj_data_ram_init) {
		.eng = attr.eng,
		.ref = attr.ref,
	});
	
	trj_data_ramld_init(&self->ramld, (s_trj_data_ramld_init) {
		.eng = attr.eng,
		.ref = attr.ref,
	});
	
	self->tied_en = 0x00;
	self->lla_en  = 0x00;
	self->hpr_en  = 0x00;
	self->ecef_en = 0x00;
	self->ld_en = 0x00;
	
	return 0x00;
}

inline uint8_t trj_data_mat_save(s_trj_data_mat *self, s_trj_data_mat_init *attr, uint8_t **v_file)
{
	s_trj_data_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trj_data_ram_save(&self->ram, &data_ram_attr, v_file);
	
	s_trj_data_ramld_init data_ramld_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	trj_data_ramld_save(&self->ramld, &data_ramld_attr, v_file);
	
	return 0x00;
}

inline uint8_t trj_data_mat_load(s_trj_data_mat *self, s_trj_data_mat_init *attr, uint8_t **v_file)
{
	self->file_data = NULL;
	
	s_trj_data_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trj_data_ram_load(&self->ram, &data_ram_attr, v_file);
	
	s_trj_data_ramld_init data_ramld_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trj_data_ramld_load(&self->ramld, &data_ramld_attr, v_file);
	
	return 0x00;
}

inline uint8_t trj_data_mat_render(s_trj_data_mat *self, s_trj_obj *obj)
{
	s_trj_data_ram *ram = &self->ram;
	s_trj_data_ramld *ramld = &self->ramld;
	
	trj_data_ram_render(ram, obj);
	trj_data_ramld_render(ramld, obj);
	
	self->file_data = TinyMATWriter_open(self->file_name);
	
	if (self->file_data)
	{
		TinyMATWriter_writeVectorAsColumn(
				self->file_data, "time", ram->time, ram->offset);
		
		// HPR
		
		if (self->hpr_en != 0x00)
		{
			TinyMATWriter_writeVectorAsColumn(
					self->file_data, "heading", ram->heading, ram->offset);
			
			TinyMATWriter_writeVectorAsColumn(
					self->file_data, "pitch", ram->pitch, ram->offset);
			
			TinyMATWriter_writeVectorAsColumn(
					self->file_data, "roll", ram->roll, ram->offset);
			
			TinyMATWriter_writeMatrixND_colmajor(
					self->file_data, "ctn", ram->ctn, (const int32_t[]) { (int32_t) ram->offset, 3, 3 }, 3);
		}
		
		// LLA
		
		if (self->lla_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "lla_pos", ram->lla_pos, 3, ram->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "lla_vel", ram->lla_vel, 3, ram->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "lla_acc", ram->lla_acc, 3, ram->offset);
		}
		
		// ECEF
		
		if (self->lla_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "ecef_pos", ram->ecef_pos, 3, ram->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "ecef_vel", ram->ecef_vel, 3, ram->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "ecef_acc", ram->ecef_acc, 3, ram->offset);
		}
		
		// TIED
		
		if (self->tied_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "tied_acc", ram->tied_acc, 3, ram->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "tied_grs", ram->tied_grs, 3, ram->offset);
		}
		
		// LATERAL DEVIATION
		
		if (self->ld_en != 0x00)
		{
			TinyMATWriter_writeVectorAsColumn(
					self->file_data, "ld", ramld->ld, ramld->offset);
		}
		
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
	
	trj_data_ram_reset(&self->ram, obj);
	trj_data_ramld_reset(&self->ramld, obj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_data_mat_init_ (void **data, void *config)
{
	*data = (s_trj_data_mat*) malloc(sizeof(s_trj_data_mat));
	
	s_trj_data_mat *data_ = (s_trj_data_mat*) *data;
	s_trj_data_mat_init *data_init = (s_trj_data_mat_init*) config;
	
	return trj_data_mat_init(data_, *data_init);
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
