
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcdata_bin - trace

#ifndef __TRCDATA_BIN__
#define __TRCDATA_BIN__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcdata.h>

//------------------------------------------------------------------------------

typedef struct trcdata_bin
{
	s_trcdata_ram   ram;
	
	char file_name[256];
	
} 	s_trcdata_bin;

typedef struct trcdata_bin_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
} 	s_trcdata_bin_init;

inline u8_t trcdata_bin_init(s_trcdata_bin *self, s_trcdata_bin_init attr)
{
	sprintf(self->file_name, "file not selected");
	
	trcdata_ram_init(&self->ram, (s_trcdata_ram_init) {
			.eng = attr.eng,
			.ref = attr.ref,
	});
	
	return 0x00;
}

inline u8_t trcdata_bin_save(s_trcdata_bin *self, s_trcdata_bin_init *attr, u8_t **v_file)
{
	s_trcdata_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trcdata_ram_save(&self->ram, &data_ram_attr, v_file);
	
	return 0x00;
}

inline u8_t trcdata_bin_load(s_trcdata_bin *self, s_trcdata_bin_init *attr, u8_t **v_file)
{
	s_trcdata_ram_init data_ram_attr = {
			.eng = attr->eng,
			.ref = attr->ref,
	};
	
	trcdata_ram_load(&self->ram, &data_ram_attr, v_file);
	
	return 0x00;
}

typedef struct __attribute__((packed))  trcdata_bin_block
{
	f64_t time;
	
	f64_t heading;
	f64_t pitch;
	f64_t roll;
	
	f64_t lla_pos[3];
	f64_t lla_vel[3];
	f64_t lla_acc[3];
	
	f64_t ecef_pos[3];
	f64_t ecef_vel[3];
	f64_t ecef_acc[3];
	
//	f64_t ref_pos[3];
//	f64_t ref_vel[3];
//	f64_t ref_acc[3];
	
	f64_t tied_acc[3];
	f64_t tied_grs[3];
	
}	s_trcdata_bin_block;

inline u8_t trcdata_bin_render(s_trcdata_bin *self, s_trcobj *obj)
{
	s_trcdata_ram *ram = &self->ram;
	
	trcdata_ram_render(ram, obj);
	
	FILE *file_data = fopen(self->file_name, "wb+");
	
	u32_t i;
	s_trcdata_bin_block block;
	
	for (s32_t i = 0; i < self->ram.offset; ++i)
	{
		block.time = self->ram.time[i];
		
		block.heading = self->ram.heading[i];
		block.pitch   = self->ram.pitch[i];
		block.roll    = self->ram.roll[i];
		
		vl3v_copy(block.lla_pos, &self->ram.lla_pos[i*3]);
		vl3v_copy(block.lla_vel, &self->ram.lla_vel[i*3]);
		vl3v_copy(block.lla_acc, &self->ram.lla_acc[i*3]);
		
		vl3v_copy(block.ecef_pos, &self->ram.ecef_pos[i*3]);
		vl3v_copy(block.ecef_vel, &self->ram.ecef_vel[i*3]);
		vl3v_copy(block.ecef_acc, &self->ram.ecef_acc[i*3]);
//
//		vl_vcopy(block.ref_pos, &self->ram.[i*3]);
//		vl_vcopy(block.ref_vel, &self->ram.lla_vel[i*3]);
//		vl_vcopy(block.ref_acc, &self->ram.lla_acc[i*3]);
		
		vl3v_copy(block.tied_acc, &self->ram.tied_acc[i*3]);
		vl3v_copy(block.tied_grs, &self->ram.tied_grs[i*3]);
		
		fwrite(&block, sizeof(s_trcdata_bin_block), 1, file_data);
	}
	
	fclose(file_data);
	
	return 0x00;
}

inline u8_t trcdata_bin_reset(s_trcdata_bin *self, s_trcobj *obj)
{
	trcdata_ram_reset(&self->ram, obj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcdata_bin_init_ (void **data, void *config)
{
	*data = (s_trcdata_mat*) malloc(sizeof(s_trcdata_bin));
	
	s_trcdata_bin *data_ = (s_trcdata_bin*) *data;
	s_trcdata_bin_init *data_init = (s_trcdata_bin_init*) config;
	
	return trcdata_bin_init(data_, *data_init);
}

inline u8_t trcdata_bin_free_ (void **data)
{
	s_trcdata_bin *data_ = (s_trcdata_bin*) *data;
	
	return 0x00;
}

inline u8_t trcdata_bin_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcdata_bin_load_(void *data, void *config, u8_t **v_file)
{
	s_trcdata_bin *data_ = (s_trcdata_bin*) data;
	s_trcdata_bin_init *attr = (s_trcdata_bin_init*) config;
	
	return trcdata_bin_load(data_, attr, v_file);
}

inline u8_t trcdata_bin_render_(void *data, void *obj)
{
	s_trcdata_bin *data_ = (s_trcdata_bin*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_bin_render(data_, obj_);
}

inline u8_t trcdata_bin_reset_(void *data, void *obj)
{
	s_trcdata_bin *data_ = (s_trcdata_bin*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_bin_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __trcdata___ */
