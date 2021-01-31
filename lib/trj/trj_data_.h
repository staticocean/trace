
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
	s_trj_eng 	*eng;
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	uint8_t 	ellp_en;
	s_trj_ellp 	*ellp;
	uint32_t 	ellp_hash;
	
	uint32_t 	offset;
	
	vlf_t 		*time;
	vlf_t 		*time3;
	
	uint8_t 	hpr_en;
	vlf_t 		*heading;
	vlf_t 		*pitch;
	vlf_t 		*roll;
	
	uint8_t 	lla_en;
	vlf_t 		*lla_pos;
	vlf_t 		*lla_vel;
	vlf_t 		*lla_acc;
	
	uint8_t 	ecef_en;
	vlf_t 		*ecef_ctn;
	vlf_t 		*ecef_pos;
	vlf_t 		*ecef_vel;
	vlf_t 		*ecef_acc;
	
	uint8_t 	tied_en;
	vlf_t 		*tied_acc;
	vlf_t 		*tied_grs;
	
	char file_name[256];
	TinyMATWriterFile *file_data;
	
} 	s_trj_data_mat;

typedef struct trj_data_mat_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
	uint8_t ellp_en;
	s_trj_ellp *ellp;
	
} 	s_trj_data_mat_init;

inline void __trj_data_mat_null__(s_trj_data_mat *self)
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

inline void __trj_data_mat_free__(s_trj_data_mat *self)
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
	
	__trj_data_mat_null__(self);
	
	return;
}

inline uint8_t trj_data_mat_init(s_trj_data_mat *self, s_trj_data_mat_init attr)
{
	self->file_data = NULL;
	sprintf(self->file_name, "mat file not selected");
	
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->ellp_en = attr.ellp_en;
	self->ellp = attr.ellp;
	if (self->ellp != NULL)
	{ self->ellp_hash = vl_crc32(self->ellp->desc); }
	
	__trj_data_mat_null__(self);
	
	self->tied_en = 0x00;
	self->lla_en  = 0x00;
	self->hpr_en  = 0x00;
	self->ecef_en = 0x00;
	
	return 0x00;
}

inline uint8_t trj_data_mat_save(s_trj_data_mat *self, s_trj_data_mat_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_data_mat_load(s_trj_data_mat *self, s_trj_data_mat_init *attr, uint8_t **v_file)
{
	self->file_data = NULL;
	
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
	
	__trj_data_mat_null__(self);
	
	return 0x00;
}

inline uint8_t trj_data_mat_render(s_trj_data_mat *self, s_trj_obj *obj)
{
	uint32_t i;
	
	__trj_data_mat_free__(self);
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
			self->time[i] = obj->log_list[i].time[0];
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = obj->log_list[i].time[0];
			self->time3[i*3 + 0x01] = obj->log_list[i].time[0];
			self->time3[i*3 + 0x02] = obj->log_list[i].time[0];
			
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
				
				s_trj_rot_hpr hpr;
				trj_ctn_to_hpr(&hpr, ctn_local);
				
				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;
				
				trj_ellp_lla(self->ellp, &self->lla_pos[i*3], &self->ecef_pos[i*3]);

//				vlf_t ecef_p0[3];
//				vlf_t ecef_p1[3];
//				vlf_t ecef_p2[3];
//				vlf_t ref_cnt0[9];
//				vlf_t ref_cnt1[9];
//				vlf_t ref_cnt2[9];
//				vlf_t ecef_cnt[9];
//
//				vl_tnp(ref_cnt0, &self->ref->log_list[i-1].rot[0][0]);
//				vl_tnp(ref_cnt1, &self->ref->log_list[i+0].rot[0][0]);
//				vl_tnp(ref_cnt1, &self->ref->log_list[i+1].rot[0][0]);
//				vl_vsub(ecef_p0, &obj->log_list[i].pos[0][0], &self->ref->log_list[i].pos[0][0]);
//				vl_vsub(ecef_p1, &obj->log_list[i+1].pos[0][0], &self->ref->log_list[i+1].pos[0][0]);
//				vl_mmul_v(ecef_p0, ref_cnt0, ecef_p0);
//				vl_mmul_v(ecef_p1, ref_cnt1, ecef_p1);
//				vl_vsub(&self->lla_vel[i*3], ecef_p1, ecef_p0);
//				vl_vmul_s(&self->lla_vel[i*3], &self->lla_vel[i*3],
//						1.0 / (self->ref->log_list[i+1].time[0] - self->ref->log_list[i].time[0]));
//
//				trj_ellp_ecefrot(self->ellp, ecef_p0, ecef_cnt);
//				vl_tnp(ecef_cnt, ecef_cnt);
//				vl_mmul_v(&self->lla_vel[i*3], ecef_cnt, &self->lla_vel[i*3]);
				
				if (i >= 0x01)
				{
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
					
					vl_mtmul_v(&self->lla_vel[i*3], &self->ref->log_list[i-1].rot[0][0], &self->lla_vel[i*3]);
					vl_mtmul_v(&self->lla_vel[i*3], ecef_ctn_, &self->lla_vel[i*3]);
					
					// swap 1 and 2 because ecef_cnt is proj to NHW not NWH
					vlf_t vh = self->lla_vel[i*3 + 1];
					self->lla_vel[i*3 + 1] = self->lla_vel[i*3 + 2];
					self->lla_vel[i*3 + 2] = vh;
				}
				
				else
				{
					vl_vzero(self->lla_vel);
				}
			}
			
			else
			{
				s_trj_rot_hpr hpr;
				trj_ctn_to_hpr(&hpr, &self->ecef_ctn[i*9]);
				
				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;
			}
			
			vl_vmul_s(&self->tied_acc[i*3], obj->log_list[i].pos_force, 1.0 / obj->pos_inert);
			vl_vsum(&self->tied_acc[i*3], &self->tied_acc[i*3], &obj->log_list[i].pos[2][0]);
			vl_mtmul_v(&self->tied_acc[i*3], &obj->log_list[i].rot[0][0], &self->tied_acc[i*3]);
			
			vlf_t tied_grs[9];
			vl_mtmul_m(tied_grs, &obj->log_list[i].rot[0][0], &obj->log_list[i].rot[1][0]);
			vl_mmul_m(tied_grs, tied_grs, &obj->log_list[i].rot[0][0]);
			vl_unskew(&self->tied_grs[i*3], tied_grs);
		}
	}
	
	self->file_data = TinyMATWriter_open(self->file_name);
	
	if (self->file_data)
	{
		// HPR
		
		if (self->hpr_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "heading", self->heading, 1, self->offset);
		
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "pitch", self->pitch, 1, self->offset);
		
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "roll", self->roll, 1, self->offset);
		}
		
		// LLA
		
		if (self->lla_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "lla_pos", self->lla_pos, 3, self->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "lla_vel", self->lla_vel, 3, self->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "lla_acc", self->lla_acc, 3, self->offset);
		}
		
		// ECEF
		
		if (self->lla_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "ecef_pos", self->ecef_pos, 3, self->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "ecef_vel", self->ecef_vel, 3, self->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "ecef_acc", self->ecef_acc, 3, self->offset);
		}
		
		// TIED
		
		if (self->tied_en != 0x00)
		{
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "tied_acc", self->tied_acc, 3, self->offset);
			
			TinyMATWriter_writeMatrix2D_rowmajor(
					self->file_data, "tied_grs", self->tied_grs, 3, self->offset);
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
	
	__trj_data_mat_free__(self);
	
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
