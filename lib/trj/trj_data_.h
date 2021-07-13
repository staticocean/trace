
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_DATA___
#define __TRJ_DATA___

//----------------------------------------------------------------

#include <lib_internal/vl.h>

#include <lib/tinymat/tinymat.h>

#include "trj_api.h"
#include "trj_data.h"

//----------------------------------------------------------------

typedef struct trj_data_mat
{
	s_trj_data_ram   ram;
	s_trj_data_ramld ramld;
	
	uint8_t 	hpr_en;
    uint8_t 	lla_en;
    uint8_t 	abs_en;
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
    self->abs_en = 0x00;
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
			
			TinyMATWriter_writeMatrixND_rowmajor(
					self->file_data, "ctn", ram->ctn, (const int32_t[]) { 3, 3, (int32_t) ram->offset }, 3);
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
		
		if (self->ecef_en != 0x00)
		{
            TinyMATWriter_writeMatrixND_rowmajor(
                    self->file_data, "ecef_rot", ram->ecef_rot, (const int32_t[]) { 3, 3, (int32_t) ram->offset }, 3);

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

        // ABS

        if (self->abs_en != 0x00)
        {
            TinyMATWriter_writeMatrixND_rowmajor(
                    self->file_data, "abs_rot", ram->abs_rot, (const int32_t[]) { 3, 3, (int32_t) ram->offset }, 3);

            TinyMATWriter_writeMatrix2D_rowmajor(
                    self->file_data, "abs_hpr", ram->abs_hpr, 3, ram->offset);

            TinyMATWriter_writeMatrix2D_rowmajor(
                    self->file_data, "abs_pos", ram->abs_pos, 3, ram->offset);

            TinyMATWriter_writeMatrix2D_rowmajor(
                    self->file_data, "abs_vel", ram->abs_vel, 3, ram->offset);

            TinyMATWriter_writeMatrix2D_rowmajor(
                    self->file_data, "abs_acc", ram->abs_acc, 3, ram->offset);
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

//----------------------------------------------------------------

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

//----------------------------------------------------------------

typedef struct trj_data_bin
{
	s_trj_data_ram   ram;
	
	char file_name[256];
	
} 	s_trj_data_bin;

typedef struct trj_data_bin_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
} 	s_trj_data_bin_init;

inline uint8_t trj_data_bin_init(s_trj_data_bin *self, s_trj_data_bin_init attr)
{
	sprintf(self->file_name, "file not selected");
	
	trj_data_ram_init(&self->ram, (s_trj_data_ram_init) {
			.eng = attr.eng,
			.ref = attr.ref,
	});
	
	return 0x00;
}

inline uint8_t trj_data_bin_save(s_trj_data_bin *self, s_trj_data_bin_init *attr, uint8_t **v_file)
{
	s_trj_data_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trj_data_ram_save(&self->ram, &data_ram_attr, v_file);
	
	return 0x00;
}

inline uint8_t trj_data_bin_load(s_trj_data_bin *self, s_trj_data_bin_init *attr, uint8_t **v_file)
{
	s_trj_data_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trj_data_ram_load(&self->ram, &data_ram_attr, v_file);
	
	return 0x00;
}

typedef struct __attribute__((packed))  trj_data_bin_block
{
	float64_t time;
	
	float64_t heading;
	float64_t pitch;
	float64_t roll;
	
	float64_t lla_pos[3];
	float64_t lla_vel[3];
	float64_t lla_acc[3];
	
	float64_t ecef_pos[3];
	float64_t ecef_vel[3];
	float64_t ecef_acc[3];
	
//	float64_t ref_pos[3];
//	float64_t ref_vel[3];
//	float64_t ref_acc[3];
	
	float64_t tied_acc[3];
	float64_t tied_grs[3];
	
}	s_trj_data_bin_block;

inline uint8_t trj_data_bin_render(s_trj_data_bin *self, s_trj_obj *obj)
{
	s_trj_data_ram *ram = &self->ram;
	
	trj_data_ram_render(ram, obj);
	
	FILE *file_data = fopen(self->file_name, "wb+");
	
	uint32_t i;
	s_trj_data_bin_block block;
	
	for (i = 0; i < self->ram.offset; ++i)
	{
		block.time = self->ram.time[i];
		
		block.heading = self->ram.heading[i];
		block.pitch   = self->ram.pitch[i];
		block.roll    = self->ram.roll[i];
		
		vl3_vcopy(block.lla_pos, &self->ram.lla_pos[i*3]);
		vl3_vcopy(block.lla_vel, &self->ram.lla_vel[i*3]);
		vl3_vcopy(block.lla_acc, &self->ram.lla_acc[i*3]);
		
		vl3_vcopy(block.ecef_pos, &self->ram.ecef_pos[i*3]);
		vl3_vcopy(block.ecef_vel, &self->ram.ecef_vel[i*3]);
		vl3_vcopy(block.ecef_acc, &self->ram.ecef_acc[i*3]);
//
//		vl_vcopy(block.ref_pos, &self->ram.[i*3]);
//		vl_vcopy(block.ref_vel, &self->ram.lla_vel[i*3]);
//		vl_vcopy(block.ref_acc, &self->ram.lla_acc[i*3]);
		
		vl3_vcopy(block.tied_acc, &self->ram.tied_acc[i*3]);
		vl3_vcopy(block.tied_grs, &self->ram.tied_grs[i*3]);
		
		fwrite(&block, sizeof(s_trj_data_bin_block), 1, file_data);
	}
	
	fclose(file_data);
	
	return 0x00;
}

inline uint8_t trj_data_bin_reset(s_trj_data_bin *self, s_trj_obj *obj)
{
	trj_data_ram_reset(&self->ram, obj);
	
	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t trj_data_bin_init_ (void **data, void *config)
{
	*data = (s_trj_data_mat*) malloc(sizeof(s_trj_data_bin));
	
	s_trj_data_bin *data_ = (s_trj_data_bin*) *data;
	s_trj_data_bin_init *data_init = (s_trj_data_bin_init*) config;
	
	return trj_data_bin_init(data_, *data_init);
}

inline uint8_t trj_data_bin_free_ (void **data)
{
	s_trj_data_bin *data_ = (s_trj_data_bin*) *data;
	
	return 0x00;
}

inline uint8_t trj_data_bin_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_bin_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_data_bin *data_ = (s_trj_data_bin*) data;
	s_trj_data_bin_init *attr = (s_trj_data_bin_init*) config;
	
	return trj_data_bin_load(data_, attr, v_file);
}

inline uint8_t trj_data_bin_render_(void *data, void *obj)
{
	s_trj_data_bin *data_ = (s_trj_data_bin*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_bin_render(data_, obj_);
}

inline uint8_t trj_data_bin_reset_(void *data, void *obj)
{
	s_trj_data_bin *data_ = (s_trj_data_bin*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_bin_reset(data_, obj_);
}

//----------------------------------------------------------------

#endif /* __TRJ_DATA___ */
