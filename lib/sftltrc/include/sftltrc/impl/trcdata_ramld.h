
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
	t_u32 	ref_hash;
	
	t_u8 	ellp_en;
	s_trcellp 	*ellp;
	t_u32 	ellp_hash;
	
	t_u32 	offset;
	
	t_f64 		*time;
	t_f64 		*time3;
	
	t_f64 		*ld;
	
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

inline t_u8 trcdata_ramld_init(s_trcdata_ramld *self, s_trcdata_ramld_init attr)
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

inline t_u8 trcdata_ramld_save(s_trcdata_ramld *self, s_trcdata_ramld_init *attr, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trcdata_ramld_load(s_trcdata_ramld *self, s_trcdata_ramld_init *attr, t_u8 **v_file)
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

inline t_u8 trcdata_ramld_render(s_trcdata_ramld *self, s_trcobj *obj)
{
	__trcdata_ramld_free__(self);
	self->offset = obj->log_sz;
	
	if (self->offset != 0x00)
	{
		self->time 	 	= (t_f64*) malloc(sizeof(t_f64) * self->offset);
		self->time3	 	= (t_f64*) malloc(sizeof(t_f64) * self->offset * 3);
		
		self->ld 		= (t_f64*) malloc(sizeof(t_f64) * self->offset);
		
		t_f64 ecef_f[3]; // first
		t_f64 ecef_l[3]; // last
		
		if (self->ref != NULL)
		{
			vld3v_subv(ecef_f, &obj->log_ls[0].pos[0][0], &self->ref->log_ls[0].pos[0][0]);
			vld3v_subv(ecef_l, &obj->log_ls[self->ref->log_sz-1].pos[0][0], &self->ref->log_ls[self->ref->log_sz-1].pos[0][0]);
			vld3m_tmulv(ecef_f, &self->ref->log_ls[0].rot[0][0], ecef_f);
			vld3m_tmulv(ecef_l, &self->ref->log_ls[self->ref->log_sz-1].rot[0][0], ecef_l);
		}
		
		for (int i = 0; i < self->offset; ++i)
		{
			s_trcobj_data *log = &obj->log_ls[i];
			self->time[i] = log->time[0];
			
			// support for implot strange api
			self->time3[i*3 + 0x00] = log->time[0];
			self->time3[i*3 + 0x01] = log->time[0];
			self->time3[i*3 + 0x02] = log->time[0];
			
			t_f64 ecef[3];
			
			if (self->ref != NULL && self->ellp != NULL && self->ellp_en != 0x00)
			{
				vld3v_subv(ecef, &obj->log_ls[i].pos[0][0], &self->ref->log_ls[i].pos[0][0]);
				vld3m_tmulv(ecef, &self->ref->log_ls[i].rot[0][0], ecef);
				
				t_f64 trje[3];
				vld3v_subv(trje, ecef_l, ecef_f);
				vld3v_muls(trje, trje, 1.0 / vld3v_norm(trje));
				
				t_f64 ort[3];
				vld3v_subv(ort, ecef, ecef_f);
				
				t_f64 path = vld3v_dot(ort, trje);
				vl3_vsumm(ort, ort, trje, -path);
				
				t_f64 ref_pos[3];
				vld3v_subv(ref_pos, ecef, ort);
				
				vld3v_muls(ecef, ecef, 1.0 / vld3v_norm(ecef));
				vld3v_muls(ort, ort, 1.0 / vld3v_norm(ort));
				vld3v_muls(ref_pos, ref_pos, 1.0 / vld3v_norm(ref_pos));
				
				t_f64 arc_cos = vld3v_dot(ref_pos, ecef);
				t_f64 arc = acos(arc_cos);
				
				t_f64 right_e[3];
				vld3v_cross(right_e, ecef, trje);
				
				if (vld3v_dot(ort, right_e) < 0.0) { arc = -arc; }
				
				self->ld[i] = arc * self->ellp->a;
			}
			
			else
			{
				vld3v_copy(ecef, &obj->log_ls[i].pos[0][0]);
			}
		}
	}
	
	return 0x00;
}

inline t_u8 trcdata_ramld_reset(s_trcdata_ramld *self, s_trcobj *obj)
{
	__trcdata_ramld_free__(self);
	
	return 0x00;
}

inline t_u8 trcdata_ramld_init_ (void **data, void *config)
{
	*data = (s_trcdata_ramld*) malloc(sizeof(s_trcdata_ramld));
	
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) *data;
	s_trcdata_ramld_init *data_init = (s_trcdata_ramld_init*) config;
	
	return trcdata_ramld_init(data_, *data_init);
}

inline t_u8 trcdata_ramld_save_(void *data, void *config, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trcdata_ramld_load_(void *data, void *config, t_u8 **v_file)
{
	s_trcdata_ramld_init *attr = (s_trcdata_ramld_init*) config;
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) data;
	
	return trcdata_ramld_load(data_, attr, v_file);
}

inline t_u8 trcdata_ramld_free_ (void **data)
{
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) *data;
	__trcdata_ramld_free__(data_);
	
	free(data_);
	
	return 0x00;
}

inline t_u8 trcdata_ramld_render_(void *data, void *obj)
{
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_ramld_render(data_, obj_);
}

inline t_u8 trcdata_ramld_reset_(void *data, void *obj)
{
	s_trcdata_ramld *data_ = (s_trcdata_ramld*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcdata_ramld_reset(data_, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRCDATA_RAMLD__ */
