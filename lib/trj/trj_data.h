
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
	uint32_t temp;
	
} 	s_trj_data_text_init;

inline uint8_t trj_data_text_init(s_trj_data_text *self, s_trj_data_text_init attr)
{
	strcpy(self->file_name, "No file selected");
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
	
	vlf_t 		*heading;
	vlf_t 		*pitch;
	vlf_t 		*roll;
	vlf_t 		*lla_pos;
	vlf_t 		*lla_vel;
	vlf_t 		*lla_acc;
	
	vlf_t 		*ecef_ctn;
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
	
} 	s_trj_data_ram_init;

inline void __trj_data_ram_null__(s_trj_data_ram *self)
{
	self->offset = 0x00;
	
	self->time 		= NULL;
	self->time3		= NULL;
	
	self->heading 	= NULL;
	self->pitch 	= NULL;
	self->roll 		= NULL;
	self->lla_pos 	= NULL;
	self->lla_vel 	= NULL;
	self->lla_acc 	= NULL;
	
	self->ecef_ctn	= NULL;
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
	
	if (self->heading 	!= NULL) free(self->heading 	);
	if (self->pitch 	!= NULL) free(self->pitch 		);
	if (self->roll 		!= NULL) free(self->roll 		);
	if (self->lla_pos 	!= NULL) free(self->lla_pos 	);
	if (self->lla_vel 	!= NULL) free(self->lla_vel 	);
	if (self->lla_acc 	!= NULL) free(self->lla_acc 	);
	
	if (self->ecef_ctn	!= NULL) free(self->ecef_ctn	);
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
	
	self->ellp_en = 0x00;
	self->ellp = NULL;
	
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
		
		self->heading 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		self->pitch 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		self->roll 		= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		
		self->lla_pos 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->lla_vel 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->lla_acc 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		self->ecef_ctn	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 9);
		self->ecef_pos 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->ecef_vel 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->ecef_acc 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		self->tied_acc 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		self->tied_grs 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		for (int i = 0; i < self->offset; ++i)
		{
			s_trj_obj_data *log = &obj->log_list[i];
			self->time[i] = log->time[0];
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = log->time[0];
			self->time3[i*3 + 0x01] = log->time[0];
			self->time3[i*3 + 0x02] = log->time[0];
			
			if (self->ref != NULL)
			{
				vlf_t ref_cnt[9];
				
				vl_tnp(ref_cnt, &self->ref->log_list[i].rot[0][0]);
				vl_mmul_m(&self->ecef_ctn[i*9], ref_cnt, &obj->log_list[i].rot[0][0]);
				
				vl_vsub(&self->ecef_pos[i*3], &obj->log_list[i].pos[0][0], &self->ref->log_list[i].pos[0][0]);
				vl_mmul_v(&self->ecef_pos[i*3], ref_cnt, &self->ecef_pos[i*3]);
				
				vl_mmul_v(&self->ecef_vel[i*3], ref_cnt, &obj->log_list[i].pos[1][0]);
				
				vl_mmul_v(&self->ecef_acc[i*3], ref_cnt, &obj->log_list[i].pos[2][0]);
			}
			
			else
			{
				vl_mcopy(&self->ecef_ctn[i*9], &obj->log_list[i].rot[0][0]);
				vl_vcopy(&self->ecef_pos[i*3], &obj->log_list[i].pos[0][0]);
				vl_vcopy(&self->ecef_vel[i*3], &obj->log_list[i].pos[1][0]);
				vl_vcopy(&self->ecef_acc[i*3], &obj->log_list[i].pos[2][0]);
			}
			
			if (self->ellp != NULL && self->ref != NULL)
			{
				vlf_t ecef_ctn[9];
				vlf_t ctn_local[9];
				trj_ellp_ecefrot(self->ellp, &self->ecef_pos[i*3], ecef_ctn);
				vl_mtmul_m(ctn_local, ecef_ctn, &self->ecef_ctn[i*9]);

				s_vl_hpr hpr;
				vl_hpr(&hpr, ctn_local);

				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;

				trj_ellp_lla(self->ellp, &self->lla_pos[i*3], &self->ecef_pos[i*3]);
				
				vlf_t ecef_ctn_[9];
				vlf_t ecef_pos_[3];

				vl_vsub(ecef_pos_, &obj->log_list[i].pos[0][0], &self->ref->log_list[i].pos[0][0]);
				vl_mtmul_v(ecef_pos_, &self->ref->log_list[i].rot[0][0], ecef_pos_);
				trj_ellp_ecefrot(self->ellp, ecef_pos_, ecef_ctn_);
				
				vl_vsub(&self->lla_vel[i*3], &obj->log_list[i].pos[1][0], &self->ref->log_list[i].pos[1][0]);
				vlf_t rel_pos[3];
				vl_vsub(rel_pos, &obj->log_list[i].pos[0][0], &self->ref->log_list[i].pos[0][0]);
				vlf_t ang_vel[3];
				vl_mmul_v(ang_vel, &self->ref->log_list[i].rot[1][0], rel_pos);
				vl_vsub(&self->lla_vel[i*3], &self->lla_vel[i*3], ang_vel);

				vl_mtmul_v(&self->lla_vel[i*3], &self->ref->log_list[i].rot[0][0], &self->lla_vel[i*3]);
				vl_mtmul_v(&self->lla_vel[i*3], ecef_ctn_, &self->lla_vel[i*3]);

				// swap 1 and 2 because ecef_cnt is proj to NHW not NWH
				vlf_t vh = self->lla_vel[i*3 + 1];
				self->lla_vel[i*3 + 1] = self->lla_vel[i*3 + 2];
				self->lla_vel[i*3 + 2] = vh;
			}
			
			else
			{
				s_vl_hpr hpr;
				vl_hpr(&hpr, &self->ecef_ctn[i*9]);
				
				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;
			}
			
			vl_vmul_s(&self->tied_acc[i*3], log->pos_force, -1.0 / obj->pos_inert);
			vl_vsum(&self->tied_acc[i*3], &self->tied_acc[i*3], &log->pos[2][0]);
			vl_mtmul_v(&self->tied_acc[i*3], &log->rot[0][0], &self->tied_acc[i*3]);
			
			vl_unskew(&self->tied_grs[i*3], &log->rot[1][0]);
			vl_mtmul_v(&self->tied_grs[i*3], &log->rot[0][0], &self->tied_grs[i*3]);
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

typedef struct trj_data_ramld
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
	
	vlf_t 		*ld;
	
} 	s_trj_data_ramld;

typedef struct trj_data_ramld_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
} 	s_trj_data_ramld_init;

inline void __trj_data_ramld_null__(s_trj_data_ramld *self)
{
	self->offset = 0x00;
	
	self->time 		= NULL;
	self->time3		= NULL;
	
	self->ld    	= NULL;
	
	return;
}

inline void __trj_data_ramld_free__(s_trj_data_ramld *self)
{
	self->offset = 0x00;
	
	if (self->time 		!= NULL) free(self->time 		);
	if (self->time3		!= NULL) free(self->time3 		);
	
	if (self->ld 	 	!= NULL) free(self->ld       	);
	
	__trj_data_ramld_null__(self);
	
	return;
}

inline uint8_t trj_data_ramld_init(s_trj_data_ramld *self, s_trj_data_ramld_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->ellp_en = 0x00;
	self->ellp = NULL;
	
	__trj_data_ramld_null__(self);
	
	return 0x00;
}

inline uint8_t trj_data_ramld_save(s_trj_data_ramld *self, s_trj_data_ramld_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_ramld_load(s_trj_data_ramld *self, s_trj_data_ramld_init *attr, uint8_t **v_file)
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
	
	__trj_data_ramld_null__(self);
	
	return 0x00;
}

inline uint8_t trj_data_ramld_render(s_trj_data_ramld *self, s_trj_obj *obj)
{
	__trj_data_ramld_free__(self);
	self->offset = obj->log_offset;
	
	if (self->offset != 0x00)
	{
		self->time 	 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		self->time3	 	= (vlf_t*) malloc(sizeof(vlf_t) * self->offset * 3);
		
		self->ld 		= (vlf_t*) malloc(sizeof(vlf_t) * self->offset);
		
		vlf_t ecef_f[3]; // first
		vlf_t ecef_l[3]; // last
		
		if (self->ref != NULL)
		{
			vl_vsub(ecef_f, &obj->log_list[0].pos[0][0], &self->ref->log_list[0].pos[0][0]);
			vl_vsub(ecef_l, &obj->log_list[self->ref->log_offset-1].pos[0][0], &self->ref->log_list[self->ref->log_offset-1].pos[0][0]);
			vl_mtmul_v(ecef_f, &self->ref->log_list[0].rot[0][0], ecef_f);
			vl_mtmul_v(ecef_l, &self->ref->log_list[self->ref->log_offset-1].rot[0][0], ecef_l);
		}
		
		for (int i = 0; i < self->offset; ++i)
		{
			s_trj_obj_data *log = &obj->log_list[i];
			self->time[i] = log->time[0];
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = log->time[0];
			self->time3[i*3 + 0x01] = log->time[0];
			self->time3[i*3 + 0x02] = log->time[0];
			
			vlf_t ecef[3];
			
			if (self->ref != NULL && self->ellp != NULL && self->ellp_en != 0x00)
			{
				vl_vsub(ecef, &obj->log_list[i].pos[0][0], &self->ref->log_list[i].pos[0][0]);
				vl_mtmul_v(ecef, &self->ref->log_list[i].rot[0][0], ecef);
				
				vlf_t trje[3];
				vl_vsub(trje, ecef_l, ecef_f);
				vl_vmul_s(trje, trje, 1.0 / vl_vnorm(trje));
				
				vlf_t ort[3];
				vl_vsub(ort, ecef, ecef_f);
				
				vlf_t path = vl_vdot(ort, trje);
				vl_vsumm(ort, ort, trje, -path);
				
				vlf_t ref_pos[3];
				vl_vsub(ref_pos, ecef, ort);
				
				vl_vmul_s(ecef, ecef, 1.0 / vl_vnorm(ecef));
				vl_vmul_s(ort, ort, 1.0 / vl_vnorm(ort));
				vl_vmul_s(ref_pos, ref_pos, 1.0 / vl_vnorm(ref_pos));
				
				vlf_t arc_cos = vl_vdot(ref_pos, ecef);
				vlf_t arc = acos(arc_cos);
				
				vlf_t right_e[3];
				vl_cross(right_e, ecef, trje);
				
				if (vl_vdot(ort, right_e) < 0.0) { arc = -arc; }
				
				self->ld[i] = arc * self->ellp->a;
			}
			
			else
			{
				vl_vcopy(ecef, &obj->log_list[i].pos[0][0]);
			}
		}
	}
	
	return 0x00;
}

inline uint8_t trj_data_ramld_reset(s_trj_data_ramld *self, s_trj_obj *obj)
{
	__trj_data_ramld_free__(self);
	
	return 0x00;
}

inline uint8_t trj_data_ramld_init_ (void **data, void *config)
{
	*data = (s_trj_data_ramld*) malloc(sizeof(s_trj_data_ramld));
	
	s_trj_data_ramld *data_ = (s_trj_data_ramld*) *data;
	s_trj_data_ramld_init *data_init = (s_trj_data_ramld_init*) config;
	
	return trj_data_ramld_init(data_, *data_init);
}

inline uint8_t trj_data_ramld_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_ramld_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_data_ramld_init *attr = (s_trj_data_ramld_init*) config;
	s_trj_data_ramld *data_ = (s_trj_data_ramld*) data;
	
	return trj_data_ramld_load(data_, attr, v_file);
}

inline uint8_t trj_data_ramld_free_ (void **data)
{
	s_trj_data_ramld *data_ = (s_trj_data_ramld*) *data;
	__trj_data_ramld_free__(data_);
	
	free(data_);
	
	return 0x00;
}

inline uint8_t trj_data_ramld_render_(void *data, void *obj)
{
	s_trj_data_ramld *data_ = (s_trj_data_ramld*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_ramld_render(data_, obj_);
}

inline uint8_t trj_data_ramld_reset_(void *data, void *obj)
{
	s_trj_data_ramld *data_ = (s_trj_data_ramld*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_data_ramld_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRJ_UTILS__ */
