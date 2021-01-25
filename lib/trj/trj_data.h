
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
	s_trj_eng 	*eng;
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	uint8_t 	ellp_en;
	s_trj_ellp 	*ellp;
	uint32_t 	ellp_hash;
	
	uint32_t 	offset;
	
	vlf_t 		*time;
	vlf_t 		*time3;
	
	vlf_t 		*ctn;
	vlf_t 		*heading;
	vlf_t 		*pitch;
	vlf_t 		*roll;
	
	vlf_t 		*lla_pos;
	vlf_t 		*lla_vel;
	vlf_t 		*lla_acc;
	
	vlf_t 		*ecef_pos;
	vlf_t 		*ecef_vel;
	vlf_t 		*ecef_acc;
	
	vlf_t 		*tied_acc;
	vlf_t 		*tied_grs;
	
} 	s_trj_data_ram;

typedef struct trj_data_ram_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
	uint8_t ellp_en;
	s_trj_ellp *ellp;
	
} 	s_trj_data_ram_init;

inline void __trj_data_ram_null__(s_trj_data_ram *self)
{
	self->offset = 0x00;
	
	self->time 		= NULL;
	self->time3		= NULL;
	
	self->ctn 		= NULL;
	self->heading 	= NULL;
	self->pitch 	= NULL;
	self->roll 		= NULL;
	
	self->lla_pos 	= NULL;
	self->lla_vel 	= NULL;
	self->lla_acc 	= NULL;
	
	self->ecef_pos 	= NULL;
	self->ecef_vel 	= NULL;
	self->ecef_acc 	= NULL;
	
	self->tied_acc 	= NULL;
	self->tied_grs 	= NULL;
	
	return;
}

inline void __trj_data_ram_free__(s_trj_data_ram *self)
{
	self->offset = 0x00;
	
	if (self->time 		!= NULL) free(self->time 		);
	if (self->time3		!= NULL) free(self->time3 		);
	
	if (self->ctn 		!= NULL) free(self->ctn 		);
	if (self->heading 	!= NULL) free(self->heading 	);
	if (self->pitch 	!= NULL) free(self->pitch 		);
	if (self->roll 		!= NULL) free(self->roll 		);
	
	if (self->lla_pos 	!= NULL) free(self->lla_pos 	);
	if (self->lla_vel 	!= NULL) free(self->lla_vel 	);
	if (self->lla_acc 	!= NULL) free(self->lla_acc 	);
	
	if (self->ecef_pos 	!= NULL) free(self->ecef_pos 	);
	if (self->ecef_vel 	!= NULL) free(self->ecef_vel 	);
	if (self->ecef_acc 	!= NULL) free(self->ecef_acc 	);
	
	if (self->tied_acc 	!= NULL) free(self->tied_acc 	);
	if (self->tied_grs 	!= NULL) free(self->tied_grs 	);
	
	__trj_data_ram_null__(self);
	
	return;
}

inline uint8_t trj_data_ram_init(s_trj_data_ram *self, s_trj_data_ram_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->ellp_en = attr.ellp_en;
	self->ellp = attr.ellp;
	if (self->ellp != NULL)
	{ self->ellp_hash = vl_crc32(self->ellp->desc); }
	
	__trj_data_ram_null__(self);
	
	return 0x00;
}

inline uint8_t trj_data_ram_save(s_trj_data_ram *self, s_trj_data_ram_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_ram_load(s_trj_data_ram *self, s_trj_data_ram_init *attr, uint8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref  = trj_eng_find_obj (self->eng, self->ref_hash);
	self->ellp = trj_eng_find_ellp(self->eng, self->ellp_hash);
	
	if (self->ref == NULL)
	{
		self->ref_hash = 0x00;
	}
	
	if (self->ellp == NULL)
	{
		self->ellp_hash = 0x00;
		self->ellp_en = 0x00;
	}
	
	__trj_data_ram_null__(self);
	
	return 0x00;
}

inline uint8_t trj_data_ram_render(s_trj_data_ram *self, s_trj_obj *obj)
{
	__trj_data_ram_free__(self);
	self->offset = obj->log_offset;
	
	if (self->offset != 0x00)
	{
		self->time 	 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		self->time3	 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		self->ctn 	 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 9);
		self->heading 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		self->pitch 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		self->roll 		= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		
		self->lla_pos 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->lla_vel 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->lla_acc 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		self->ecef_pos 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->ecef_vel 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->ecef_acc 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		self->tied_acc 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->tied_grs 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		for (int i = 0; i < self->offset; ++i)
		{
			self->time[i] = obj->log_list[i].time[0];
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = obj->log_list[i].time[0];
			self->time3[i*3 + 0x01] = obj->log_list[i].time[0];
			self->time3[i*3 + 0x02] = obj->log_list[i].time[0];
			
			if (self->ref != NULL)
			{
				vlf_t ref_cnt[9];
				
				vl_tnp(ref_cnt, &self->ref->log_list[i].rot[0][0]);
				vl_mmul_m(&self->ctn[i*9], ref_cnt, &obj->log_list[i].rot[0][0]);
				
				vl_vcopy(&self->ecef_pos[i*3], &obj->log_list[i].pos[0][0]);
				vl_vsub(&self->ecef_pos[i*3], &self->ecef_pos[i*3], &self->ref->log_list[i].pos[0][0]);
				vl_mmul_v(&self->ecef_pos[i*3], ref_cnt, &self->ecef_pos[i*3]);
				
				vl_vcopy(&self->ecef_vel[i*3], &obj->log_list[i].pos[1][0]);
				vl_mmul_v(&self->ecef_vel[i*3], ref_cnt, &self->ecef_vel[i*3]);

				vl_vcopy(&self->ecef_acc[i*3], &obj->log_list[i].pos[2][0]);
				vl_mmul_v(&self->ecef_acc[i*3], ref_cnt, &self->ecef_acc[i*3]);
			}
			
			else
			{
				vl_vcopy(&self->ecef_pos[i*3], &obj->log_list[i].pos[0][0]);
				vl_vcopy(&self->ecef_vel[i*3], &obj->log_list[i].pos[1][0]);
				vl_vcopy(&self->ecef_acc[i*3], &obj->log_list[i].pos[2][0]);
			}
			
			if (self->ellp != NULL)
			{
				vlf_t cnt_ecef[9];
				vlf_t ctn_local[9];
				trj_ellp_ecefrot(self->ellp, &self->ecef_pos[i*3], cnt_ecef);
				vl_tnp(cnt_ecef, cnt_ecef);
				vl_mmul_m(ctn_local, cnt_ecef, &self->ctn[i*9]);
				
				s_trj_rot_hpr hpr;
				trj_ctn_to_hpr(&hpr, ctn_local);
				
				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;
				
				trj_ellp_lla(self->ellp, &self->lla_pos[i*3], &self->ecef_pos[i*3]);
//				trj_ellp_nwhvel(self->ellp, &self->lla_pos[i*3], &self->lla_vel[i*3]);
			}
			
			else
			{
				vl_mcopy(&self->ctn[i*9], &obj->log_list[i].rot[0][0]);
			}
			
			vlf_t self_cnt[9];
			vl_tnp(self_cnt, &obj->log_list[i].rot[0][0]);
			
			vl_vmul_s(&self->tied_acc[i*3], obj->log_list[i].pos_force, 1.0 / obj->pos_inert);
			vl_vsum(&self->tied_acc[i*3], &self->tied_acc[i*3], &obj->log_list[i].pos[2][0]);
			vl_mmul_v(&self->tied_acc[i*3], self_cnt, &self->tied_acc[i*3]);
		}
	}
	
	return 0x00;
}

inline uint8_t trj_data_ram_reset(s_trj_data_ram *self, s_trj_obj *obj)
{
	__trj_data_ram_free__(self);
	
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
	__trj_data_ram_free__(data_);
	
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
