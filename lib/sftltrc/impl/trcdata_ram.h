
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcdata_ram - trace

#ifndef __TRCDATA_RAM__
#define __TRCDATA_RAM__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcdata.h>

//------------------------------------------------------------------------------

typedef struct trcdata_ram
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	u32_t 	ref_hash;
	
	u8_t 	ellp_en;
	s_trcellp 	*ellp;
	u32_t 	ellp_hash;
	
	u32_t 	offset;
	
	f64_t 		*time;
	f64_t 		*time3;
	
	f64_t 		*ctn;
	f64_t 		*heading;
	f64_t 		*pitch;
	f64_t 		*roll;
	
	f64_t 		*lla_pos;
	f64_t 		*lla_vel;
	f64_t 		*lla_acc;
	
	f64_t 		*ecef_rot;
	f64_t 		*ecef_pos;
	f64_t 		*ecef_vel;
	f64_t 		*ecef_acc;
	
	f64_t 		*tied_acc;
	f64_t 		*tied_grs;

    f64_t       *abs_rot;
    f64_t       *abs_hpr;
    f64_t       *abs_pos;
    f64_t       *abs_vel;
    f64_t       *abs_acc;
	
} 	s_trcdata_ram;

typedef struct trcdata_ram_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
} 	s_trcdata_ram_init;

inline void __trcdata_ram_null__(s_trcdata_ram *self)
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
	
	self->ecef_rot	= NULL;
	self->ecef_pos 	= NULL;
	self->ecef_vel 	= NULL;
	self->ecef_acc 	= NULL;

    self->tied_acc 	= NULL;
    self->tied_grs 	= NULL;

    self->abs_rot 	= NULL;
    self->abs_hpr 	= NULL;
    self->abs_pos 	= NULL;
    self->abs_vel 	= NULL;
    self->abs_acc 	= NULL;

	return;
}

inline void __trcdata_ram_free__(s_trcdata_ram *self)
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
	
	if (self->ecef_rot	!= NULL) free(self->ecef_rot	);
	if (self->ecef_pos 	!= NULL) free(self->ecef_pos 	);
	if (self->ecef_vel 	!= NULL) free(self->ecef_vel 	);
	if (self->ecef_acc 	!= NULL) free(self->ecef_acc 	);

    if (self->tied_acc 	!= NULL) free(self->tied_acc 	);
    if (self->tied_grs 	!= NULL) free(self->tied_grs 	);

    if (self->abs_rot 	!= NULL) free(self->abs_rot 	);
    if (self->abs_hpr 	!= NULL) free(self->abs_hpr 	);
    if (self->abs_pos 	!= NULL) free(self->abs_pos 	);
    if (self->abs_vel 	!= NULL) free(self->abs_vel 	);
    if (self->abs_acc 	!= NULL) free(self->abs_acc 	);
//
	__trcdata_ram_null__(self);
	
	return;
}

inline u8_t trcdata_ram_init(s_trcdata_ram *self, s_trcdata_ram_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->ellp_en = 0x00;
	self->ellp = NULL;
	
	__trcdata_ram_null__(self);
	
	return 0x00;
}

inline u8_t trcdata_ram_save(s_trcdata_ram *self, s_trcdata_ram_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcdata_ram_load(s_trcdata_ram *self, s_trcdata_ram_init *attr, u8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref  = trceng_find_obj (self->eng, self->ref_hash);
	self->ellp = trceng_find_ellp(self->eng, self->ellp_hash);
	
	if (self->ref == NULL)
	{
		self->ref_hash = 0x00;
	}
	
	if (self->ellp == NULL)
	{
		self->ellp_hash = 0x00;
		self->ellp_en = 0x00;
	}
	
	__trcdata_ram_null__(self);
	
	return 0x00;
}

inline u8_t trcdata_ram_render(s_trcdata_ram *self, s_trcobj *obj)
{
	__trcdata_ram_free__(self);
	self->offset = obj->log_sz;
	
	if (self->offset != 0x00)
	{
		self->time 	 	= (f64_t*) malloc(sizeof(f64_t) * self->offset);
		self->time3	 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		
		self->ctn 		= (f64_t*) malloc(sizeof(f64_t) * self->offset * 9);
		self->heading 	= (f64_t*) malloc(sizeof(f64_t) * self->offset);
		self->pitch 	= (f64_t*) malloc(sizeof(f64_t) * self->offset);
		self->roll 		= (f64_t*) malloc(sizeof(f64_t) * self->offset);
		
		self->lla_pos 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		self->lla_vel 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		self->lla_acc 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		
		self->ecef_rot	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 9);
		self->ecef_pos 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		self->ecef_vel 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		self->ecef_acc 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);

        self->tied_acc 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
        self->tied_grs 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);

        self->abs_rot 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 9);
        self->abs_hpr 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
        self->abs_pos 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
        self->abs_vel 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
        self->abs_acc 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);

        for (int i = 0; i < self->offset; ++i)
		{
			s_trcobj_data *log = &obj->log_ls[i];
			self->time[i] = log->time[0];

            vl3m_copy(&self->abs_rot[i*9], &log->rot[0][0]);
            s_vl_hpr abs_hpr = vl_hpr(&self->abs_rot[i*9]);
            self->abs_hpr[i*3 + 0x00] = abs_hpr.heading;
            self->abs_hpr[i*3 + 0x01] = abs_hpr.pitch;
            self->abs_hpr[i*3 + 0x02] = abs_hpr.roll;
            vl3v_copy(&self->abs_pos[i*3], &log->pos[0][0]);
            vl3v_copy(&self->abs_vel[i*3], &log->pos[1][0]);
            vl3_vsumm(&self->abs_acc[i*3], &log->pos[2][0], log->pos_force, -1.0 / obj->pos_inert);
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = log->time[0];
			self->time3[i*3 + 0x01] = log->time[0];
			self->time3[i*3 + 0x02] = log->time[0];
			
			if (self->ref != NULL)
			{
				f64_t ref_cnt[9];
				
				vl3m_tnp(ref_cnt, &self->ref->log_ls[i].rot[0][0]);
				vl3_mmul_m(&self->ecef_rot[i*9], ref_cnt, &obj->log_ls[i].rot[0][0]);
				
				vl3v_subv(&self->ecef_pos[i*3], &obj->log_ls[i].pos[0][0], &self->ref->log_ls[i].pos[0][0]);
				vl3_mmul_v(&self->ecef_pos[i*3], ref_cnt, &self->ecef_pos[i*3]);
				
				vl3_mmul_v(&self->ecef_vel[i*3], ref_cnt, &obj->log_ls[i].pos[1][0]);
				
				vl3_mmul_v(&self->ecef_acc[i*3], ref_cnt, &obj->log_ls[i].pos[2][0]);
			}
			
			else
			{
				vl3m_copy(&self->ecef_rot[i*9], &obj->log_ls[i].rot[0][0]);
				vl3v_copy(&self->ecef_pos[i*3], &obj->log_ls[i].pos[0][0]);
				vl3v_copy(&self->ecef_vel[i*3], &obj->log_ls[i].pos[1][0]);
				vl3v_copy(&self->ecef_acc[i*3], &obj->log_ls[i].pos[2][0]);
			}
			
			if (self->ellp != NULL && self->ref != NULL)
			{
				f64_t ecef_ctn[9];
				f64_t ctn_local[9];
				trcellp_ecefrot(self->ellp, &self->ecef_pos[i*3], ecef_ctn);
				vl3_mtmul_m(ctn_local, ecef_ctn, &self->ecef_rot[i*9]);

				s_vl_hpr hpr = vl_hpr(ctn_local);
				
				vl3m_copy(&self->ctn[i*9], ctn_local);
				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;

				trcellp_lla(self->ellp, &self->lla_pos[i*3], &self->ecef_pos[i*3]);
				
				f64_t ecef_ctn_[9];
				f64_t ecef_pos_[3];

				vl3v_subv(ecef_pos_, &obj->log_ls[i].pos[0][0], &self->ref->log_ls[i].pos[0][0]);
				vl3_mtmul_v(ecef_pos_, &self->ref->log_ls[i].rot[0][0], ecef_pos_);
				trcellp_ecefrot(self->ellp, ecef_pos_, ecef_ctn_);
				
				vl3v_subv(&self->lla_vel[i*3], &obj->log_ls[i].pos[1][0], &self->ref->log_ls[i].pos[1][0]);
				f64_t rel_pos[3];
				vl3v_subv(rel_pos, &obj->log_ls[i].pos[0][0], &self->ref->log_ls[i].pos[0][0]);
				f64_t ang_vel[3];
				vl3_mmul_v(ang_vel, &self->ref->log_ls[i].rot[1][0], rel_pos);
				vl3v_subv(&self->lla_vel[i*3], &self->lla_vel[i*3], ang_vel);

				vl3_mtmul_v(&self->lla_vel[i*3], &self->ref->log_ls[i].rot[0][0], &self->lla_vel[i*3]);
				vl3_mtmul_v(&self->lla_vel[i*3], ecef_ctn_, &self->lla_vel[i*3]);

				// swap 1 and 2 because ecef_cnt is proj to NHW not NWH
				f64_t vh = self->lla_vel[i*3 + 1];
				self->lla_vel[i*3 + 1] = self->lla_vel[i*3 + 2];
				self->lla_vel[i*3 + 2] = vh;
			}
			
			else
			{
				s_vl_hpr hpr = vl_hpr(&self->ecef_rot[i*9]);
				
				self->heading[i] 	= hpr.heading;
				self->pitch[i] 		= hpr.pitch;
				self->roll[i] 		= hpr.roll;
			}
			
			vl3v_muls(&self->tied_acc[i*3], log->pos_force, -1.0 / obj->pos_inert);
			vl3_vsum(&self->tied_acc[i*3], &self->tied_acc[i*3], &log->pos[2][0]);
			vl3_mtmul_v(&self->tied_acc[i*3], &log->rot[0][0], &self->tied_acc[i*3]);
			
			vl3_unskew(&self->tied_grs[i*3], &log->rot[1][0]);
			vl3_mtmul_v(&self->tied_grs[i*3], &log->rot[0][0], &self->tied_grs[i*3]);
		}
	}
	
	return 0x00;
}

inline u8_t trcdata_ram_reset(s_trcdata_ram *self, s_trcobj *obj)
{
	__trcdata_ram_free__(self);
	
	return 0x00;
}

inline u8_t trcdata_ram_init_ (void **data, void *config)
{
	*data = (s_trcdata_ram*) malloc(sizeof(s_trcdata_ram));
	
	s_trcdata_ram *data_ = (s_trcdata_ram*) *data;
	s_trcdata_ram_init *data_init = (s_trcdata_ram_init*) config;
	
	return trcdata_ram_init(data_, *data_init);
}

inline u8_t trcdata_ram_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcdata_ram_load_(void *data, void *config, u8_t **v_file)
{
	s_trcdata_ram_init *attr = (s_trcdata_ram_init*) config;
	s_trcdata_ram *data_ = (s_trcdata_ram*) data;
	
	return trcdata_ram_load(data_, attr, v_file);
}

inline u8_t trcdata_ram_free_ (void **data)
{
	s_trcdata_ram *data_ = (s_trcdata_ram*) *data;
	__trcdata_ram_free__(data_);
	
	free(data_);
	
	return 0x00;
}

inline u8_t trcdata_ram_render_(void *data, void *obj)
{
	s_trcdata_ram *data_ = (s_trcdata_ram*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_ram_render(data_, obj_);
}

inline u8_t trcdata_ram_reset_(void *data, void *obj)
{
	s_trcdata_ram *data_ = (s_trcdata_ram*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_ram_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRCDATA_RAM__ */
