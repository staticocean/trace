
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcdata_ramld - trace

#ifndef __TRCDATA_RAMLD__
#define __TRCDATA_RAMLD__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcdata.h>

//------------------------------------------------------------------------------

typedef struct trcdata_ramld
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
	
	f64_t 		*ld;
	
} 	s_trcdata_ramld;

typedef struct trcdata_ramld_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
} 	s_trcdata_ramld_init;

inline void __trcdata_ramld_null__(s_trcdata_ramld *self)
{
	self->offset = 0x00;
	
	self->time 		= NULL;
	self->time3		= NULL;
	
	self->ld    	= NULL;
	
	return;
}

inline void __trcdata_ramld_free__(s_trcdata_ramld *self)
{
	self->offset = 0x00;
	
	if (self->time 		!= NULL) free(self->time 		);
	if (self->time3		!= NULL) free(self->time3 		);
	
	if (self->ld 	 	!= NULL) free(self->ld       	);
	
	__trcdata_ramld_null__(self);
	
	return;
}

inline u8_t trcdata_ramld_init(s_trcdata_ramld *self, s_trcdata_ramld_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->ellp_en = 0x00;
	self->ellp = NULL;
	
	__trcdata_ramld_null__(self);
	
	return 0x00;
}

inline u8_t trcdata_ramld_save(s_trcdata_ramld *self, s_trcdata_ramld_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcdata_ramld_load(s_trcdata_ramld *self, s_trcdata_ramld_init *attr, u8_t **v_file)
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
	
	__trcdata_ramld_null__(self);
	
	return 0x00;
}

inline u8_t trcdata_ramld_render(s_trcdata_ramld *self, s_trcobj *obj)
{
	__trcdata_ramld_free__(self);
	self->offset = obj->log_sz;
	
	if (self->offset != 0x00)
	{
		self->time 	 	= (f64_t*) malloc(sizeof(f64_t) * self->offset);
		self->time3	 	= (f64_t*) malloc(sizeof(f64_t) * self->offset * 3);
		
		self->ld 		= (f64_t*) malloc(sizeof(f64_t) * self->offset);
		
		f64_t ecef_f[3]; // first
		f64_t ecef_l[3]; // last
		
		if (self->ref != NULL)
		{
			vl3_vsub(ecef_f, &obj->log_ls[0].pos[0][0], &self->ref->log_ls[0].pos[0][0]);
			vl3_vsub(ecef_l, &obj->log_ls[self->ref->log_sz-1].pos[0][0], &self->ref->log_ls[self->ref->log_sz-1].pos[0][0]);
			vl3_mtmul_v(ecef_f, &self->ref->log_ls[0].rot[0][0], ecef_f);
			vl3_mtmul_v(ecef_l, &self->ref->log_ls[self->ref->log_sz-1].rot[0][0], ecef_l);
		}
		
		for (int i = 0; i < self->offset; ++i)
		{
			s_trcobj_data *log = &obj->log_ls[i];
			self->time[i] = log->time[0];
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = log->time[0];
			self->time3[i*3 + 0x01] = log->time[0];
			self->time3[i*3 + 0x02] = log->time[0];
			
			f64_t ecef[3];
			
			if (self->ref != NULL && self->ellp != NULL && self->ellp_en != 0x00)
			{
				vl3_vsub(ecef, &obj->log_ls[i].pos[0][0], &self->ref->log_ls[i].pos[0][0]);
				vl3_mtmul_v(ecef, &self->ref->log_ls[i].rot[0][0], ecef);
				
				f64_t trje[3];
				vl3_vsub(trje, ecef_l, ecef_f);
				vl3_vmul_s(trje, trje, 1.0 / vl3_vnorm(trje));
				
				f64_t ort[3];
				vl3_vsub(ort, ecef, ecef_f);
				
				f64_t path = vl3_vdot(ort, trje);
				vl3_vsumm(ort, ort, trje, -path);
				
				f64_t ref_pos[3];
				vl3_vsub(ref_pos, ecef, ort);
				
				vl3_vmul_s(ecef, ecef, 1.0 / vl3_vnorm(ecef));
				vl3_vmul_s(ort, ort, 1.0 / vl3_vnorm(ort));
				vl3_vmul_s(ref_pos, ref_pos, 1.0 / vl3_vnorm(ref_pos));
				
				f64_t arc_cos = vl3_vdot(ref_pos, ecef);
				f64_t arc = acos(arc_cos);
				
				f64_t right_e[3];
				vl3_cross(right_e, ecef, trje);
				
				if (vl3_vdot(ort, right_e) < 0.0) { arc = -arc; }
				
				self->ld[i] = arc * self->ellp->a;
			}
			
			else
			{
				vl3_vcopy(ecef, &obj->log_ls[i].pos[0][0]);
			}
		}
	}
	
	return 0x00;
}

inline u8_t trcdata_ramld_reset(s_trcdata_ramld *self, s_trcobj *obj)
{
	__trcdata_ramld_free__(self);
	
	return 0x00;
}

inline u8_t trcdata_ramld_init_ (void **data, void *config)
{
	*data = (s_trcdata_ramld*) malloc(sizeof(s_trcdata_ramld));
	
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) *data;
	s_trcdata_ramld_init *data_init = (s_trcdata_ramld_init*) config;
	
	return trcdata_ramld_init(data_, *data_init);
}

inline u8_t trcdata_ramld_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcdata_ramld_load_(void *data, void *config, u8_t **v_file)
{
	s_trcdata_ramld_init *attr = (s_trcdata_ramld_init*) config;
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) data;
	
	return trcdata_ramld_load(data_, attr, v_file);
}

inline u8_t trcdata_ramld_free_ (void **data)
{
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) *data;
	__trcdata_ramld_free__(data_);
	
	free(data_);
	
	return 0x00;
}

inline u8_t trcdata_ramld_render_(void *data, void *obj)
{
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_ramld_render(data_, obj_);
}

inline u8_t trcdata_ramld_reset_(void *data, void *obj)
{
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_ramld_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRCDATA_RAMLD__ */
