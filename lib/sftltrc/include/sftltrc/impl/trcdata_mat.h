
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcdata_mat - trace

#ifndef __TRCDATA_MAT__
#define __TRCDATA_MAT__

//------------------------------------------------------------------------------

#include <tinymat/tinymat.h>

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcdata.h>

//------------------------------------------------------------------------------

typedef struct trcdata_mat
{
	s_trcdata_ram   ram;
	s_trcdata_ramld ramld;
	
	u8_t 	hpr_en;
    u8_t 	lla_en;
    u8_t 	abs_en;
	u8_t 	ecef_en;
	u8_t 	tied_en;
	u8_t 	ld_en;
	
	char file_name[256];
	TinyMATWriterFile *file_data;
	
} 	s_trcdata_mat;

typedef struct trcdata_mat_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
} 	s_trcdata_mat_init;

inline u8_t trcdata_mat_init(s_trcdata_mat *self, s_trcdata_mat_init attr)
{
	self->file_data = NULL;
	sprintf(self->file_name, "mat file not selected");
	
	trcdata_ram_init(&self->ram, (s_trcdata_ram_init) {
		.eng = attr.eng,
		.ref = attr.ref,
	});
	
	trcdata_ramld_init(&self->ramld, (s_trcdata_ramld_init) {
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

inline u8_t trcdata_mat_save(s_trcdata_mat *self, s_trcdata_mat_init *attr, u8_t **v_file)
{
	s_trcdata_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trcdata_ram_save(&self->ram, &data_ram_attr, v_file);
	
	s_trcdata_ramld_init data_ramld_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	trcdata_ramld_save(&self->ramld, &data_ramld_attr, v_file);
	
	return 0x00;
}

inline u8_t trcdata_mat_load(s_trcdata_mat *self, s_trcdata_mat_init *attr, u8_t **v_file)
{
	self->file_data = NULL;
	
	s_trcdata_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trcdata_ram_load(&self->ram, &data_ram_attr, v_file);
	
	s_trcdata_ramld_init data_ramld_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trcdata_ramld_load(&self->ramld, &data_ramld_attr, v_file);
	
	return 0x00;
}

inline u8_t trcdata_mat_render(s_trcdata_mat *self, s_trcobj *obj)
{
	s_trcdata_ram *ram = &self->ram;
	s_trcdata_ramld *ramld = &self->ramld;
	
	trcdata_ram_render(ram, obj);
	trcdata_ramld_render(ramld, obj);
	
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
					self->file_data, "ctn", ram->ctn, (const s32_t[]) { 3, 3, (s32_t) ram->offset }, 3);
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
                    self->file_data, "ecef_rot", ram->ecef_rot, (const s32_t[]) { 3, 3, (s32_t) ram->offset }, 3);

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
                    self->file_data, "abs_rot", ram->abs_rot, (const s32_t[]) { 3, 3, (s32_t) ram->offset }, 3);

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

inline u8_t trcdata_mat_reset(s_trcdata_mat *self, s_trcobj *obj)
{
	if (self->file_data != NULL)
	{
		self->file_data = NULL;
	}
	
	trcdata_ram_reset(&self->ram, obj);
	trcdata_ramld_reset(&self->ramld, obj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcdata_mat_init_ (void **data, void *config)
{
	*data = (s_trcdata_mat*) malloc(sizeof(s_trcdata_mat));
	
	s_trcdata_mat *data_ = (s_trcdata_mat*) *data;
	s_trcdata_mat_init *data_init = (s_trcdata_mat_init*) config;
	
	return trcdata_mat_init(data_, *data_init);
}

inline u8_t trcdata_mat_free_ (void **data)
{
	s_trcdata_mat *data_ = (s_trcdata_mat*) *data;
	
	if (data_->file_data != NULL)
	{
		data_->file_data = NULL;
	}
	
	return 0x00;
}

inline u8_t trcdata_mat_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcdata_mat_load_(void *data, void *config, u8_t **v_file)
{
	s_trcdata_mat *data_ = (s_trcdata_mat*) data;
	s_trcdata_mat_init *attr = (s_trcdata_mat_init*) config;
	
	return trcdata_mat_load(data_, attr, v_file);
}

inline u8_t trcdata_mat_render_(void *data, void *obj)
{
	s_trcdata_mat *data_ = (s_trcdata_mat*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_mat_render(data_, obj_);
}

inline u8_t trcdata_mat_reset_(void *data, void *obj)
{
	s_trcdata_mat *data_ = (s_trcdata_mat*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_mat_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRCDATA_MAT__ */
